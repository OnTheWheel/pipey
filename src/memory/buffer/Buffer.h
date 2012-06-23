#ifndef PIPEY_BUFFER_H
#define PIPEY_BUFFER_H

#include "../../common/common.h"
#include "../../util/ByteOrder.h"
#include "../pool/MemoryPool.h"

namespace pipey {
	namespace memory {
		namespace buffer {

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

				virtual short ReadShort() = 0;
				virtual unsigned short ReadUnsignedShort() = 0;

				virtual int32_t ReadInt() = 0;
				virtual uint32_t ReadUnsignedInt() = 0;

				virtual int32_t ReadLong() = 0;
				virtual uint32_t ReadUnsignedLong() = 0;

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

				virtual short GetShort(const uint32_t &nIndex) const = 0;
				virtual unsigned short GetUnsignedShort(const uint32_t &nIndex) const = 0;

				virtual int32_t GetInt(const uint32_t &nIndex) const = 0;
				virtual uint32_t GetUnsignedInt(const uint32_t &nIndex) const = 0;

				virtual int32_t GetLong(const uint32_t &nIndex) const = 0;
				virtual uint32_t GetUnsignedLong(const uint32_t &nIndex) const = 0;

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

				virtual void WriteShort(const short &value) = 0;
				virtual void WriteUnsignedShort(const unsigned short &value) = 0;

				virtual void WriteInt(const int32_t &value) = 0;
				virtual void WriteUnsignedInt(const uint32_t &value) = 0;

				virtual void WriteLong(const int32_t &value) = 0;
				virtual void WriteUnsignedLong(const uint32_t &value) = 0;

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

				virtual void SetShort(const uint32_t &nIndex, const short &value) = 0;
				virtual void SetUnsignedShort(const uint32_t &nIndex, const unsigned short &value) = 0;

				virtual void SetInt(const uint32_t &nIndex, const int32_t &value) = 0;
				virtual void SetUnsignedInt(const uint32_t &nIndex, const uint32_t &value) = 0;

				virtual void SetLong(const uint32_t &nIndex, const int32_t &value) = 0;
				virtual void SetUnsignedLong(const uint32_t &nIndex, const uint32_t &value) = 0;

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