#if !defined(MG__NETBSD__H__) && defined(__NetBSD__)
#define MG__NETBSD__H__

#include <util.h>
#include <sys/stat.h>

#define st_atim st_atimespec
#define st_ctim st_ctimespec
#define st_mtim st_mtimespec

static inline int pledge(const char *s, void *p)  {
    if (p == s) return 0;
    else return 1;
}

#endif