#ifndef MMF_H
#define MMF_H

#include <cstddef>
#include <string>
#include <set>
#include <shared_mutex>

#include "mmap_wrapper.hxx"

namespace pmmf {

class MappedFile {
public:

  template<typename T>
  class MappedArray {
    friend class MappedFile;
    FileDescriptor file_descriptor;
    size_t offset;
    size_t byte_offset;
    size_t element_count;
    void* page_start;

    MappedArray(FileDescriptor file_descriptor, size_t offset, size_t byte_offset, size_t element_count, void* page_start)
      : file_descriptor(file_descriptor), offset(offset), byte_offset(byte_offset), element_count(element_count), page_start(page_start) {}

    T* getDataStart() const { return reinterpret_cast<T*>(reinterpret_cast<char*>(page_start) + byte_offset); }

  public:
    MappedArray(const MappedArray<T>&) = delete;

    MappedArray(MappedArray<T>&& other)
      : file_descriptor(other.file_descriptor), offset(other.offset), byte_offset(other.byte_offset), element_count(other.element_count), page_start(other.page_start) {
      other.file_descriptor = INVALID_FILE_DESCRIPTOR;
      other.offset = -1;
      other.byte_offset = -1;
      other.element_count = -1;
      other.page_start = nullptr;
    }

    ~MappedArray() {
      if(page_start) {
        flush();
        munmap(page_start, byte_offset + element_count * sizeof(T));
      }
    }

    inline bool isMapped() const { return !!page_start; }
    inline size_t getDataOffset() const { return offset + byte_offset; }
    inline size_t getPageOffset() const { return offset; }
    inline size_t getElementCount() const { return element_count; }
    inline size_t getDataSize() const { return element_count * sizeof(T); }
    inline size_t getPageSize() const { return byte_offset + element_count * sizeof(T); }

    T& operator[](size_t index) { return getDataStart() + index; }
    const T& operator[](size_t index) const { return getDataStart() + index; }
    T& operator*() { return *getDataStart(); }
    const T& operator*() const { return *getDataStart(); }

    bool flush() {
      if(isMapped()) {
#ifdef _WIN32
        return
            ::FlushViewOfFile(page_start, 0) &&
            ::FlushFileBuffers(file_descriptor);
      } else return !::FlushFileBuffers(file_descriptor);
#else
        return ::msync(page_start, byte_offset + element_count * sizeof(T), MS_SYNC) == 0;
      } else return false;
#endif
    }

  };

  template<typename T>
  friend class MappedArray;


private:

  static size_t getSystemPageSize() {
  #ifdef _WIN32
    SYSTEM_INFO sysinfo = {0};
    ::GetSystemInfo(&sysinfo);
    return sysinfo.dwAllocationGranularity;
  #else
    return sysconf(_SC_PAGESIZE);
  #endif
  }

  static inline const size_t OS_PAGE_SIZE = getSystemPageSize();
  FileDescriptor file_descriptor = INVALID_FILE_DESCRIPTOR;
  ProtectionMode proterction_mode = ProtectionMode(0x00);
  MapFlag map_flag = MapFlag(0x00);

  FileDescriptor openFile(std::string file_path, ProtectionMode protection_mode);

public:
  MappedFile(std::string file_path, ProtectionMode protection_mode = ProtectionMode::rw, MapFlag map_flag = MapFlag::priv);
  MappedFile(const MappedFile&) = delete;
  MappedFile(MappedFile&& other)
    : file_descriptor(other.file_descriptor), proterction_mode(other.proterction_mode), map_flag(other.map_flag) {}

  inline bool isFileOpen() const { return file_descriptor != INVALID_FILE_DESCRIPTOR; }

  template<typename T>
  MappedArray<T> getMappedArray(size_t byte_offset, size_t element_count = 1) {
    // MappedArray(MappedFile* file, size_t offset, size_t byte_offset, size_t element_count, void* page_start)
    if(!isFileOpen())
      return MappedArray<T>(nullptr, -1, -1, -1, nullptr);
    const size_t page_aligned_offset = (byte_offset / OS_PAGE_SIZE) * OS_PAGE_SIZE;
    byte_offset = byte_offset - page_aligned_offset;
    void* page_start = mmap(nullptr, byte_offset + element_count * sizeof(T), proterction_mode, map_flag, file_descriptor, page_aligned_offset);
    if(!page_start)
      return MappedArray<T>(nullptr, -1, -1, -1, nullptr);
    return MappedArray<T>(file_descriptor, page_aligned_offset, byte_offset, element_count, page_start);
  }

};

template <typename T>
using MappedArray = MappedFile::MappedArray<T>;

}

#undef OS_DEP

#endif //MMF_H
