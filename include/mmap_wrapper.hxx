#ifndef WINMMAP_H
#define WINMMAP_H

#include <cstddef>
#include <inttypes.h>

//! Portable Memory-Mapped File

#ifdef _WIN32

#include <windows.h>

namespace pmmf {
/**
 * @brief FileDescriptor - alias for native handle of file
 */
typedef HANDLE FileDescriptor;
const FileDescriptor INVALID_FILE_DESCRIPTOR = INVALID_HANDLE_VALUE;

#ifdef _WIN64
typedef int64_t off_t;
#else
typedef uint32_t off_t;
#endif

#else

#include <aio.h>

namespace pmmf {
/**
 * @brief FileDescriptor - alias for native handle of file
 */
typedef int FileDescriptor;
constexpr FileDescriptor INVALID_FILE_DESCRIPTOR = -1;
#endif

/**
 * @brief The ProtectionMode enum - describes the desired memory protection mode (it must not conflict with the file open mode)
 */
enum class ProtectionMode : int {
  read        = 0x01, //< data can be read
  write       = 0x02, //< data can be write

//#ifdef FILE_MAP_EXECUTE
  exec        = 0x04, //< data can be executed
  rx          = 0x01 | 0x04,
  wx          = 0x02 | 0x04,
  rwx         = 0x01 | 0x02 | 0x04,
  rxt         = 0x01 | 0x04 | 0x08,
  wxt         = 0x02 | 0x04 | 0x08,
  rwxt        = 0x01 | 0x02 | 0x04 | 0x08,

//#else
//  exec        = 0x00, //< data can be executed (FLAG IS DISABLED)
//  rx          = 0x01 | 0x00,
//  wx          = 0x02 | 0x00,
//  rwx         = 0x01 | 0x02 | 0x00,
//  rxt         = 0x01 | 0x00 | 0x08,
//  wxt         = 0x02 | 0x00 | 0x08,
//  rwxt        = 0x01 | 0x02 | 0x00 | 0x08,

//#endif
  trunc       = 0x08, //< clear file on open
  rw          = 0x01 | 0x02,
  rt          = 0x01 | 0x08,
  wt          = 0x02 | 0x08,
  rwt         = 0x01 | 0x02 | 0x08

};

/**
 * @brief The MapFlag enum - The flags argument provides other information about the handling of the mapped data. The value of flags is the bitwise inclusive OR of these options
 */
enum class MapFlag : int {
  shared      = 0x0001,
  priv        = 0x0002,
  anonymous   = 0x0004,
  anon        = anonymous,
  fixed       = 0x0008,
#ifndef _WIN32 // Not implemented in windows
  denywrite   = 0x0010,
  executable  = 0x0020,
  noreserve   = 0x0040,
  locked      = 0x0080,
  growsdown   = 0x0100,
  file        = 0x0200
#endif
};

#ifndef MAP_FAILED
#define MAP_FAILED reinterpret_cast<void*>(0xFFFFFFFFFFFFFFFFull)
#endif


/**
 * @brief mmap - portable analogue of Unix mmap, see: https://www.opennet.ru/man.shtml?topic=mmap&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
 */
extern void* mmap(void* start, size_t length, ProtectionMode prot, MapFlag flags, FileDescriptor fd, off_t offset);

/**
 * @brief munmap - portable analogue of Unix munmap, see: https://www.opennet.ru/man.shtml?topic=mmap&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
 */
extern void munmap(void *addr, std::size_t length);

/**
 * @brief msync - synchronize a file with a memory map: https://www.opennet.ru/man.shtml?topic=msync&russian=2&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
 */
extern int msync(FileDescriptor file_descriptor, void *start, size_t length, int flags);

}

#endif // WINMMAP_H
