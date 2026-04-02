// tlv_reader.h

#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "common/buffer/byte_buffer.h"
#include "common/codec/tlv/tlv.h"
#include "common/codec/tlv/tlv_view.h"

namespace msghub::common {
  class TlvReaderImpl;
  class TlvReader {
   public:
   public:
    explicit TlvReader(const uint8_t* buffer, size_t len) noexcept;
    ~TlvReader();

    TlvReader(const TlvReader&) = delete;
    TlvReader& operator=(const TlvReader&) = delete;

    TlvReader(TlvReader&& other) noexcept;
    TlvReader& operator=(TlvReader&& other) noexcept;

    TlvView Next();
    TlvView Find(TlvType type);

    void Reset();
    bool HasNext() const;

   private:
    std::unique_ptr<TlvReaderImpl> impl_;
  };
}  // namespace msghub::common