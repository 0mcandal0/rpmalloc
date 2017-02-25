
#include <stdint.h>
#include <stddef.h>

extern int
benchmark_initialize(void);

extern int
benchmark_finalize(void);

extern int
benchmark_thread_initialize(void);

extern int
benchmark_thread_finalize(void);

extern void*
benchmark_malloc(size_t alignment, size_t size);

extern void
benchmark_free(void* ptr);

extern const char*
benchmark_name(void);
