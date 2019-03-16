/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 *
 * This file is part of GNU Mes.
 *
 * GNU Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * GNU Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MES_GNU_SYSCALL_H
#define __MES_GNU_SYSCALL_H

#if __MESC__

// stripped mach/message.h -- possibly just copy message.h and edit
// that...
typedef unsigned int mach_msg_bits_t;
typedef	unsigned int mach_msg_size_t;

#if 0
typedef natural_t mach_msg_seqno_t;
typedef integer_t mach_msg_id_t;
#else
typedef unsigned int mach_msg_seqno_t;
typedef int mach_msg_id_t;
#endif

typedef	struct {
    mach_msg_bits_t	msgh_bits;
    mach_msg_size_t	msgh_size;
    mach_port_t		msgh_remote_port;
    union {
        mach_port_t	msgh_local_port;
        unsigned long	msgh_protected_payload;
    };
    mach_port_seqno_t	msgh_seqno;
    mach_msg_id_t	msgh_id;
} mach_msg_header_t;


typedef unsigned int mach_msg_type_name_t;
typedef unsigned int mach_msg_type_size_t;
// #if 0
// typedef natural_t mach_msg_type_number_t;
// #else
// typedef unsigned int mach_msg_type_number_t;
// #endif

typedef struct  {
    unsigned int	msgt_name : 8,
			msgt_size : 8,
			msgt_number : 12,
			msgt_inline : 1,
			msgt_longform : 1,
			msgt_deallocate : 1,
			msgt_unused : 1;
} mach_msg_type_t;

typedef	struct	{
    mach_msg_type_t	msgtl_header;
    unsigned short	msgtl_name;
    unsigned short	msgtl_size;
    natural_t		msgtl_number;
} mach_msg_type_long_t;

#if 0
typedef kern_return_t mach_msg_return_t;
typedef natural_t mach_msg_timeout_t;
typedef integer_t mach_msg_option_t;
#else
typedef int mach_msg_return_t;
typedef unsigned int mach_msg_timeout_t;
typedef int mach_msg_option_t;
#endif

#define MACH_MSG_OPTION_NONE	0x00000000

#define	MACH_SEND_MSG		0x00000001
#define	MACH_RCV_MSG		0x00000002

#define MACH_SEND_TIMEOUT	0x00000010
#define MACH_SEND_NOTIFY	0x00000020
#define MACH_SEND_CANCEL	0x00000080
#define MACH_RCV_TIMEOUT	0x00000100
#define MACH_RCV_NOTIFY		0x00000200
#define MACH_RCV_LARGE		0x00000800

extern mach_msg_return_t
mach_msg
   (mach_msg_header_t *msg,
    mach_msg_option_t option,
    mach_msg_size_t send_size,
    mach_msg_size_t rcv_size,
    mach_port_t rcv_name,
    mach_msg_timeout_t timeout,
    mach_port_t notify);

#else

#define _GNU_SOURCE 1
#define __USE_GNU 1

#include <mach/mach_types.h>
#include <mach/port.h>
#include <mach/message.h>
#include <gnu/hurd.h>
#include <gnu/hurd-types.h>

#endif

// mach/mach.defs
enum __mach_defs
  {
   SYS__task_terminate = 2008,
   SYS__vm_allocate = 2021,
   SYS__vm_statistics = 2030,
   SYS__task_get_special_port = 2058,
  };

// hurd/fsys.defs
enum __fsys_defs
  {
   SYS__dir_lookup = 20018,
  };

// hurd/io.defs
enum __io_defs
  {
   SYS__io_write = 21000,
   SYS__io_read,
  };

// hurd/process.defs
enum __process_defs
  {
   SYS__proc_mark_exit = 24025,
  };

// hurd/startup.defs
enum __startup_defs
  {
   SYS__exec_startup_get_info = 30500,
  };

