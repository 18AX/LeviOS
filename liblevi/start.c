#include <syscall.h>
#include <types.h>

extern int main(void);

void __start_main(void)
{
    u64 ret = main();
    exit(ret);
}