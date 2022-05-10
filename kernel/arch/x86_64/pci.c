#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/pci.h>

STATUS pci_init(void)
{
    struct acpi_sdt_header *mcfg = sdt_find(SDT_SIG_MCFG);

    if (mcfg == NULL)
    {
        return FAILED;
    }

    return SUCCESS;
}