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

				virtual unsigned long GetCapacity() const = 0;
				virtual unsigned long GetReadIndex() const = 0;
				virtual unsigned long GetWriteIndex() const = 0;
				virtual unsigned long GetReadableLength() const = 0;

				virtual const ::pipey::memory::pool::CMemoryHandle &GetMemoryHandle() const = 0;
				virtual ::pipey::memory::pool::IMemoryPool *GetMemoryPool() const = 0;

				virtual unsigned long SetReadIndex(const unsigned long &nIndex) = 0;
				virtual unsigned long SetWriteIndex(const unsigned long &nIndex) = 0;

				virtual bool IsReadable() const = 0;
				virtual bool IsWritable() const = 0;

				virtual ::pipey::util::BYTE_ENDIAN GetEndian() const = 0;

				virtual bool EnsureWritable(const unsigned long &nLength) = 0;

				virtual unsigned long Find(const char &value) const = 0;
				virtual unsigned long Find(const unsigned long &nStartIndex, const unsigned long &nLength, const char &value) const = 0;

				virtual char ReadChar() = 0;
				virtual unsigned char ReadUnsignedChar() = 0;

				virtual short ReadShort() = 0;
				virtual unsigned short ReadUnsignedShort() = 0;

				virtual int ReadInt() = 0;
				virtual unsigned int ReadUnsignedInt() = 0;

				virtual long ReadLong() = 0;
				virtual unsigned long ReadUnsignedLong() = 0;

				virtual ::pipey::common::_integer64 ReadInteger64() = 0;
				virtual ::pipey::common::_uinteger64 ReadUnsignedInteger64() = 0;

				virtual float ReadFloat() = 0;

				virtual double ReadDouble() = 0;

				virtual void ReadBytes(char *pBuffer, const unsigned long &nLength) = 0;
				virtual void ReadBytes(IBuffer *pBuffer, const unsigned long &nLength) = 0;

				virtual void ReadString(char *pBuffer, const unsigned long &nSize) = 0;
				virtual void ReadString(IBuffer *pBuffer) = 0;

				virtual char GetChar(const unsigned long &nIndex) const = 0;
				virtual unsigned char GetUnsignedChar(const unsigned long &nIndex) const = 0;

				virtual short GetShort(const unsigned long &nIndex) const = 0;
				virtual unsigned short GetUnsignedShort(const unsigned long &nIndex) const = 0;

				virtual int GetInt(const unsigned long &nIndex) const = 0;
				virtual unsigned int GetUnsignedInt(const unsigned long &nIndex) const = 0;

				virtual long GetLong(const unsigned long &nIndex) const = 0;
				virtual unsigned long GetUnsignedLong(const unsigned long &nIndex) const = 0;

				virtual ::pipey::common::_integer64 GetInteger64(const unsigned long &nIndex) const = 0;
				virtual ::pipey::common::_uinteger64 GetUnsignedInteger64(const unsigned long &nIndex) const = 0;

				virtual float GetFloat(const unsigned long &nIndex) const = 0;

				virtual double GetDouble(const unsigned long &nIndex) const = 0;

				virtual void GetBytes(const unsigned long &nIndex, char *pBuffer, const unsigned long &nLength) const = 0;
				virtual void GetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength) const = 0;

				virtual void GetString(const unsigned long &nIndex, char *pBuffer, const unsigned long &nSize) const = 0;
				virtual void GetString(const unsigned long &nIndex, IBuffer *pBuffer) const = 0;

				virtual void WriteChar(const char &value) = 0;
				virtual void WriteUnsignedChar(const unsigned char &value) = 0;

				virtual void WriteShort(const short &value) = 0;
				virtual void WriteUnsignedShort(const unsigned short &value) = 0;

				virtual void WriteInt(const int &value) = 0;
				virtual void WriteUnsignedInt(const unsigned int &value) = 0;

				virtual void WriteLong(const long &value) = 0;
				virtual void WriteUnsignedLong(const unsigned long &value) = 0;

				virtual void WriteInteger64(const ::pipey::common::_integer64 &value) = 0;
				virtual void WriteUnsignedInteger64(const ::pipey::common::_uinteger64 &value) = 0;

				virtual void WriteFloat(const float &value) = 0;

				virtual void WriteDouble(const double &value) = 0;

				virtual void WriteBytes(const char *pBuffer, const unsigned long &nLength) = 0;
				virtual void WriteBytes(IBuffer *pBuffer, const unsigned long &nLength) = 0;

				virtual void WriteString(const char *pBuffer) = 0;
				virtual void WriteString(IBuffer *pBuffer) = 0;

				virtual void WriteZero(const unsigned long &nLength) = 0;

				virtual void SetChar(const unsigned long &nIndex, const char &value) = 0;
				virtual void SetUnsignedChar(const unsigned long &nIndex, const unsigned char &value) = 0;

				virtual void SetShort(const unsigned long &nIndex, const short &value) = 0;
				virtual void SetUnsignedShort(const unsigned long &nIndex, const unsigned short &value) = 0;

				virtual void SetInt(const unsigned long &nIndex, const int &value) = 0;
				virtual void SetUnsignedInt(const unsigned long &nIndex, const unsigned int &value) = 0;

				virtual void SetLong(const unsigned long &nIndex, const long &value) = 0;
				virtual void SetUnsignedLong(const unsigned long &nIndex, const unsigned long &value) = 0;

				virtual void SetInteger64(const unsigned long &nIndex, const ::pipey::common::_integer64 &value) = 0;
				virtual void SetUnsignedInteger64(const unsigned long &nIndex, const ::pipey::common::_uinteger64 &value) = 0;

				virtual void SetFloat(const unsigned long &nIndex, const float &value) = 0;

				virtual void SetDouble(const unsigned long &nIndex, const double &value) = 0;

				virtual void SetBytes(const unsigned long &nIndex, const char *pBuffer, const unsigned long &nLength) = 0;
				virtual void SetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength) = 0;

				virtual void SetString(const unsigned long &nIndex, const char *pBuffer) = 0;
				virtual void SetString(const unsigned long &nIndex, IBuffer *pBuffer) = 0;

				virtual void SetZero(const unsigned long &nIndex, const unsigned long &nLength) = 0;
				virtual void SetZero() = 0;

				virtual unsigned long Skip(const unsigned long &nLength) = 0;
				virtual unsigned long SkipTo(const char &target) = 0;
				virtual unsigned long SkipAll() = 0;
			};
		}
	}
}

#endif