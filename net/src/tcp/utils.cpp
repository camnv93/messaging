#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "net/tcp/utils.h"

namespace msghub::net {
  ssize_t Utils::RecvNonBlocking(int32_t fd, uint8_t* buffer,
                                 size_t buffer_size) {
    if (fd == -1)
      return -1;
    while (true) {
      ssize_t received = ::recv(fd, buffer, buffer_size, 0);
      if (received < 0) {
        std::cout << "received: " << received << ", errno: " << errno
                  << std::endl;
        // Trường hợp 1: Buffer trống (Normal cho Non-blocking)
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          return 0;  // Trả về 0 byte đọc được
        }
        // Trường hợp 2: Bị ngắt bởi Signal hệ thống
        if (errno == EINTR) {
          continue;  // Thử lại ngay lập tức bằng vòng lặp, không dùng đệ quy
        }
        // Trường hợp 3: Lỗi kết nối thực sự (ECONNRESET, ETIMEDOUT, v.v.)
        return -1;
      }
      if (received == 0) {
        // Trường hợp 4: Peer chủ động đóng kết nối (Graceful shutdown)
        return -1;  // Trả về -1 hoặc một hằng số đặc biệt để báo đứt kết nối
      }
      return received;  // Trả về số byte thực tế đọc được (> 0)
    }
  }

  ssize_t Utils::ReadAll(int32_t fd, std::vector<uint8_t>& out) {
    uint8_t buffer[4096]{};
    while (true) {
      ssize_t n = ::recv(fd, buffer, sizeof(buffer), 0);

      if (n > 0) {
        out.insert(out.end(), buffer, buffer + n);
        continue;
      }
      if (n == 0) {
        // client đóng kết nối
        return -1;
      }
      // n < 0
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // đọc hết rồi
        return out.size();
      }
      if (errno == EINTR) {
        continue;
      }
      // lỗi thật
      return -1;
    }
  }
}  // namespace msghub::net