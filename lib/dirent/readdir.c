# 1 "src/dirent/readdir.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/dirent/readdir.c"
# 1 "./include/dirent.h" 1







# 1 "./src/include/features.h" 1



# 1 "./src/include/../../include/features.h" 1
# 5 "./src/include/features.h" 2
# 9 "./include/dirent.h" 2







# 1 "obj/include/bits/alltypes.h" 1
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
# 2 "src/dirent/readdir.c" 2
# 1 "./src/include/errno.h" 1


extern int errno;
# 1 "./src/include/../../include/errno.h" 1
# 10 "./src/include/../../include/errno.h"
# 1 "./arch/generic/bits/errno.h" 1
# 11 "./src/include/../../include/errno.h" 2


__attribute__((const))

//int *__errno_location(void);
# 5 "./src/include/errno.h" 2


__attribute__((const))

//__attribute__((__visibility__("hidden"))) int *___errno_location(void);
# 3 "src/dirent/readdir.c" 2
# 1 "./include/stddef.h" 1
# 17 "./include/stddef.h"
# 1 "obj/include/bits/alltypes.h" 1
# 32 "obj/include/bits/alltypes.h"
typedef long int wchar_t;
# 136 "obj/include/bits/alltypes.h"
typedef unsigned int size_t;
# 146 "obj/include/bits/alltypes.h"
typedef int ptrdiff_t;
# 18 "./include/stddef.h" 2
# 4 "src/dirent/readdir.c" 2
# 1 "src/dirent/__dirent.h" 1
struct __dirstream
{
 off_t tell;
 int fd;
 int buf_pos;
 int buf_end;
 volatile int lock[1];


 char buf[2048];
};
# 5 "src/dirent/readdir.c" 2
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
# 6 "src/dirent/readdir.c" 2

typedef char dirstream_buf_alignment_check[1-2*(int)(
 __builtin_offsetof(struct __dirstream, buf) % sizeof(off_t))];

struct dirent *readdir(DIR *dir)
{
 struct dirent *de;

 if (dir->buf_pos >= dir->buf_end) {
  int len = __syscall3(220,((long) (dir->fd)),((long) (dir->buf)),((long) (sizeof dir->buf)));
  if (len <= 0) {
   if (len < 0 && len != -2) errno = -len;
   return 0;
  }
  dir->buf_end = len;
  dir->buf_pos = 0;
 }
 de = (void *)(dir->buf + dir->buf_pos);
 dir->buf_pos += de->d_reclen;
 dir->tell = de->d_off;
 return de;
}

extern __typeof(readdir) readdir64 __attribute__((__weak__, __alias__("readdir")));
