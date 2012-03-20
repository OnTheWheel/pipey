#ifndef PIPEY_FIXEDBUFFER_H
#define PIPEY_FIXEDBUFFER_H

#include "Buffer.h"

namespace pipey {
	namespace memory {
		namespace buffer {

			class CFixedBuffer : public IBuffer
			{
			public:
				CFixedBuffer(const unsigned long &nLength, const ::pipey::util::BYTE_ENDIAN &endian, ::pipey::memory::pool::IMemoryPool *pMemoryPool);
				CFixedBuffer(const unsigned long &nLength, ::pipey::memory::pool::IMemoryPool *pMemoryPool);
				virtual ~CFixedBuffer();

			protected:
				CFixedBuffer(const IBuffer *pParentBuffer, const unsigned long &nIndex, const unsigned long &nLength);

			private:
				::pipey::memory::pool::CMemoryHandle m_hMemory;
				::pipey::memory::pool::IMemoryPool *m_pMemoryPool;
				char *m_pBuffer;
				::pipey::util::BYTE_ENDIAN m_endian;
				unsigned long m_nCapacity;
				unsigned long m_nReadIndex;
				unsigned long m_nWriteIndex;
				unsigned long m_nMarkedReadIndex;
				unsigned long m_nMarkedWriteIndex;

			public:

				virtual unsigned long GetCapacity() const;
				virtual unsigned long GetReadIndex() const;
				virtual unsigned long GetWriteIndex() const;
				virtual unsigned long GetReadableLength() const;

				virtual const ::pipey::memory::pool::CMemoryHandle &GetMemoryHandle() const;
				virtual ::pipey::memory::pool::IMemoryPool *GetMemoryPool() const;


				virtual unsigned long SetReadIndex(const unsigned long &nIndex);
				virtual unsigned long SetWriteIndex(const unsigned long &nIndex);

				virtual bool IsReadable() const;
				virtual bool IsWritable() const;

				virtual ::pipey::util::BYTE_ENDIAN GetEndian() const;

				virtual bool EnsureWritable(const unsigned long &nLength);

				virtual unsigned long Find(const char &value) const;
				virtual unsigned long Find(const unsigned long &nStartIndex, const unsigned long &nLength, const char &value) const;

				virtual char ReadChar();
				virtual unsigned char ReadUnsignedChar();

				virtual short ReadShort();
				virtual unsigned short ReadUnsignedShort();

				virtual int ReadInt();
				virtual unsigned int ReadUnsignedInt();

				virtual long ReadLong();
				virtual unsigned long ReadUnsignedLong();

				virtual ::pipey::common::_integer64 ReadInteger64();
				virtual ::pipey::common::_uinteger64 ReadUnsignedInteger64();

				virtual float ReadFloat();

				virtual double ReadDouble();

				virtual void ReadBytes(char *pBuffer, const unsigned long &nLength);
				virtual void ReadBytes(IBuffer *pBuffer, const unsigned long &nLength);

				virtual void ReadString(char *pBuffer, const unsigned long &nSize);
				virtual void ReadString(IBuffer *pBuffer);

				virtual char GetChar(const unsigned long &nIndex) const;
				virtual unsigned char GetUnsignedChar(const unsigned long &nIndex) const;

				virtual short GetShort(const unsigned long &nIndex) const;
				virtual unsigned short GetUnsignedShort(const unsigned long &nIndex) const;

				virtual int GetInt(const unsigned long &nIndex) const;
				virtual unsigned int GetUnsignedInt(const unsigned long &nIndex) const;

				virtual long GetLong(const unsigned long &nIndex) const;
				virtual unsigned long GetUnsignedLong(const unsigned long &nIndex) const;

				virtual ::pipey::common::_integer64 GetInteger64(const unsigned long &nIndex) const;
				virtual ::pipey::common::_uinteger64 GetUnsignedInteger64(const unsigned long &nIndex) const;

				virtual float GetFloat(const unsigned long &nIndex) const;

				virtual double GetDouble(const unsigned long &nIndex) const;

				virtual void GetBytes(const unsigned long &nIndex, char *pBuffer, const unsigned long &nLength) const;
				virtual void GetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength) const;

				virtual void GetString(const unsigned long &nIndex, char *pBuffer, const unsigned long &nSize) const;
				virtual void GetString(const unsigned long &nIndex, IBuffer *pBuffer) const;

				virtual void WriteChar(const char &value);
				virtual void WriteUnsignedChar(const unsigned char &value);

				virtual void WriteShort(const short &value);
				virtual void WriteUnsignedShort(const unsigned short &value);

				virtual void WriteInt(const int &value);
				virtual void WriteUnsignedInt(const unsigned int &value);

				virtual void WriteLong(const long &value);
				virtual void WriteUnsignedLong(const unsigned long &value);

				virtual void WriteInteger64(const ::pipey::common::_integer64 &value);
				virtual void WriteUnsignedInteger64(const ::pipey::common::_uinteger64 &value);

				virtual void WriteFloat(const float &value);

				virtual void WriteDouble(const double &value);

				virtual void WriteBytes(const char *pBuffer, const unsigned long &nLength);
				virtual void WriteBytes(IBuffer *pBuffer, const unsigned long &nLength);

				virtual void WriteString(const char *pBuffer);
				virtual void WriteString(IBuffer *pBuffer);

				virtual void WriteZero(const unsigned long &nLength);

				virtual void SetChar(const unsigned long &nIndex, const char &value);
				virtual void SetUnsignedChar(const unsigned long &nIndex, const unsigned char &value);

				virtual void SetShort(const unsigned long &nIndex, const short &value);
				virtual void SetUnsignedShort(const unsigned long &nIndex, const unsigned short &value);

				virtual void SetInt(const unsigned long &nIndex, const int &value);
				virtual void SetUnsignedInt(const unsigned long &nIndex, const unsigned int &value);

				virtual void SetLong(const unsigned long &nIndex, const long &value);
				virtual void SetUnsignedLong(const unsigned long &nIndex, const unsigned long &value);

				virtual void SetInteger64(const unsigned long &nIndex, const ::pipey::common::_integer64 &value);
				virtual void SetUnsignedInteger64(const unsigned long &nIndex, const ::pipey::common::_uinteger64 &value);

				virtual void SetFloat(const unsigned long &nIndex, const float &value);

				virtual void SetDouble(const unsigned long &nIndex, const double &value);

				virtual void SetBytes(const unsigned long &nIndex, const char *pBuffer, const unsigned long &nLength);
				virtual void SetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength);

				virtual void SetString(const unsigned long &nIndex, const char *pBuffer);
				virtual void SetString(const unsigned long &nIndex, IBuffer *pBuffer);

				virtual void SetZero(const unsigned long &nIndex, const unsigned long &nLength);
				virtual void SetZero();

				virtual unsigned long Skip(const unsigned long &nLength);
				virtual unsigned long SkipTo(const char &target);
				virtual unsigned long SkipAll();
			};
		}
	}
}

#endif