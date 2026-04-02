#include <stdlib.h>
#include <cstdint>
#include <memory>
#include <string>

#include "net/tcp/socket.h"
#include "tcp_connection.h"

namespace msghub::core {
  struct TcpConnection::Impl {
   public:
    Impl(std::string host, int32_t port)
        : host_(std::move(host)), port_(port) {}
    ~Impl() = default;

    bool Open() { return socket_.Connect(host_, port_); }
    void Close() { socket_.Close(); }
    int Send(const uint8_t* data, size_t len) { return 0; }
    int Receive(uint8_t* buf, size_t max_len) { return 0; }
    bool IsConnected() const { return false; }
    int GetFd() const { return -1; }

   private:
    net::Socket socket_;
    std::string host_;
    int32_t port_;
    int sock_fd_{-1};
    bool is_connected_{false};
  };
  TcpConnection::~TcpConnection() = default;

  TcpConnection::TcpConnection(std::string host, int32_t port)
      : impl_(std::make_unique<Impl>(std::move(host), port)) {}

  int TcpConnection::Send(const uint8_t* data, size_t len) {
    return impl_->Send(data, len);
  }
  int TcpConnection::Receive(uint8_t* buf, size_t max_len) {
    return impl_->Receive(buf, max_len);
  }
  bool TcpConnection::IsConnected() const {
    return impl_->IsConnected();
  }
  int TcpConnection::GetFd() const {
    return impl_->GetFd();
  }
  bool TcpConnection::Open() {
    return impl_->Open();
  }
  void TcpConnection::Close() {
    impl_->Close();
  }
  void TcpConnection::Run() {
    impl_->Run();
  }
}  // namespace msghub::core