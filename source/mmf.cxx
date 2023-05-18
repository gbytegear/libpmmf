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


//using namespace pmmf;

//inline int checkProtoFlag(ProtectionMode proto, ProtectionMode flag) { return (int)proto & (int)flag; }

//FileDescriptor MemoryMappedFile::openFile(std::string file_path, ProtectionMode prot) {
//  if(access(file_path.c_str(), F_OK) == 0) {
//#ifdef _WIN32
//    return ::CreateFile(
//          file_path.c_str(),
//            ((DWORD)0x00)
//            | ((checkProtoFlag(prot, ProtectionMode::read) || checkProtoFlag(prot, ProtectionMode::exec)) ? GENERIC_READ : ((DWORD)0x00))
//            | (checkProtoFlag(prot, ProtectionMode::write) ? GENERIC_WRITE : ((DWORD)0x00))
//            | (checkProtoFlag(prot, ProtectionMode::exec) ? GENERIC_EXECUTE : ((DWORD)0x00)),
//            checkProtoFlag(prot, ProtectionMode::write) ? 0x00 : FILE_SHARE_READ,
//            0,
//            checkProtoFlag(prot, ProtectionMode::trunc) ? TRUNCATE_EXISTING : OPEN_EXISTING,
//            FILE_ATTRIBUTE_NORMAL,
//            0
//          );
//#else
//    return ::open(
//          file_path.c_str(),
//          ((int)0x00
//          | (checkProtoFlag(prot, ProtectionMode::trunc) ? O_TRUNC : 0x00)
//          | (checkProtoFlag(prot, ProtectionMode::read) && checkProtoFlag(prot, ProtectionMode::write)) ? O_RDWR
//          : checkProtoFlag(prot, ProtectionMode::read) ? O_RDONLY
//            : checkProtoFlag(prot, ProtectionMode::write)
//              ? O_WRONLY
//              : 0x00),
//          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
//          );
//#endif
//  } else {
//#ifdef _WIN32
//    return ::CreateFile(
//          file_path.c_str(),
//          ((DWORD)0x00)
//          | ((checkProtoFlag(prot, ProtectionMode::read) || checkProtoFlag(prot, ProtectionMode::exec)) ? GENERIC_READ : 0x00)
//          | (checkProtoFlag(prot, ProtectionMode::write) ? GENERIC_WRITE : 0x00),
//          (checkProtoFlag(prot, ProtectionMode::write) ? 0x00 : FILE_SHARE_READ),
//          0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
//#else
//    return ::open(
//          file_path.c_str(),
//          ((int)O_CREAT
//          | (checkProtoFlag(prot, ProtectionMode::trunc) ? O_TRUNC : 0x00)
//          | (checkProtoFlag(prot, ProtectionMode::read) && checkProtoFlag(prot, ProtectionMode::write)) ? O_RDWR
//          : checkProtoFlag(prot, ProtectionMode::read) ? O_RDONLY
//            : checkProtoFlag(prot, ProtectionMode::write)
//              ? O_WRONLY
//              : 0x00),
//          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
//          );
//#endif
//  }
//}

//MemoryMappedFile::MemoryMappedFile(MemoryMappedFile&& other)
//  : file_descriptor(other.file_descriptor),
//    mapped_areas(std::move(other.mapped_areas)),
//    prot(other.prot),
//    map_flags(other.map_flags) {
//  other.file_descriptor = INVALID_FILE_DESCRIPTOR;
//}

//MemoryMappedFile::MemoryMappedFile(std::string file_path,
//                                   std::size_t offset,
//                                   std::size_t map_size,
//                                   ProtectionMode prot,
//                                   MapFlag flags)
//  : file_descriptor(openFile(std::move(file_path), prot)), prot(prot), map_flags(flags) {
//  if(isFileOpen()) {
//    if(void* mapped_area = mmapPortable(nullptr, (map_size = !map_size ? getFileSize() : map_size), prot, flags, file_descriptor, offset); mapped_area != MAP_FAILED)
//      mapped_areas.emplace(file_descriptor, mapped_area, offset, map_size);
//  }
//}

