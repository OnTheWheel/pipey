#ifndef PIPEY_FIXEDBUFFER_H
#define PIPEY_FIXEDBUFFER_H

#include "Buffer.h"

namespace pipey {
	namespace memory {
		namespace buffer {

			class CFixedBuffer : public IBuffer
			{
			public:
				CFixedBuffer(const uint32_t &nLength, const ::pipey::util::BYTE_ENDIAN &endian, ::pipey::memory::pool::IMemoryPool *pMemoryPool);
				CFixedBuffer(const uint32_t &nLength, ::pipey::memory::pool::IMemoryPool *pMemoryPool);
				virtual ~CFixedBuffer();

			protected:
				CFixedBuffer(const IBuffer *pParentBuffer, const uint32_t &nIndex, const uint32_t &nLength);

			private:
				::pipey::memory::pool::CMemoryHandle m_hMemory;
				::pipey::memory::pool::IMemoryPool *m_pMemoryPool;
				char *m_pBuffer;
				::pipey::util::BYTE_ENDIAN m_endian;
				uint32_t m_nCapacity;
				uint32_t m_nReadIndex;
				uint32_t m_nWriteIndex;
				uint32_t m_nMarkedReadIndex;
				uint32_t m_nMarkedWriteIndex;
				SCATTERED_BUFFER m_scatteredBuffer;
				IO_BUFFER m_ioBuffer;

			public:

				virtual uint32_t GetCapacity() const;
				virtual uint32_t GetReadIndex() const;
				virtual uint32_t GetWriteIndex() const;
				virtual uint32_t GetReadableLength() const;

				virtual IO_BUFFER *GetIOBuffer();

				virtual const ::pipey::memory::pool::CMemoryHandle &GetMemoryHandle() const;
				virtual ::pipey::memory::pool::IMemoryPool *GetMemoryPool() const;


				virtual uint32_t SetReadIndex(const uint32_t &nIndex);
				virtual uint32_t SetWriteIndex(const uint32_t &nIndex);

				virtual bool IsReadable() const;
				virtual bool IsWritable() const;

				virtual ::pipey::util::BYTE_ENDIAN GetEndian() const;

				virtual bool EnsureWritable(const uint32_t &nLength);

				virtual uint32_t Find(const char &value) const;
				virtual uint32_t Find(const uint32_t &nStartIndex, const uint32_t &nLength, const char &value) const;

				virtual char ReadChar();
				virtual unsigned char ReadUnsignedChar();

				virtual int16_t ReadInteger16();
				virtual uint16_t ReadUnsignedInteger16();

				virtual int32_t ReadInteger32();
				virtual uint32_t ReadUnsignedInteger32();

				virtual int64_t ReadInteger64();
				virtual uint64_t ReadUnsignedInteger64();

				virtual float ReadFloat();

				virtual double ReadDouble();

				virtual void ReadBytes(char *pBuffer, const uint32_t &nLength);
				virtual void ReadBytes(IBuffer *pBuffer, const uint32_t &nLength);

				virtual void ReadString(char *pBuffer, const uint32_t &nSize);
				virtual void ReadString(IBuffer *pBuffer);

				virtual char GetChar(const uint32_t &nIndex) const;
				virtual unsigned char GetUnsignedChar(const uint32_t &nIndex) const;

				virtual int16_t GetInteger16(const uint32_t &nIndex) const;
				virtual uint16_t GetUnsignedInteger16(const uint32_t &nIndex) const;

				virtual int32_t GetInteger32(const uint32_t &nIndex) const;
				virtual uint32_t GetUnsignedInteger32(const uint32_t &nIndex) const;

				virtual int64_t GetInteger64(const uint32_t &nIndex) const;
				virtual uint64_t GetUnsignedInteger64(const uint32_t &nIndex) const;

				virtual float GetFloat(const uint32_t &nIndex) const;

				virtual double GetDouble(const uint32_t &nIndex) const;

				virtual void GetBytes(const uint32_t &nIndex, char *pBuffer, const uint32_t &nLength) const;
				virtual void GetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength) const;

				virtual void GetString(const uint32_t &nIndex, char *pBuffer, const uint32_t &nSize) const;
				virtual void GetString(const uint32_t &nIndex, IBuffer *pBuffer) const;

				virtual void WriteChar(const char &value);
				virtual void WriteUnsignedChar(const unsigned char &value);

				virtual void WriteInteger16(const int16_t &value);
				virtual void WriteUnsignedInteger16(const uint16_t &value);

				virtual void WriteInteger32(const int32_t &value);
				virtual void WriteUnsignedInteger32(const uint32_t &value);

				virtual void WriteInteger64(const int64_t &value);
				virtual void WriteUnsignedInteger64(const uint64_t &value);

				virtual void WriteFloat(const float &value);

				virtual void WriteDouble(const double &value);

				virtual void WriteBytes(const char *pBuffer, const uint32_t &nLength);
				virtual void WriteBytes(IBuffer *pBuffer, const uint32_t &nLength);

				virtual void WriteString(const char *pBuffer);
				virtual void WriteString(IBuffer *pBuffer);

				virtual void WriteZero(const uint32_t &nLength);

				virtual void SetChar(const uint32_t &nIndex, const char &value);
				virtual void SetUnsignedChar(const uint32_t &nIndex, const unsigned char &value);

				virtual void SetInteger16(const uint32_t &nIndex, const int16_t &value);
				virtual void SetUnsignedInteger16(const uint32_t &nIndex, const uint16_t &value);

				virtual void SetInteger32(const uint32_t &nIndex, const int32_t &value);
				virtual void SetUnsignedInteger32(const uint32_t &nIndex, const uint32_t &value);

				virtual void SetInteger64(const uint32_t &nIndex, const int64_t &value);
				virtual void SetUnsignedInteger64(const uint32_t &nIndex, const uint64_t &value);

				virtual void SetFloat(const uint32_t &nIndex, const float &value);

				virtual void SetDouble(const uint32_t &nIndex, const double &value);

				virtual void SetBytes(const uint32_t &nIndex, const char *pBuffer, const uint32_t &nLength);
				virtual void SetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength);

				virtual void SetString(const uint32_t &nIndex, const char *pBuffer);
				virtual void SetString(const uint32_t &nIndex, IBuffer *pBuffer);

				virtual void SetZero(const uint32_t &nIndex, const uint32_t &nLength);
				virtual void SetZero();

				virtual uint32_t Skip(const uint32_t &nLength);
				virtual uint32_t SkipTo(const char &target);
				virtual uint32_t SkipAll();
			};
		}
	}
}

#endif