/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compatibility shim for the removed strncpy() string helper.
 *
 * The kernel's <linux/string.h> no longer declares (and vmlinux no longer
 * exports) strncpy(). The out-of-tree Tegra drivers in this tree still call
 * it. This header is force-included (via -include) for the nvidia-oot build so
 * that every call site compiles against a declaration with the historical
 * semantics:
 *
 *     char *strncpy(char *dest, const char *src, size_t count);
 *
 * It copies up to @count characters from @src to @dest; if @src is shorter
 * than @count, the remainder of @dest is padded with NUL bytes. @dest must be
 * at least @count bytes long. Returns @dest.
 *
 * IMPORTANT: this header is included on the command line (before the source
 * file's own #defines). It must therefore NOT include <linux/string.h> or any
 * header that defines pr_fmt() (printk.h), because doing so would define the
 * default pr_fmt() before the source file gets to define its own, triggering a
 * "pr_fmt redefined" error. It uses only plain loops and size_t, so it relies
 * solely on <linux/types.h>. Only the public-source tree is affected.
 */
#ifndef _LINUX_STRNCPY_COMPAT_H
#define _LINUX_STRNCPY_COMPAT_H

#include <linux/types.h>

#ifndef NV_STRNCPY_COMPAT_DECLARED
#define NV_STRNCPY_COMPAT_DECLARED
static inline char *nv_compat_strncpy(char *dest, const char *src, size_t count)
{
	size_t i = 0;

	while (i < count && src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	while (i < count) {
		dest[i] = '\0';
		i++;
	}

	return dest;
}
#endif

/*
 * Provide the historical strncpy() name for the out-of-tree sources.
 * This kernel no longer declares strncpy(), so no redefinition conflict can
 * occur here.
 */
#undef strncpy
#define strncpy nv_compat_strncpy

#endif /* _LINUX_STRNCPY_COMPAT_H */