//MemoryMappedFile::MemoryMappedFile(std::string file_path, ProtectionMode prot, MapFlag flags)
//  : file_descriptor(openFile(std::move(file_path), prot)), prot(prot), map_flags(flags) {
//  if(isFileOpen()) {
//    std::size_t map_size = getFileSize();
//    if(void* mapped_area = mmapPortable(nullptr, map_size, prot, flags, file_descriptor, 0); mapped_area != MAP_FAILED)
//      mapped_areas.emplace(mapped_area, mapped_area, 0, map_size);
//  }
//}


//MemoryMappedFile::~MemoryMappedFile() {
//  flush();
//  mapped_areas.clear();
//  if(file_descriptor != INVALID_FILE_DESCRIPTOR)
//#ifdef _WIN32
//    ::CloseHandle(file_descriptor);
//#else
//    ::close(file_descriptor);
//#endif
//  file_descriptor = INVALID_FILE_DESCRIPTOR;
//}

//std::size_t MemoryMappedFile::getFileSize() const {
//  #ifdef _WIN32
//    DWORD high_size;
//    DWORD low_size = ::GetFileSize(file_descriptor, &high_size);
//    return (static_cast<std::size_t>(high_size) << 32) | low_size;
//  #else
//    struct stat f_stat_buf;
//    if(::fstat(file_descriptor, &f_stat_buf)) return 0;
//    return f_stat_buf.st_size;
//#endif
//}

//MemoryMappedFileArea* MemoryMappedFile::addMappedArea(size_t offset, size_t size) {
//  if(auto mapped_area = getMappedArea(offset); mapped_area) {
//    if(mapped_area->getMappedSize() == size) return mapped_area; else return nullptr;
//  }
//  if(auto mapped_area = getMappedArea(offset + size - 1); mapped_area) return nullptr;

//  if(isFileOpen()) {
//    if(void* mapped_area = mmapPortable(nullptr, (size = !size ? getFileSize() - offset : size), prot, map_flags, file_descriptor, offset); mapped_area != MAP_FAILED) {
//      return &const_cast<MemoryMappedFileArea&>(*mapped_areas.emplace(file_descriptor, mapped_area, offset, size).first);
//    }
//  }
//  return nullptr;
//}

//size_t MemoryMappedFile::flush() {
//  size_t count = 0;
//  for(const auto& mapped_area : mapped_areas)
//    if(const_cast<MemoryMappedFileArea&>(mapped_area).flush()) ++count;
//  return count;
//}

//MemoryMappedFileArea* MemoryMappedFile::getMappedArea(std::size_t offset) {
//  auto it = mapped_areas.lower_bound(offset);
//  if(it->getOffset() == offset) return &const_cast<MemoryMappedFileArea&>(*it);
//  if(it->getOffset() > offset) {
//    if(it == mapped_areas.begin() || it == mapped_areas.end()) return nullptr;
//    if(--it; it->getOffset() + it->getMappedSize() > offset) return &const_cast<MemoryMappedFileArea&>(*it);
//    return nullptr;
//  } else return nullptr;
//}


//MemoryMappedFileArea::~MemoryMappedFileArea() {
//  if(memory_page_start)
//    munmapPortable(memory_page_start, size);
//  memory_page_start = nullptr;
//  offset = 0;
//  size = 0;
//}

//bool MemoryMappedFileArea::flush() const {
//  if(isMapped()) {
//#ifdef _WIN32
//    return
//        ::FlushViewOfFile(memory_page_start, 0) &&
//        ::FlushFileBuffers(file_descriptor);
//  } else return !::FlushFileBuffers(file_descriptor);
//#else
//    return ::msync(memory_page_start, mapped_size, MS_SYNC) == 0;
//  } else return false;
//#endif
//}

inline int checkProtoFlag(ProtectionMode protection_mode, ProtectionMode flag) { return (int)protection_mode & (int)flag; }

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
