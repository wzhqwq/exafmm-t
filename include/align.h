#ifndef align_h
#define align_h

#ifdef _WIN32
#define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ? 0 : errno)
#define free(p) _aligned_free(p)
#include <errno.h>
#include <malloc.h>
#else
#include <cstdlib>
#endif

#include <memory>

template <typename T, size_t NALIGN>
struct AlignedAllocator : public std::allocator<T> {
  template <typename U>
  struct rebind {
    typedef AlignedAllocator<U, NALIGN> other;
  };

  T * allocate(size_t n) {
    void *ptr = nullptr;
    int rc = posix_memalign(&ptr, NALIGN, n * sizeof(T));
    if (rc != 0) return nullptr;
    if (ptr == nullptr) throw std::bad_alloc();
    return reinterpret_cast<T*>(ptr);
  }

  void deallocate(T * p, size_t) {
    return free(p);
  }
};
#endif
