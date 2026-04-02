#pragma once

#include <functional>
#include <memory>
#include <string>

#include "net/tcp/common.h"

namespace msghub::net {
  class ISocketHandler {
   public:
    virtual ~ISocketHandler() = default;
    virtual void OnReceived(msghub::net::TcpMessage&& tcp_msg) = 0;
    virtual void OnError() = 0;
    virtual void OnClose() = 0;

   protected:
    ISocketHandler() = default;
  };

  class Socket {
   public:
    Socket(std::shared_ptr<ISocketHandler> handler = nullptr);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    bool Connect(const std::string& host, int port, int timeout_ms = 5000);
    void Close();
    void Shutdown();
    bool IsConnected() const noexcept;
    bool IsValid() const noexcept;

    std::string Host() const;
    std::string IP() const;
    uint16_t Port() const;
    uint16_t LocalPort() const;
    int GetFd() const noexcept;

   public:
    ssize_t Recv(uint8_t* buff, size_t buff_len);
    ssize_t Send(const uint8_t* data, size_t len);

   public:
    using RecvDataCb = std::function<ssize_t(int fd, uint8_t* buf, size_t len)>;
    void Run(RecvDataCb recv_data = nullptr);

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::net