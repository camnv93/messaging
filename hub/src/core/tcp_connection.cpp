#include "hub/core/tcp_connection.h"

namespace msghub::hub {

  TcpConnection::TcpConnection(int32_t fd) : fd_(fd) {}
  TcpConnection::~TcpConnection() {}

  void TcpConnection::Close() {}
  bool TcpConnection::IsConnected() const {
    return true;
  }
  int32_t TcpConnection::GetFd() const {
    return this->fd_;
  }
}  // namespace msghub::hub