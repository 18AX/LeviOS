#ifndef MODULE_HEADER
#define MODULE_HEADER

#include <levi/stivale2.h>
#include <levi/types.h>

/**
 * @brief create a device for each modules
 *
 * @param boot_info
 * @return u32 number of modules loaded
 */
u32 module_init(struct stivale2_struct *boot_info);

#endif