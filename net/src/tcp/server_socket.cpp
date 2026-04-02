
#include "net/tcp/server_socket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

#include "net/tcp/utils.h"

#define MAX_EVENTS 1024

namespace msghub::net {
  struct ServerSocket::Impl {
    Impl() {};
    ~Impl() {};

    bool Init() {
      struct addrinfo hints{}, *servinfo, *p;
      int yes = 1;
      int rv;

      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      std::string port_str = std::to_string(port_);

      if ((rv = getaddrinfo(nullptr, port_str.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
      }

      for (p = servinfo; p != nullptr; p = p->ai_next) {
        sockfd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd_ == -1) {
          perror("socket");
          continue;
        }

        // reuse addr
        setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

#ifdef SO_REUSEPORT
        setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
#endif

        // ⚡ QUAN TRỌNG: set non-blocking
        int flags = fcntl(sockfd_, F_GETFL, 0);
        fcntl(sockfd_, F_SETFL, flags | O_NONBLOCK);

        if (bind(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd_);
          perror("bind");
          continue;
        }

        break;
      }

      freeaddrinfo(servinfo);

      if (p == nullptr) {
        fprintf(stderr, "failed to bind\n");
        return false;
      }

      // listen
      if (listen(sockfd_, SOMAXCONN) == -1) {
        perror("listen");
        return false;
      }

      return true;
    }

    void Run() {
      int epfd = epoll_create1(0);
      if (epfd == -1) {
        perror("epoll_create1");
        return;
      }

      // add listen socket vào epoll
      epoll_event ev{};
      ev.events = EPOLLIN;
      ev.data.fd = sockfd_;

      if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd_, &ev) == -1) {
        perror("epoll_ctl");
        return;
      }

      epoll_event events[MAX_EVENTS];
      std::cout << "######## SERVER RUNNING ###########" << std::endl;

      while (true) {
        std::cout << "######## ---RUNNING--- ###########" << std::endl;
        int n = epoll_wait(epfd, events, MAX_EVENTS, 10);
        if (n == -1) {
          if (errno == EINTR)
            continue;
          perror("epoll_wait");
          break;
        }

        for (int i = 0; i < n; ++i) {
          if (events[i].data.fd == sockfd_) {
            // 🔥 accept loop (CỰC QUAN TRỌNG)
            while (true) {
              sockaddr_storage client_addr;
              socklen_t len = sizeof(client_addr);

              int client_fd = accept(sockfd_, (sockaddr*)&client_addr, &len);

              if (client_fd == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                  break;  // hết connection pending
                } else {
                  perror("accept");
                  break;
                }
              }
              // set client non-blocking
              int flags = fcntl(client_fd, F_GETFL, 0);
              fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

              // add client vào epoll
              epoll_event client_ev{};
              client_ev.events = EPOLLIN | EPOLLET;  // edge-trigger
              client_ev.data.fd = client_fd;

              if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
                perror("epoll_ctl client");
                close(client_fd);
                continue;
              }
              printf("New connection: fd=%d\n", client_fd);
              if (handler_) {
                handler_->OnNewConnection(client_fd);
              }
            }
          } else {
            int fd = events[i].data.fd;
            std::vector<uint8_t> data;
            data.reserve(4096);
            auto ret = Utils::ReadAll(fd, data);
            if (ret > 0) {
              std::cout << "recv: " << data.size() << std::endl;
              if (handler_) {
                handler_->OnReceived(TcpMessage(std::move(data), fd));
              }
            } else {
              std::cout << "close" << std::endl;
              if (handler_) {
                handler_->OnClose(fd);
              }
            }
          }
        }
      }

      close(epfd);
    }

   public:
    std::shared_ptr<TcpSvHandler> handler_;
    int port_{0};
    int sockfd_{-1};
  };

  ServerSocket::ServerSocket(std::shared_ptr<TcpSvHandler> handler)
      : impl_(std::make_unique<Impl>()) {
    impl_->handler_ = handler;
  };

  ServerSocket::~ServerSocket() {}

  bool ServerSocket::Start(int port) {
    impl_->port_ = port;
    impl_->Init();
    impl_->Run();
    return true;
  }
}  // namespace msghub::net