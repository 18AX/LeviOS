#include <levi/fs/memfs.h>
#include <levi/modules/module.h>

u32 module_init(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_modules *tag_module =
        stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_MODULES_ID);

    if (tag_module == NULL)
    {
        return 0;
    }

    u32 modules_loaded = 0;

    for (u32 i = 0; i < tag_module->module_count; ++i)
    {
        if (memfile_create(
                tag_module->modules[i].string, tag_module->modules[i].begin,
                tag_module->modules[i].end - tag_module->modules[i].begin, 0x0)
            == SUCCESS)
        {
            ++modules_loaded;
        }
    }

    return modules_loaded;
}