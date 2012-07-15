#ifndef PIPEY_SCATTERED_BUFFER_H
#define PIPEY_SCATTERED_BUFFER_H

namespace pipey {
	namespace memory {
		namespace buffer {

#if defined(WIN32) || defined(WIN64)

			struct SCATTERED_BUFFER : public WSABUF {

				size_t GetSize() {
					return len;
				}

				void SetSize(size_t size) {
					len = size;
				}

				void *GetBuffer() {
					return buf;
				}

				void SetBuffer(void *buffer) {
					buf = (char *)buffer;
				}
			};

#elif defined(__linux__) || defined(__unix__)
			struct SCATTERED_BUFFER : public iovec {

				size_t GetSize() {
					return iov_len;
				}

				void SetSize(size_t size) {
					iov_len = size;
				}

				void *GetBuffer() {
					return iov_base;
				}

				void SetBuffer(void *buffer) {
					iov_base = buffer;
				}
			};
#endif
		}
	}
}

#endif