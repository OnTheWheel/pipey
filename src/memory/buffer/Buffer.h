#ifndef PIPEY_BUFFER_H
#define PIPEY_BUFFER_H

#include "../../common/common.h"
#include "../../util/ByteOrder.h"
#include "../pool/MemoryPool.h"

namespace pipey {
	namespace memory {
		namespace buffer {

			struct IO_BUFFER {
				SCATTERED_BUFFER *pBuffer;
				uint32_t nBuffer;
			};

			class IBuffer
			{
			public:
				IBuffer() {};
				virtual ~IBuffer() {};

			public:

				virtual uint32_t GetCapacity() const = 0;
				virtual uint32_t GetReadIndex() const = 0;
				virtual uint32_t GetWriteIndex() const = 0;
				virtual uint32_t GetReadableLength() const = 0;

				virtual IO_BUFFER *GetIOBuffer() = 0;

				virtual const ::pipey::memory::pool::CMemoryHandle &GetMemoryHandle() const = 0;
				virtual ::pipey::memory::pool::IMemoryPool *GetMemoryPool() const = 0;

				virtual uint32_t SetReadIndex(const uint32_t &nIndex) = 0;
				virtual uint32_t SetWriteIndex(const uint32_t &nIndex) = 0;

				virtual bool IsReadable() const = 0;
				virtual bool IsWritable() const = 0;

				virtual ::pipey::util::BYTE_ENDIAN GetEndian() const = 0;

				virtual bool EnsureWritable(const uint32_t &nLength) = 0;

				virtual uint32_t Find(const char &value) const = 0;
				virtual uint32_t Find(const uint32_t &nStartIndex, const uint32_t &nLength, const char &value) const = 0;

				virtual char ReadChar() = 0;
				virtual unsigned char ReadUnsignedChar() = 0;

				virtual int16_t ReadInteger16() = 0;
				virtual uint16_t ReadUnsignedInteger16() = 0;

				virtual int32_t ReadInteger32() = 0;
				virtual uint32_t ReadUnsignedInteger32() = 0;

				virtual int64_t ReadInteger64() = 0;
				virtual uint64_t ReadUnsignedInteger64() = 0;

				virtual float ReadFloat() = 0;

				virtual double ReadDouble() = 0;

				virtual void ReadBytes(char *pBuffer, const uint32_t &nLength) = 0;
				virtual void ReadBytes(IBuffer *pBuffer, const uint32_t &nLength) = 0;

				virtual void ReadString(char *pBuffer, const uint32_t &nSize) = 0;
				virtual void ReadString(IBuffer *pBuffer) = 0;

				virtual char GetChar(const uint32_t &nIndex) const = 0;
				virtual unsigned char GetUnsignedChar(const uint32_t &nIndex) const = 0;

				virtual int16_t GetInteger16(const uint32_t &nIndex) const = 0;
				virtual uint16_t GetUnsignedInteger16(const uint32_t &nIndex) const = 0;

				virtual int32_t GetInteger32(const uint32_t &nIndex) const = 0;
				virtual uint32_t GetUnsignedInteger32(const uint32_t &nIndex) const = 0;

				virtual int64_t GetInteger64(const uint32_t &nIndex) const = 0;
				virtual uint64_t GetUnsignedInteger64(const uint32_t &nIndex) const = 0;

				virtual float GetFloat(const uint32_t &nIndex) const = 0;

				virtual double GetDouble(const uint32_t &nIndex) const = 0;

				virtual void GetBytes(const uint32_t &nIndex, char *pBuffer, const uint32_t &nLength) const = 0;
				virtual void GetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength) const = 0;

				virtual void GetString(const uint32_t &nIndex, char *pBuffer, const uint32_t &nSize) const = 0;
				virtual void GetString(const uint32_t &nIndex, IBuffer *pBuffer) const = 0;

				virtual void WriteChar(const char &value) = 0;
				virtual void WriteUnsignedChar(const unsigned char &value) = 0;

				virtual void WriteInteger16(const int16_t &value) = 0;
				virtual void WriteUnsignedInteger16(const uint16_t &value) = 0;

				virtual void WriteInteger32(const int32_t &value) = 0;
				virtual void WriteUnsignedInteger32(const uint32_t &value) = 0;

				virtual void WriteInteger64(const int64_t &value) = 0;
				virtual void WriteUnsignedInteger64(const uint64_t &value) = 0;

				virtual void WriteFloat(const float &value) = 0;

				virtual void WriteDouble(const double &value) = 0;

				virtual void WriteBytes(const char *pBuffer, const uint32_t &nLength) = 0;
				virtual void WriteBytes(IBuffer *pBuffer, const uint32_t &nLength) = 0;

				virtual void WriteString(const char *pBuffer) = 0;
				virtual void WriteString(IBuffer *pBuffer) = 0;

				virtual void WriteZero(const uint32_t &nLength) = 0;

				virtual void SetChar(const uint32_t &nIndex, const char &value) = 0;
				virtual void SetUnsignedChar(const uint32_t &nIndex, const unsigned char &value) = 0;

				virtual void SetInteger16(const uint32_t &nIndex, const int16_t &value) = 0;
				virtual void SetUnsignedInteger16(const uint32_t &nIndex, const uint16_t &value) = 0;

				virtual void SetInteger32(const uint32_t &nIndex, const int32_t &value) = 0;
				virtual void SetUnsignedInteger32(const uint32_t &nIndex, const uint32_t &value) = 0;

				virtual void SetInteger64(const uint32_t &nIndex, const int64_t &value) = 0;
				virtual void SetUnsignedInteger64(const uint32_t &nIndex, const uint64_t &value) = 0;

				virtual void SetFloat(const uint32_t &nIndex, const float &value) = 0;

				virtual void SetDouble(const uint32_t &nIndex, const double &value) = 0;

				virtual void SetBytes(const uint32_t &nIndex, const char *pBuffer, const uint32_t &nLength) = 0;
				virtual void SetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength) = 0;

				virtual void SetString(const uint32_t &nIndex, const char *pBuffer) = 0;
				virtual void SetString(const uint32_t &nIndex, IBuffer *pBuffer) = 0;

				virtual void SetZero(const uint32_t &nIndex, const uint32_t &nLength) = 0;
				virtual void SetZero() = 0;

				virtual uint32_t Skip(const uint32_t &nLength) = 0;
				virtual uint32_t SkipTo(const char &target) = 0;
				virtual uint32_t SkipAll() = 0;
			};
		}
	}
}

#endif