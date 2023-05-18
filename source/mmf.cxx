#include "include/mmf.hxx"

#ifdef _WIN32
#define OS_DEP(UNIX_EXPRESSION, WINDOWS_EXPRESSION) WINDOWS_EXPRESSION
#include <windows.h>
#include <io.h>
#define F_OK 0
#define access _access

#else
#define OS_DEP(UNIX_EXPRESSION, WINDOWS_EXPRESSION) UNIX_EXPRESSION
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#endif

using namespace pmmf;

inline int checkProtoFlag(ProtectionMode protection_mode, ProtectionMode flag) { return (int)protection_mode & (int)flag; }

size_t MappedFile::getSystemPageSize() {
#ifdef _WIN32
  SYSTEM_INFO sysinfo = {0};
  ::GetSystemInfo(&sysinfo);
  return sysinfo.dwAllocationGranularity;
#else
  return sysconf(_SC_PAGESIZE);
#endif
}

FileDescriptor MappedFile::openFile(std::string file_path, ProtectionMode protection_mode) {
  if(access(file_path.c_str(), F_OK) == 0) {
#ifdef _WIN32
    return ::CreateFile(
          file_path.c_str(),
            ((DWORD)0x00)
            | ((checkProtoFlag(protection_mode, ProtectionMode::read) || checkProtoFlag(protection_mode, ProtectionMode::exec)) ? GENERIC_READ : ((DWORD)0x00))
            | (checkProtoFlag(protection_mode, ProtectionMode::write) ? GENERIC_WRITE : ((DWORD)0x00))
            | (checkProtoFlag(protection_mode, ProtectionMode::exec) ? GENERIC_EXECUTE : ((DWORD)0x00)),
            checkProtoFlag(protection_mode, ProtectionMode::write) ? 0x00 : FILE_SHARE_READ,
            0,
            checkProtoFlag(protection_mode, ProtectionMode::trunc) ? TRUNCATE_EXISTING : OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0
          );
#else
    return ::open(
          file_path.c_str(),
          ((int)0x00
          | (checkProtoFlag(protection_mode, ProtectionMode::trunc) ? O_TRUNC : 0x00)
          | (checkProtoFlag(protection_mode, ProtectionMode::read) && checkProtoFlag(protection_mode, ProtectionMode::write)) ? O_RDWR
          : checkProtoFlag(protection_mode, ProtectionMode::read) ? O_RDONLY
            : checkProtoFlag(protection_mode, ProtectionMode::write)
              ? O_WRONLY
              : 0x00),
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
          );
#endif
  } else {
#ifdef _WIN32
    return ::CreateFile(
          file_path.c_str(),
          ((DWORD)0x00)
          | ((checkProtoFlag(protection_mode, ProtectionMode::read) || checkProtoFlag(protection_mode, ProtectionMode::exec)) ? GENERIC_READ : 0x00)
          | (checkProtoFlag(protection_mode, ProtectionMode::write) ? GENERIC_WRITE : 0x00),
          (checkProtoFlag(protection_mode, ProtectionMode::write) ? 0x00 : FILE_SHARE_READ),
          0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
#else
    return ::open(
          file_path.c_str(),
          ((int)O_CREAT
          | (checkProtoFlag(protection_mode, ProtectionMode::trunc) ? O_TRUNC : 0x00)
          | (checkProtoFlag(protection_mode, ProtectionMode::read) && checkProtoFlag(protection_mode, ProtectionMode::write)) ? O_RDWR
          : checkProtoFlag(protection_mode, ProtectionMode::read) ? O_RDONLY
            : checkProtoFlag(protection_mode, ProtectionMode::write)
              ? O_WRONLY
              : 0x00),
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
          );
#endif
  }
}

MappedFile::MappedFile(std::string file_path, ProtectionMode protection_mode, MapFlag map_flag)
  : file_descriptor(openFile(std::move(file_path), protection_mode)), proterction_mode(protection_mode), map_flag(map_flag) {
  if(!isFileOpen()) {
    protection_mode = ProtectionMode(0x00);
    map_flag = MapFlag(0x00);
  }
}

size_t MappedFile::getFileSize() const {
#ifdef _WIN32
  DWORD high_size;
  DWORD low_size = ::GetFileSize(file_descriptor, &high_size);
  return (static_cast<std::size_t>(high_size) << 32) | low_size;
#else
  struct stat f_stat_buf;
  if(::fstat(file_descriptor, &f_stat_buf)) return 0;
  return f_stat_buf.st_size;
#endif
}
