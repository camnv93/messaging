#include <cstring>
#include <memory>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <poll.h>
#include <functional>
#include <iostream>
#include <unordered_set>

#include "net/net_exception.h"
#include "net/tcp/socket.h"
#include "net/tcp/utils.h"

namespace msghub::net {
  struct Socket::Impl {
    Impl(std::shared_ptr<ISocketHandler> handler) : handler_(handler) {}
    ~Impl() = default;
    bool Connect(const std::string& host, int port, int timeout_ms) {
      if (is_connected_)
        Close();

      struct addrinfo hints{}, *servinfo = nullptr;
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;

      int rv = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints,
                           &servinfo);
      if (rv != 0)
        throw NetException("Line: " + std::to_string(__LINE__) + ", " +
                           gai_strerror(rv));

      // Đảm bảo giải phóng bộ nhớ tự động
      std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> addr_ptr(
        servinfo, freeaddrinfo);

      for (struct addrinfo* p = servinfo; p != nullptr; p = p->ai_next) {
        sockfd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd_ == -1)
          continue;

        // BƯỚC 1: Thiết lập Non-blocking
        int flags = fcntl(sockfd_, F_GETFL, 0);
        if (flags == -1) {
          ::close(sockfd_);
          continue;
        }
        fcntl(sockfd_, F_SETFL, flags | O_NONBLOCK);

        // BƯỚC 2: Gọi connect
        int res = ::connect(sockfd_, p->ai_addr, p->ai_addrlen);

        if (res < 0) {
          if (errno == EINPROGRESS) {
            // BƯỚC 3: Dùng select để chờ đợi kết nối trong timeout_ms
            fd_set write_fds;
            FD_ZERO(&write_fds);
            FD_SET(sockfd_, &write_fds);

            struct timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

            // select trả về: >0 (sẵn sàng), 0 (timeout), -1 (lỗi hệ thống)
            res = select(sockfd_ + 1, nullptr, &write_fds, nullptr, &tv);

            if (res > 0) {
              // BƯỚC 4: Kiểm tra xem có lỗi ngầm nào không (getsockopt)
              int so_error = 0;
              socklen_t len = sizeof(so_error);
              if (getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, &so_error, &len) <
                  0) {
                res = -1;  // Lỗi hệ thống khi lấy option
              } else if (so_error != 0) {
                errno =
                  so_error;  // Gán lại errno để báo đúng lỗi (Connection Refused, v.v.)
                res = -1;
              } else {
                res = 0;  // Thành công rực rỡ!
              }
            } else if (res == 0) {
              errno = ETIMEDOUT;  // Gán lỗi quá thời gian
              res = -1;
            }
          }
        }

