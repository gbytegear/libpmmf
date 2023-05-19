#ifndef MMF_H
#define MMF_H

#include <cstddef>
#include <string>
#include <iterator>

#include "mmap_wrapper.hxx"

#ifdef _WIN32
#define IF_WIN(exp) exp
#define IF_UNIX(exp)
#else
#define IF_WIN(exp)
#define IF_UNIX(exp) exp
#include <sys/mman.h>
#include <unistd.h>
#endif

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

    inline T& operator[](size_t index) { return *(getDataStart() + index); }
    inline const T& operator[](size_t index) const { return *(getDataStart() + index); }
    inline T& operator*() { return *getDataStart(); }
    inline const T& operator*() const { return *getDataStart(); }
    inline T* operator->() { return getDataStart(); }
    inline const T* operator->() const { return getDataStart(); }

    inline T* begin() { return getDataStart(); }
    inline T* end() { return getDataStart() + element_count; }
    inline const T* begin() const { return getDataStart(); }
    inline const T* end() const { return getDataStart() + element_count; }
    inline const T* cbegin() const { return getDataStart(); }
    inline const T* cend() const { return getDataStart() + element_count; }
    inline std::reverse_iterator<T*> rbegin() { return getDataStart() + element_count - 1; }
    inline std::reverse_iterator<T*> rend() { return getDataStart() - 1; }
    inline std::reverse_iterator<const T*> rbegin() const { return getDataStart() + element_count - 1; }
    inline std::reverse_iterator<const T*> rend() const { return getDataStart() - 1; }
    inline std::reverse_iterator<const T*> crbegin() const { return getDataStart() + element_count - 1; }
    inline std::reverse_iterator<const T*> crend() const { return getDataStart() - 1; }

    bool flush() {
      #ifdef _WIN32
      constexpr int MS_SYNC = 0;
      #endif
      if(isMapped()) return msync(file_descriptor, page_start, byte_offset + element_count * sizeof(T), MS_SYNC) == 0;
      return false;
    }
  };

  template<typename T>
  friend class MappedArray;


private:

  static size_t getSystemPageSize();

  static inline const size_t OS_PAGE_SIZE = getSystemPageSize();
  FileDescriptor file_descriptor = INVALID_FILE_DESCRIPTOR;
  ProtectionMode proterction_mode = ProtectionMode(0x00);
  MapFlag map_flag = MapFlag(0x00);

  FileDescriptor openFile(std::string file_path, ProtectionMode protection_mode);

public:
  MappedFile(std::string file_path, ProtectionMode protection_mode = ProtectionMode::rw, MapFlag map_flag = MapFlag::shared);
  MappedFile(const MappedFile&) = delete;
  MappedFile(MappedFile&& other)
    : file_descriptor(other.file_descriptor), proterction_mode(other.proterction_mode), map_flag(other.map_flag) {}

  ~MappedFile();

  static inline size_t getOSPageSize() { return OS_PAGE_SIZE; }
  inline bool isFileOpen() const { return file_descriptor != INVALID_FILE_DESCRIPTOR; }
  size_t getFileSize() const;

  template<typename T>
  MappedArray<T> getMappedArray(size_t byte_offset, size_t element_count = 1) {
    if(!isFileOpen())
      return MappedArray<T>(INVALID_FILE_DESCRIPTOR, -1, -1, -1, nullptr);
    #ifndef _WIN32
    if(getFileSize() < byte_offset + element_count * sizeof(T))
      if(ftruncate64(file_descriptor, byte_offset + element_count * sizeof(T)) != 0)
        return MappedArray<T>(INVALID_FILE_DESCRIPTOR, -1, -1, -1, nullptr);
    #endif
    const size_t page_aligned_offset = (byte_offset / OS_PAGE_SIZE) * OS_PAGE_SIZE;
    byte_offset = byte_offset - page_aligned_offset;
    void* page_start = mmap(nullptr, byte_offset + element_count * sizeof(T), proterction_mode, map_flag, file_descriptor, page_aligned_offset);
    if(page_start == (void*)-1)
      return MappedArray<T>(INVALID_FILE_DESCRIPTOR, -1, -1, -1, nullptr);
    return MappedArray<T>(file_descriptor, page_aligned_offset, byte_offset, element_count, page_start);
  }

};

template <typename T>
using MappedArray = MappedFile::MappedArray<T>;

}

#undef OS_DEP

#endif //MMF_H
