#pragma once

#include <cstdint>
#include <string>

namespace msghub::common {
enum class Error : uint32_t {
    Ok = 0,

    // Buffer errors
    Buffer_Underflow,
    Buffer_Overflow,
    Buffer_InvalidOffset,
    Buffer_Empty,

    // Message / Header errors
    Message_InvalidMagic,
    Message_InvalidVersion,
    Message_InvalidType,
    Message_HeaderCRC8Failed,
    Message_CRC32Failed,
    Message_PayloadTooLarge,
    Message_InvalidHeader,
    Message_InvalidPayload,
    Message_InvalidChecksum,

    // TLV errors
    TLV_InvalidType,
    TLV_InvalidState,
    TLV_LengthMismatch,
    TLV_BufferTooSmall,
    TLV_LengthOverflow,
    TLV_UnexpectedEnd,

    // Payload errors
    Payload_NullData,
    Payload_SizeZero,

    // Codec / Serialize errors
    Codec_EncodeFailed,
    Codec_DecodeFailed,
    Codec_InsufficientBuffer,

    // Utils errors
    Utils_InvalidHexString,
    Utils_ConversionFailed,

    // General errors
    InvalidArgument,
    NotSupported,
    OutOfMemory,
    InternalError,
    Corruption,

    Unknown = 0xFFFFFFFFu
};

std::string ToString(Error err) noexcept;

inline constexpr bool IsOk(Error err) noexcept {
    return err == Error::Ok;
}

inline constexpr bool IsError(Error err) noexcept {
    return err != Error::Ok;
}

#define CHECK_ERROR(expr) \
    do { \
        ::msghub::common::Error __err = (expr); \
        if (::msghub::common::IsError(__err)) { \
            return __err; \
        } \
    } while (false);

} // namespace msghub::common