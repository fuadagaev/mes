# 1 "src/linux/getdents.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/linux/getdents.c"

# 1 "./include/dirent.h" 1







# 1 "./src/include/features.h" 1



# 1 "./src/include/../../include/features.h" 1
# 5 "./src/include/features.h" 2
# 9 "./include/dirent.h" 2







# 1 "obj/include/bits/alltypes.h" 1
# 136 "obj/include/bits/alltypes.h"
typedef unsigned int size_t;
# 238 "obj/include/bits/alltypes.h"
typedef long long off_t;




typedef unsigned long long ino_t;
# 17 "./include/dirent.h" 2

typedef struct __dirstream DIR;





struct dirent {
 ino_t d_ino;
 off_t d_off;
 unsigned short d_reclen;
 unsigned char d_type;
 char d_name[256];
};



int closedir(DIR *);
DIR *fdopendir(int);
DIR *opendir(const char *);
struct dirent *readdir(DIR *);
int readdir_r(DIR *restrict, struct dirent *restrict, struct dirent **restrict);
void rewinddir(DIR *);
int dirfd(DIR *);

int alphasort(const struct dirent **, const struct dirent **);
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **));


void seekdir(DIR *, long);
long telldir(DIR *);
# 62 "./include/dirent.h"
int getdents(int, struct dirent *, size_t);
# 3 "src/linux/getdents.c" 2
# 1 "./include/limits.h" 1







# 1 "./arch/i386/bits/limits.h" 1
# 9 "./include/limits.h" 2
# 4 "src/linux/getdents.c" 2
# 1 "./src/internal/syscall.h" 1




# 1 "./include/sys/syscall.h" 1



# 1 "obj/include/bits/syscall.h" 1
# 5 "./include/sys/syscall.h" 2
# 6 "./src/internal/syscall.h" 2
# 1 "./arch/i386/syscall_arch.h" 1
# 15 "./arch/i386/syscall_arch.h"
static inline long __syscall0(long n)
{
 unsigned long __ret;
 __asm__ __volatile__ ("call *%%gs:16" : "=a"(__ret) : "a"(n) : "memory");
 return __ret;
}

static inline long __syscall1(long n, long a1)
{
 unsigned long __ret;
 __asm__ __volatile__ ("xchg %%ebx,%%edx ; " "call *%%gs:16" " ; xchg %%ebx,%%edx" : "=a"(__ret) : "a"(n), "d"(a1) : "memory");
 return __ret;
}

static inline long __syscall2(long n, long a1, long a2)
{
 unsigned long __ret;
 __asm__ __volatile__ ("xchg %%ebx,%%edx ; " "call *%%gs:16" " ; xchg %%ebx,%%edx" : "=a"(__ret) : "a"(n), "d"(a1), "c"(a2) : "memory");
 return __ret;
}

static inline long __syscall3(long n, long a1, long a2, long a3)
{
 unsigned long __ret;

 __asm__ __volatile__ ("call *%%gs:16" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory");



 return __ret;
}

static inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
 unsigned long __ret;

 __asm__ __volatile__ ("call *%%gs:16" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");



 return __ret;
}

static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
 unsigned long __ret;

 __asm__ __volatile__ ("call *%%gs:16"
  : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");




 return __ret;
}

static inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
 unsigned long __ret;

 __asm__ __volatile__ ("pushl %7 ; push %%ebp ; mov 4(%%esp),%%ebp ; " "call *%%gs:16" " ; pop %%ebp ; add $4,%%esp"
  : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "g"(a6) : "memory");





 return __ret;
}
# 7 "./src/internal/syscall.h" 2
# 22 "./src/internal/syscall.h"
typedef long syscall_arg_t;


__attribute__((__visibility__("hidden"))) long __syscall_ret(unsigned long),
 __syscall_cp(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t,
              syscall_arg_t, syscall_arg_t, syscall_arg_t);
# 335 "./src/internal/syscall.h"
__attribute__((__visibility__("hidden"))) void __procfdname(char __buf[27], unsigned);

__attribute__((__visibility__("hidden"))) void *__vdsosym(const char *, const char *);
# 5 "src/linux/getdents.c" 2

int getdents(int fd, struct dirent *buf, size_t len)
{
 if (len>0x7fffffff) len = 0x7fffffff;
 return __syscall_ret(__syscall3(220,((long) (fd)),((long) (buf)),((long) (len))));
}

extern __typeof(getdents) getdents64 __attribute__((__weak__, __alias__("getdents")));