extern mach_msg_type_t mach_msg_type_int32;
extern mach_msg_type_t mach_msg_type_int64;
extern mach_msg_type_long_t mach_msg_type_pointer;

struct mach_msg
{
  mach_msg_header_t header;
};

struct mach_msg_1
{
  mach_msg_header_t header;
  mach_msg_type_t type_one; int one;
};

struct mach_msg_2
{
  mach_msg_header_t header;
  mach_msg_type_t type_one; int one;
  mach_msg_type_t type_two; int two;
};

struct mach_msg_loff_int
{
  mach_msg_header_t header;
  mach_msg_type_t type_one; loff_t one;
  mach_msg_type_t type_two; int two;
};

struct mach_msg_startup_info
{
  mach_msg_header_t header;
  mach_msg_type_t RetCodeType;
  kern_return_t RetCode;
  mach_msg_type_t user_entryType;
  vm_address_t user_entry;
  mach_msg_type_t phdrType;
  vm_address_t phdr;
  mach_msg_type_t phdr_sizeType;
  vm_size_t phdr_size;
  mach_msg_type_t stack_baseType;
  vm_address_t stack_base;
  mach_msg_type_t stack_sizeType;
  vm_size_t stack_size;
  mach_msg_type_t flagsType;
  int flags;
  mach_msg_type_long_t argvType;
  char *argv;
  mach_msg_type_long_t envpType;
  char *envp;
  mach_msg_type_long_t dtableType;
  mach_port_t *dtable;
  mach_msg_type_long_t portarrayType;
  mach_port_t *portarray;
  mach_msg_type_long_t intarrayType;
  int *intarray;
};

//#include <mach/machine/vm_types.h> // natural_t, integer_t, vm_size_t
////#include <mach/vm_statistics.h>

///// vm_statistics
struct vm_statistics {
	integer_t	pagesize;		/* page size in bytes */
	integer_t	free_count;		/* # of pages free */
	integer_t	active_count;		/* # of pages active */
	integer_t	inactive_count;		/* # of pages inactive */
	integer_t	wire_count;		/* # of pages wired down */
	integer_t	zero_fill_count;	/* # of zero fill pages */
	integer_t	reactivations;		/* # of pages reactivated */
	integer_t	pageins;		/* # of pageins */
	integer_t	pageouts;		/* # of pageouts */
	integer_t	faults;			/* # of faults */
	integer_t	cow_faults;		/* # of copy-on-writes */
	integer_t	lookups;		/* object cache lookups */
	integer_t	hits;			/* object cache hits */
};
typedef struct vm_statistics *vm_statistics_t;
typedef struct vm_statistics vm_statistics_data_t;
////// end vm_statistics

kern_return_t __syscall (mach_port_t port, int sys_call);
kern_return_t __syscall2 (mach_port_t port, int sys_call, int one, int two);
kern_return_t __syscall_get (mach_port_t port, int sys_call, mach_msg_header_t *message, size_t size);
kern_return_t __syscall_put (mach_port_t port, int sys_call, mach_msg_header_t *message, size_t size);

// mach.defs
kern_return_t __task_terminate (mach_port_t task);
kern_return_t __task_get_special_port (mach_port_t task, int which, mach_port_t *port);
kern_return_t __vm_allocate (mach_port_t task, vm_address_t *address, vm_size_t size, boolean_t anywhere);
kern_return_t __vm_statistics (mach_port_t task, vm_statistics_data_t *vm_stats);

// process.defs
kern_return_t __proc_mark_exit (mach_port_t process, int one, int two);
kern_return_t __exec_startup_get_data (mach_port_t bootstrap, struct hurd_startup_data *data);

// io.c
kern_return_t __io_write (io_t io, data_t data, mach_msg_type_number_t size, loff_t offset, vm_size_t *wrote);
kern_return_t __io_read (io_t io, data_t *data, mach_msg_type_number_t *read, loff_t offset, vm_size_t size);

#endif // __MES_GNU_SYSCALL_H
