#include "include/mmf.hxx"


#ifdef _WIN32
#define OS_DEP(UNIX_EXPRESSION, WINDOWS_EXPRESSION) WINDOWS_EXPRESSION
#include <windows.h>
#include <io.h>
#define F_OK 0
#define access _access

#ifdef DEBUG

#include <errhandlingapi.h>
#include <iostream>
std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

#endif

#else
#define OS_DEP(UNIX_EXPRESSION, WINDOWS_EXPRESSION) UNIX_EXPRESSION  
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

using namespace pmmf;

void MemoryMappedFile::clearData() {
  file_descriptor = INVALID_FILE_DESCRIPTOR;
  memory_page_start = nullptr;
  mapped_size = 0;
}

FileDescriptor MemoryMappedFile::openFile(std::string file_path, Protocol prot) {
  if(access(file_path.c_str(), F_OK) == 0) {
#ifdef _WIN32
    return ::CreateFile(
          file_path.c_str(),
          ((DWORD)0x00)
          | (((int)prot & (int)Protocol::read || (int)prot & (int)Protocol::exec) ? GENERIC_READ : 0x00)
          | (((int)prot & (int)Protocol::write) ? GENERIC_WRITE : 0x00),
          (((int)prot & (int)Protocol::write) ? 0x00 : FILE_SHARE_READ),
          0, ((int)prot & (int)Protocol::open_trunc) ? TRUNCATE_EXISTING : OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, 0);
#else
    return ::open(
          file_path.c_str(),
          ((int)0x00
          | (((int)prot & (int)Protocol::open_trunc) ? O_TRUNC : 0x00)
          | ((int)prot & (int)Protocol::read && (int)prot & (int)Protocol::write) ? O_RDWR
          : ((int)prot & (int)Protocol::read) ? O_RDONLY
            : ((int)prot & (int)Protocol::write)
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
          | (((int)prot & (int)Protocol::read || (int)prot & (int)Protocol::exec) ? GENERIC_READ : 0x00)
          | (((int)prot & (int)Protocol::write) ? GENERIC_WRITE : 0x00),
          (((int)prot & (int)Protocol::write) ? 0x00 : FILE_SHARE_READ),
          0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
#else
    return ::open(
          file_path.c_str(),
          ((int)O_CREAT
          | (((int)prot & (int)Protocol::open_trunc) ? O_TRUNC : 0x00)
          | ((int)prot & (int)Protocol::read && (int)prot & (int)Protocol::write) ? O_RDWR
          : ((int)prot & (int)Protocol::read) ? O_RDONLY
            : ((int)prot & (int)Protocol::write)
              ? O_WRONLY
              : 0x00),
          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
          );
#endif
  }


}

MemoryMappedFile::MemoryMappedFile(std::string file_path,
    std::size_t map_size,
    Protocol prot,
    MapFlag flags,
    std::size_t offset)
  : file_descriptor(openFile(std::move(file_path), prot)),
    memory_page_start(isFileOpen() ? mmapPortable(nullptr, map_size, prot, flags, file_descriptor, offset) : nullptr),
    mapped_size(isFileMapped() ? map_size : 0) {}

MemoryMappedFile::MemoryMappedFile(std::string file_path, Protocol prot, MapFlag flags)
  : file_descriptor(openFile(std::move(file_path), prot)),
    memory_page_start(isFileOpen() ? mmapPortable(nullptr, getFileSize(), prot, flags, file_descriptor, 0) : nullptr),
    mapped_size(isFileMapped() ? getFileSize() : 0) {}

MemoryMappedFile::MemoryMappedFile(std::string file_path)
  : file_descriptor(openFile(std::move(file_path), Protocol((int)Protocol::read | (int)Protocol::write))),
    memory_page_start(isFileOpen() ? mmapPortable(nullptr, getFileSize(), Protocol((int)Protocol::read | (int)Protocol::write), MapFlag::priv, file_descriptor, 0) : nullptr),
    mapped_size(isFileMapped() ? getFileSize() : 0) {}


MemoryMappedFile::~MemoryMappedFile() {
  if(memory_page_start) munmapPortable(memory_page_start, mapped_size);
  if(file_descriptor != INVALID_FILE_DESCRIPTOR)
#ifdef _WIN32
    ::CloseHandle(file_descriptor);
#else
    ::close(file_descriptor);
#endif
  clearData();
}

std::size_t MemoryMappedFile::getFileSize() const {
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

bool MemoryMappedFile::flush() {
  if(isFileMapped()) {
#ifdef _WIN32
#ifdef DEBUG
    bool flag = true;
    if(!::FlushViewOfFile(memory_page_start, 0)) {
      flag = false;
      std::cout << GetLastErrorAsString() << std::endl;
    }

    if(!::FlushFileBuffers(file_descriptor)) {
      flag = false;
      std::cout << GetLastErrorAsString() << std::endl;
    }

    return flag;
  }

#else
    return
        ::FlushViewOfFile(memory_page_start, 0) &&
        ::FlushFileBuffers(file_descriptor);
  } else return !::FlushFileBuffers(file_descriptor);
#endif
#else
    return ::msync(memory_page_start, mapped_size, MS_SYNC) == 0;
  } else return false;
#endif
}
