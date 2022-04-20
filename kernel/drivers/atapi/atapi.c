#include "levi/drivers/atapi/atapi.h"

#include "levi/fs/fs.h"

static file_t *__open(const char *name, u32 flags);

static struct vfs_operation atapi_operation = { .mkdir = NULL,
                                                .rmdir = NULL,
                                                .open = NULL,
                                                .write = NULL,
                                                .destroy_file = NULL,
                                                .flush = NULL,
                                                .read = NULL,
                                                .lseek = NULL };

STATUS atapi_init()
{}