        if (res == -1) {
          ::close(sockfd_);
          sockfd_ = -1;
          continue;
        }
        break;
      }

      if (sockfd_ == -1) {
        throw NetException("Failed to connect to " + host + ": " +
                           strerror(errno));
        return false;
      }

      is_connected_ = true;
      this->host_ = host;
      this->port_ = port;
      return true;
    }

    void Close() {
      if (sockfd_ != -1) {
        ::close(sockfd_);
        sockfd_ = -1;
        is_connected_ = false;
      }
    }
    void Shutdown() {
      if (sockfd_ != -1) {
        ::shutdown(sockfd_, SHUT_RDWR);
        ::close(sockfd_);
        sockfd_ = -1;
        is_connected_ = false;
      }
    }

    bool IsConnected() const noexcept { return is_connected_; }

    ssize_t Send(const uint8_t* data, size_t len) {
      if (sockfd_ == -1) {
        throw NetException("Socket is not connected");
      }
      ssize_t total_sent = 0;
      while (total_sent < static_cast<ssize_t>(len)) {
        ssize_t sent = send(sockfd_, data + total_sent, len - total_sent, 0);
        if (sent == -1) {
          if (errno == EINTR) {
            continue;  // Retry if interrupted
          }
          throw NetException("Failed to send data: " +
                             std::string(strerror(errno)));
        }
        total_sent += sent;
      }
      return total_sent;
    }

    ssize_t Receive(uint8_t* buffer, size_t buffer_size) {
      if (sockfd_ == -1) {
        throw NetException("Socket is not connected");
      }
      auto ret = Utils::RecvNonBlocking(sockfd_, buffer, buffer_size);
      if (ret < 0) {
        is_connected_ = false;
      }
      return ret;
    }

    bool IsValid() const noexcept { return sockfd_ != -1; }

    std::string Host() const;
    std::string IP() const;
    uint16_t Port() const;
    uint16_t LocalPort() const;

    int GetFd() const noexcept;

    void Run(RecvDataCb recv_data) {
      if (!is_connected_) {
        //if (on_error_) on_error_("Not connected");
        std::cout << "############## Not connected ##############" << std::endl;
        return;
      }
      std::cout << "############## SOCKET RUN ##############" << std::endl;
      bool running = true;
      auto recv = recv_data;
      if (!recv) {
        recv = [this](int32_t fd, uint8_t* b, size_t s) {
          return Utils::RecvNonBlocking(fd, b, s);
        };
      }

      while (running && is_connected_) {
        struct pollfd pfd{};
        pfd.fd = sockfd_;
        pfd.events = POLLIN | POLLHUP | POLLERR;

        int ret = poll(&pfd, 1, 100);

        if (ret < 0) {
          if (errno == EINTR)
            continue;
          //if (on_error_) on_error_("Poll error: " + std::string(strerror(errno)));
          break;
        }

        if (ret > 0) {
          if (pfd.revents & (POLLERR | POLLHUP)) {
            is_connected_ = false;
            //if (on_close_) on_close_();
            break;
          }

          // 2. Có dữ liệu để đọc
          if (pfd.revents & POLLIN) {
            std::vector<uint8_t> data;
            data.reserve(4096);
            auto ret = Utils::ReadAll(sockfd_, data);
            if (ret > 0) {
              if (handler_) {
                handler_->OnReceived(TcpMessage(std::move(data), sockfd_));
              }
            } else if (ret == 0) {

            } else {
              // Server chủ động đóng kết nối (FIN)
              is_connected_ = false;
              // if (on_close_)
              //   on_close_();
              // Lỗi Receive (n < 0)
              //if (on_error_) on_error_("Receive error");
              break;
            }
          }
        }
        // Ở đây bạn có thể thêm logic "Keep-alive" hoặc "Heartbeat" nếu cần
      }
    }

   public:
    std::shared_ptr<ISocketHandler> handler_;
    int sockfd_ = -1;
    std::string host_;
    std::string ip_;
    uint16_t port_ = 0;
    uint16_t local_port_ = 0;
    size_t buff_size_{0};
    bool is_connected_ = false;
  };
  Socket::Socket(std::shared_ptr<ISocketHandler> handler)
      : impl_(std::make_unique<Impl>(handler)) {}
  Socket::~Socket() = default;
  Socket::Socket(Socket&& other) noexcept = default;
  Socket& Socket::operator=(Socket&& other) noexcept = default;

  bool Socket::Connect(const std::string& host, int port, int timeout_ms) {
    return impl_->Connect(host, port, timeout_ms);
  }

  void Socket::Close() {
    impl_->Close();
  }

  void Socket::Shutdown() {
    impl_->Shutdown();
  }
  void Socket::Run(RecvDataCb recv_data) {
    impl_->Run(recv_data);
  }
  bool Socket::IsConnected() const noexcept {
    return impl_->IsConnected();
  }

  ssize_t Socket::Send(const uint8_t* data, size_t len) {
    return impl_->Send(data, len);
  }

  ssize_t Socket::Recv(uint8_t* buffer, size_t buffer_size) {
    return impl_->Receive(buffer, buffer_size);
  }

  bool Socket::IsValid() const noexcept {
    return true;
  }

  std::string Socket::Host() const {
    return "";
  }

  std::string Socket::IP() const {
    return "";
  }

  uint16_t Socket::Port() const {
    return 0;
  }

  uint16_t Socket::LocalPort() const {
    return 0;
  }

  int Socket::GetFd() const noexcept {
    return 0;
  }
}  // namespace msghub::net