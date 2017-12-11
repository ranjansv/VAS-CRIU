#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "int.h"
#include "types.h"
#include "page.h"
#include "syscall.h"
#include "image.h"
#include "parasite-vdso.h"
#include "vma.h"
#include "log.h"
#include "common/bug.h"

#ifdef LOG_PREFIX
# undef LOG_PREFIX
#endif
#define LOG_PREFIX "vdso: "


int vdso_remap(char *who, unsigned long from, unsigned long to, size_t size)
{
	unsigned long addr;

	pr_debug("Remap %s %lx -> %lx\n", who, from, to);

	addr = sys_mremap(from, size, size, MREMAP_MAYMOVE | MREMAP_FIXED, to);
	if (addr != to) {
		pr_err("Unable to remap %lx -> %lx %lx\n",
		       from, to, addr);
		return -1;
	}

	return 0;
}

/* Park runtime vDSO in some safe place where it can be accessible from restorer */
int vdso_do_park(struct vdso_symtable *sym_rt, unsigned long park_at, unsigned long park_size)
{
	int ret;

	BUG_ON((vdso_vma_size(sym_rt) + vvar_vma_size(sym_rt)) < park_size);

	if (sym_rt->vvar_start != VDSO_BAD_ADDR) {
		if (sym_rt->vma_start < sym_rt->vvar_start) {
			ret  = vdso_remap("rt-vdso", sym_rt->vma_start,
					  park_at, vdso_vma_size(sym_rt));
			park_at += vdso_vma_size(sym_rt);
			ret |= vdso_remap("rt-vvar", sym_rt->vvar_start,
					  park_at, vvar_vma_size(sym_rt));
		} else {
			ret  = vdso_remap("rt-vvar", sym_rt->vvar_start,
					  park_at, vvar_vma_size(sym_rt));
			park_at += vvar_vma_size(sym_rt);
			ret |= vdso_remap("rt-vdso", sym_rt->vma_start,
					  park_at, vdso_vma_size(sym_rt));
		}
	} else
		ret = vdso_remap("rt-vdso", sym_rt->vma_start,
				 park_at, vdso_vma_size(sym_rt));
	return ret;
}

