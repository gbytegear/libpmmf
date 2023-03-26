#ifndef WINMMAP_H
#define WINMMAP_H

#include <cstddef>

#ifdef _WIN32
#include <windows.h>

namespace pmmf {

typedef HANDLE FileDescriptor;
const FileDescriptor INVALID_FILE_DESCRIPTOR = INVALID_HANDLE_VALUE;
#else

namespace pmmf {

typedef int FileDescriptor;
const FileDescriptor INVALID_FILE_DESCRIPTOR = -1;
#endif

enum class Protocol : int {
  read        = 0x01,
  write       = 0x02,
#ifdef FILE_MAP_EXECUTE
  exec        = 0x04,
#else
  exec        = 0x00,
#endif
  open_trunc  = 0x08
};

enum class MapFlag : int {
  shared      = 0x01,
  priv        = 0x02,
  anonymous   = 0x04,
  anon        = anonymous,
#ifndef _WIN32 // Not implemented in windows
  denywrite   = 0x08,
  executable  = 0x10,
  noreserve   = 0x20,
  locked      = 0x30,
  growsdown   = 0x80,
  file        = 0x100
#endif
};

extern void* mmapPortable(void* start, size_t length, Protocol prot, MapFlag flags, FileDescriptor fd, std::size_t offset);
extern void munmapPortable(void *addr, std::size_t length);

}

#endif // WINMMAP_H
