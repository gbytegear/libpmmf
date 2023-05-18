#include "include/mmap_wrapper.hxx"

#define DEBUG


// WIN32 =============================================================================================
#ifdef _WIN32

// UNIX =============================================================================================
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

using namespace pmmf;

extern void* pmmf::mmap(void* start, size_t length, ProtectionMode prot, MapFlag flags, FileDescriptor fd, off_t offset) {
#ifndef _WIN32
  int __prot = ( ( (int)0x00 )
               | ( ( (int)prot & (int)ProtectionMode::read ) ? PROT_READ : 0x00 )
               | ( ( (int)prot & (int)ProtectionMode::write ) ? PROT_WRITE : 0x00 )
               | ( ( (int)prot & (int)ProtectionMode::exec ) ? PROT_EXEC : 0x00 ) );

  int __flags = ( ( (int)0x00 )
                | ( ( (int)flags & (int)MapFlag::anonymous ) ? MAP_ANONYMOUS : 0x00 )
                | ( ( (int)flags & (int)MapFlag::priv ) ? MAP_PRIVATE : 0x00 )
                | ( ( (int)flags & (int)MapFlag::shared ) ? MAP_SHARED : 0x00 )
                | ( ( (int)flags & (int)MapFlag::denywrite ) ? MAP_DENYWRITE : 0x00 )
                | ( ( (int)flags & (int)MapFlag::executable ) ? MAP_EXECUTABLE : 0x00 )
                | ( ( (int)flags & (int)MapFlag::noreserve ) ? MAP_NORESERVE : 0x00 )
                | ( ( (int)flags & (int)MapFlag::locked ) ? MAP_LOCKED : 0x00 )
                | ( ( (int)flags & (int)MapFlag::growsdown ) ? MAP_GROWSDOWN : 0x00 )
                | ( ( (int)flags & (int)MapFlag::file ) ? MAP_FILE : 0x00 ) );

  return ::mmap(start, length, __prot, __flags, fd, static_cast<off_t>(offset));

#else
  if(fd == INVALID_FILE_DESCRIPTOR && (!((int)flags & (int)MapFlag::anon) || offset))
      return MAP_FAILED;

  DWORD protect =
    ((int)prot & (int)ProtectionMode::write)
    ? ((int)prot & (int)ProtectionMode::exec)
      ? PAGE_EXECUTE_READWRITE
      : PAGE_READWRITE
    : ((int)prot & (int)ProtectionMode::exec)
    ? ((int)prot & (int)ProtectionMode::read)
      ? PAGE_EXECUTE_READ
      : PAGE_EXECUTE
    : ((int)prot & (int)ProtectionMode::read)
      ? PAGE_READONLY
      : PAGE_NOACCESS;

  HANDLE map_handle;
  if constexpr (sizeof(off_t) <= sizeof(DWORD)) {
    map_handle = CreateFileMapping(fd, nullptr, protect, 0, static_cast<DWORD>(offset + static_cast<off_t>(length)), nullptr);
  } else {
    const off_t max_size = offset + static_cast<off_t>(length);
    map_handle = CreateFileMapping(fd, nullptr, protect, static_cast<DWORD>((max_size >> 32) & 0xFFFFFFFFL), static_cast<DWORD>(max_size & 0xFFFFFFFFL), nullptr);
  }
  if(map_handle == nullptr)
    return MAP_FAILED;

  DWORD dwDesiredAccess = 0
    | (((int)prot & (int)ProtectionMode::write) ? FILE_MAP_WRITE : 0)
    | (((int)prot & (int)ProtectionMode::read) ? FILE_MAP_READ : 0)
    | (((int)prot & (int)ProtectionMode::exec) ? FILE_MAP_EXECUTE : 0)
    | (((int)flags & (int)MapFlag::priv) ? FILE_MAP_COPY : 0);

  void* start_page_pointer;
  if((int)flags & (int)MapFlag::fixed) {
    if constexpr (sizeof(off_t) <= sizeof(DWORD)) {
      start_page_pointer = MapViewOfFileEx(map_handle, dwDesiredAccess, 0, static_cast<DWORD>(offset), length, start);
    } else {
      start_page_pointer = MapViewOfFileEx(map_handle, dwDesiredAccess, static_cast<DWORD>((offset >> 32) & 0xFFFFFFFFL), static_cast<DWORD>(offset & 0xFFFFFFFFL), length, start);
    }
  } else {
    if constexpr (sizeof(off_t) <= sizeof(DWORD)) {
      start_page_pointer = MapViewOfFile(map_handle, dwDesiredAccess, 0, static_cast<DWORD>(offset), length);
    } else {
      start_page_pointer = MapViewOfFile(map_handle, dwDesiredAccess, static_cast<DWORD>((offset >> 32) & 0xFFFFFFFFL), static_cast<DWORD>(offset & 0xFFFFFFFFL), length);
    }
  }

  if(start_page_pointer == nullptr) {
    CloseHandle(map_handle);
    return MAP_FAILED;
  }

  CloseHandle(map_handle);

  return start_page_pointer;
#endif
}



extern void pmmf::munmap(void *addr, std::size_t length) {
#ifndef _WIN32
  ::munmap(addr, length);
#else
  UnmapViewOfFile(addr);
#endif
}

extern int pmmf::msync(FileDescriptor file_descriptor, void* start, size_t length, int flags) {
#ifdef _WIN32
  return (::FlushViewOfFile(start, 0))
         ? (::FlushFileBuffers(file_descriptor))
           ? 0
           : -1
         : -1;
#else
  return ::msync(start, length, MS_SYNC);
#endif
}
