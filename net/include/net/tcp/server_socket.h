#pragma once
#include <memory>
#include <string>
#include <vector>

#include "common.h"

namespace msghub::net {
  class TcpSvHandler {
   public:
    virtual ~TcpSvHandler() = default;
    virtual void OnReceived(TcpMessage&& msg) = 0;
    virtual void OnNewConnection(int fd) = 0;
    virtual void OnClose(int fd) = 0;
    // virtual void OnError(int fd) = 0;

   protected:
    TcpSvHandler() = default;
  };

  class ServerSocket {
   public:
    ServerSocket(std::shared_ptr<TcpSvHandler> handler = nullptr);
    ~ServerSocket();

    ServerSocket(const ServerSocket&) = delete;
    ServerSocket& operator=(const ServerSocket&) = delete;
    ServerSocket(ServerSocket&& other) noexcept;
    ServerSocket& operator=(ServerSocket&& other) noexcept;
    bool Start(int port);
    void RegisterEventHandler(std::shared_ptr<TcpSvHandler> handler);

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace msghub::net