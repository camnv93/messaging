#include "common/utils/error.h"

namespace msghub::common {

std::string ToString(Error err) noexcept
{
    switch (err)
    {
        case Error::Ok:                        return "Ok";

        // Buffer
        case Error::Buffer_Underflow:          return "Buffer Underflow";
        case Error::Buffer_Overflow:           return "Buffer Overflow";
        case Error::Buffer_InvalidOffset:      return "Buffer Invalid Offset";
        case Error::Buffer_Empty:              return "Buffer Empty";

        // Message
        case Error::Message_InvalidMagic:      return "Message Invalid Magic";
        case Error::Message_InvalidVersion:    return "Message Invalid Version";
        case Error::Message_InvalidType:       return "Message Invalid Type";
        case Error::Message_HeaderCRC8Failed:  return "Message Header CRC8 Failed";
        case Error::Message_CRC32Failed:       return "Message CRC32 Failed";
        case Error::Message_PayloadTooLarge:   return "Message Payload Too Large";
        case Error::Message_InvalidHeader:     return "Message Invalid Header";

        // TLV
        case Error::TLV_InvalidType:           return "TLV Invalid Type";
        case Error::TLV_LengthMismatch:        return "TLV Length Mismatch";
        case Error::TLV_BufferTooSmall:        return "TLV Buffer Too Small";
        case Error::TLV_UnexpectedEnd:         return "TLV Unexpected End of Buffer";

        // Payload
        case Error::Payload_NullData:          return "Payload Null Data";
        case Error::Payload_SizeZero:          return "Payload Size Zero";

        // Codec
        case Error::Codec_EncodeFailed:        return "Codec Encode Failed";
        case Error::Codec_DecodeFailed:        return "Codec Decode Failed";
        case Error::Codec_InsufficientBuffer:  return "Codec Insufficient Buffer";

        // Utils
        case Error::Utils_InvalidHexString:    return "Utils Invalid Hex String";
        case Error::Utils_ConversionFailed:    return "Utils Conversion Failed";

        // General
        case Error::InvalidArgument:           return "Invalid Argument";
        case Error::NotSupported:              return "Not Supported";
        case Error::OutOfMemory:               return "Out Of Memory";
        case Error::InternalError:             return "Internal Error";

        case Error::Unknown:
        default:                               return "Unknown Error";
    }
}

} // namespace msghub::common