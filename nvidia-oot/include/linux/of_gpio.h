/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compatibility shim for the removed legacy device-tree GPIO lookup API.
 *
 * The kernel's <linux/of_gpio.h> (of_get_named_gpio(), of_get_gpio(),
 * of_get_named_gpio_flags(), of_get_gpio_flags()) was removed in
 * commit 40fc56ee608cdb ("gpiolib: of: remove of_get_gpio[_flags]() and
 * of_get_named_gpio_flags()") which landed in Linux 6.2. These helpers are
 * still used by the out-of-tree Tegra drivers in this tree.
 *
 * This header re-implements them on top of the modern gpiod firmware-node
 * API (fwnode_gpiod_get_index() + desc_to_gpio()) and returns the legacy
 * integer GPIO number, matching the behaviour the drivers expect.
 *
 * Only the public-source out-of-tree include path is affected; the upstream
 * kernel is not modified.
 */
#ifndef _LINUX_OF_GPIO_COMPAT_H
#define _LINUX_OF_GPIO_COMPAT_H

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_address.h>

#if !defined(NV_OF_GET_NAME_GPIO_PRESENT)

/*
 * Compatibility definition of the GPIO flag that used to be exported by the
 * removed <linux/of_gpio.h>.
 */
#ifndef GPIOF_ACTIVE_LOW
#define GPIOF_ACTIVE_LOW	1
#endif

struct device_node;

/*
 * Return the GPIO number referenced by the named GPIO property @name on
 * @np. Returns a non-negative GPIO number on success or a negative error
 * code (-ENOENT if the property is absent, -ENODATA if the property is
 * present but carries no usable value), matching the removed API.
 */
static inline int of_get_named_gpio(struct device_node *np,
				    const char *name, int index)
{
	struct gpio_desc *desc;
	int gpio;

	if (!np)
		return -ENOENT;

	desc = fwnode_gpiod_get_index(&np->fwnode, name, index,
				      GPIOD_ASIS, NULL);
	if (IS_ERR(desc))
		return PTR_ERR(desc);

	gpio = desc_to_gpio(desc);
	gpiod_put(desc);

	return gpio;
}

/*
 * Return the GPIO number referenced by the GPIO property at index @index
 * on @np (the "gpios" property).
 */
static inline int of_get_gpio(struct device_node *np, int index)
{
	return of_get_named_gpio(np, NULL, index);
}

/*
 * Return the GPIO number referenced by the named GPIO property @name on @np
 * and, if @flags is non-NULL, the GPIO flags (GPIOF_ACTIVE_LOW when the
 * device-tree specifier carries the "active-low" flag). Matches the removed
 * of_get_named_gpio_flags() API.
 */
static inline int of_get_named_gpio_flags(struct device_node *np,
					  const char *propname, int index,
					  u32 *flags)
{
	struct of_phandle_args gspec;
	int gpio;

	if (!np)
		return -ENOENT;
	if (flags)
		*flags = 0;

	gpio = of_get_named_gpio(np, propname, index);
	if (gpio < 0)
		return gpio;

	/*
	 * Recover the "active-low" flag from the specifier cells of the
	 * property. The flag cell (if present) is the last cell of the
	 * specifier; bit 0 (GPIO_ACTIVE_LOW) marks it active-low.
	 */
	if (of_parse_phandle_with_args(np, propname, "#gpio-cells", index,
				       &gspec) == 0 &&
	    gspec.args_count > 0 &&
	    (gspec.args[gspec.args_count - 1] & 1))
		*flags = GPIOF_ACTIVE_LOW;

	return gpio;
}

/*
 * Return the GPIO number referenced by the GPIO property at index @index on
 * @np and its flags.
 */
static inline int of_get_gpio_flags(struct device_node *np, int index,
				    u32 *flags)
{
	return of_get_named_gpio_flags(np, NULL, index, flags);
}

#endif /* !defined(NV_OF_GET_NAME_GPIO_PRESENT) */

#endif /* _LINUX_OF_GPIO_COMPAT_H */
