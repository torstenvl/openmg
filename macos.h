#if !defined(MG__APPLE__H__) && defined(__APPLE__) && defined(__MACH__)
#define MG__APPLE__H__

#include <stdint.h>
#include <stdlib.h>
#include <util.h>
#include <sys/stat.h>
#include <sys/queue.h>

#define st_atim st_atimespec
#define st_ctim st_ctimespec
#define st_mtim st_mtimespec

#define LOGIN_NAME_MAX 9

static inline void *reallocarray(void *p, size_t n, size_t z) {
    if (!(n && z)) return NULL;
    if ((SIZE_MAX / z) < n) return NULL;
    return realloc(p, n * z);
}

static inline int pledge(const char *s, void *p)  {
    if (p == s) return 0;
    else return 1;
}

#endif