# 1 "src/dirent/opendir.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/dirent/opendir.c"

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



int versionsort(const struct dirent **, const struct dirent **);
# 3 "src/dirent/opendir.c" 2
# 1 "./include/fcntl.h" 1
# 20 "./include/fcntl.h"
# 1 "obj/include/bits/alltypes.h" 1
# 151 "obj/include/bits/alltypes.h"
typedef int ssize_t;
# 228 "obj/include/bits/alltypes.h"
typedef unsigned mode_t;
# 311 "obj/include/bits/alltypes.h"
typedef int pid_t;
# 420 "obj/include/bits/alltypes.h"
struct iovec { void *iov_base; size_t iov_len; };
# 21 "./include/fcntl.h" 2

# 1 "./arch/generic/bits/fcntl.h" 1
# 23 "./include/fcntl.h" 2

struct flock {
 short l_type;
 short l_whence;
 off_t l_start;
 off_t l_len;
 pid_t l_pid;
};

int creat(const char *, mode_t);
int fcntl(int, int, ...);
int open(const char *, int, ...);
int openat(int, const char *, int, ...);
int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);
# 159 "./include/fcntl.h"
int lockf(int, int, off_t);







struct file_handle {
 unsigned handle_bytes;
 int handle_type;
 unsigned char f_handle[];
};
struct f_owner_ex {
 int type;
 pid_t pid;
};
# 186 "./include/fcntl.h"
int fallocate(int, int, off_t, off_t);

int name_to_handle_at(int, const char *, struct file_handle *, int *, int);
int open_by_handle_at(int, struct file_handle *, int);
ssize_t readahead(int, off_t, size_t);
int sync_file_range(int, off_t, off_t, unsigned);
ssize_t vmsplice(int, const struct iovec *, size_t, unsigned);
ssize_t splice(int, off_t *, int, off_t *, size_t, unsigned);
ssize_t tee(int, int, size_t, unsigned);
# 4 "src/dirent/opendir.c" 2
# 1 "./src/include/stdlib.h" 1



# 1 "./src/include/../../include/stdlib.h" 1
# 19 "./src/include/../../include/stdlib.h"
# 1 "obj/include/bits/alltypes.h" 1
# 32 "obj/include/bits/alltypes.h"
typedef long int wchar_t;
# 20 "./src/include/../../include/stdlib.h" 2

int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char *restrict, char **restrict);
double strtod (const char *restrict, char **restrict);
long double strtold (const char *restrict, char **restrict);

long strtol (const char *restrict, char **restrict, int);
unsigned long strtoul (const char *restrict, char **restrict, int);
long long strtoll (const char *restrict, char **restrict, int);
unsigned long long strtoull (const char *restrict, char **restrict, int);

int rand (void);
void srand (unsigned);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *aligned_alloc(size_t, size_t);

__attribute__((__noreturn__)) void abort (void);
int atexit (void (*) (void));
__attribute__((__noreturn__)) void exit (int);
__attribute__((__noreturn__)) void _Exit (int);
int at_quick_exit (void (*) (void));
__attribute__((__noreturn__)) void quick_exit (int);

char *getenv (const char *);

int system (const char *);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);

int mblen (const char *, size_t);
int mbtowc (wchar_t *restrict, const char *restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *restrict, const char *restrict, size_t);
size_t wcstombs (char *restrict, const wchar_t *restrict, size_t);




size_t __ctype_get_mb_cur_max(void);
# 99 "./src/include/../../include/stdlib.h"
int posix_memalign (void **, size_t, size_t);
int setenv (const char *, const char *, int);
int unsetenv (const char *);
int mkstemp (char *);
int mkostemp (char *, int);
char *mkdtemp (char *);
int getsubopt (char **, char *const *, char **);
int rand_r (unsigned *);






char *realpath (const char *restrict, char *restrict);
long int random (void);
void srandom (unsigned int);
char *initstate (unsigned int, char *, size_t);
char *setstate (char *);
int putenv (char *);
int posix_openpt (int);
int grantpt (int);
int unlockpt (int);
char *ptsname (int);
char *l64a (long);
long a64l (const char *);
void setkey (const char *);
double drand48 (void);
double erand48 (unsigned short [3]);
long int lrand48 (void);
long int nrand48 (unsigned short [3]);
long mrand48 (void);
long jrand48 (unsigned short [3]);
void srand48 (long);
unsigned short *seed48 (unsigned short [3]);
void lcong48 (unsigned short [7]);



# 1 "./include/alloca.h" 1
# 9 "./include/alloca.h"
# 1 "obj/include/bits/alltypes.h" 1
# 10 "./include/alloca.h" 2

void *alloca(size_t);
# 139 "./src/include/../../include/stdlib.h" 2
char *mktemp (char *);
int mkstemps (char *, int);
int mkostemps (char *, int, int);
void *valloc (size_t);
void *memalign(size_t, size_t);
int getloadavg(double *, int);
int clearenv(void);





int ptsname_r(int, char *, size_t);
char *ecvt(double, int, int *, int *);
char *fcvt(double, int, int *, int *);
char *gcvt(double, int, char *);
char *secure_getenv(const char *);
struct __locale_struct;
float strtof_l(const char *restrict, char **restrict, struct __locale_struct *);
double strtod_l(const char *restrict, char **restrict, struct __locale_struct *);
long double strtold_l(const char *restrict, char **restrict, struct __locale_struct *);
# 5 "./src/include/stdlib.h" 2

__attribute__((__visibility__("hidden"))) int __putenv(char *, size_t, char *);
__attribute__((__visibility__("hidden"))) void __env_rm_add(char *, char *);
__attribute__((__visibility__("hidden"))) int __mkostemps(char *, int, int);
__attribute__((__visibility__("hidden"))) int __ptsname_r(int, char *, size_t);
__attribute__((__visibility__("hidden"))) char *__randname(char *);
# 5 "src/dirent/opendir.c" 2
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
# 6 "src/dirent/opendir.c" 2
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
# 7 "src/dirent/opendir.c" 2

DIR *opendir(const char *name)
{
 int fd;
 DIR *dir;

 if ((fd = open(name, 00|0200000|02000000)) < 0)
  return 0;
 if (!(dir = calloc(1, sizeof *dir))) {
  __syscall1(6,((long) (fd)));
  return 0;
 }
 dir->fd = fd;
 return dir;
}
