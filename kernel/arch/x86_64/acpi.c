#include <levi/arch/x86_64/acpi.h>
#include <levi/memory/memory.h>
#include <levi/utils/string.h>

static struct rsdp2 *rsdp = NULL;

// Depends on ACPI version
static struct xsdt *xsdt = NULL;
static struct rsdt *rsdt = NULL;

STATUS acpi_init(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_rsdp *rsdp_tag =
        stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_RSDP_ID);

    if (rsdp_tag == NULL)
    {
        return FAILED;
    }

    rsdp = (struct rsdp2 *)rsdp_tag->rsdp;

    if (rsdp->rsdp.revision != 0) // ACPI version > 1
    {
        xsdt = (struct xsdt *)phys_to_hhdm(rsdp->xsdt_address);
    }

    rsdt = (struct rsdt *)phys_to_hhdm((u64)rsdp->rsdp.rsdt_address);

    return SUCCESS;
}

static struct acpi_sdt_header *__xsdt_find(const char sig[4])
{
    u64 xsdt_len =
        (xsdt->header.len - sizeof(struct acpi_sdt_header)) / sizeof(u64);

    for (u64 i = 0; i < xsdt_len; ++i)
    {
        struct acpi_sdt_header *h =
            (struct acpi_sdt_header *)phys_to_hhdm(xsdt->xsdt_table[i]);

        if (strncmp(sig, h->signature, 4) == 0)
        {
            return h;
        }
    }

    return NULL;
}

static struct acpi_sdt_header *__rsdt_find(const char sig[4])
{
    u64 rsdt_len =
        (rsdt->header.len - sizeof(struct acpi_sdt_header)) / sizeof(u32);

    for (u64 i = 0; i < rsdt_len; ++i)
    {
        struct acpi_sdt_header *h =
            (struct acpi_sdt_header *)phys_to_hhdm((u64)rsdt->rsdt_table[i]);

        if (strncmp(sig, h->signature, 4) == 0)
        {
            return h;
        }
    }

    return NULL;
}

struct acpi_sdt_header *sdt_find(const char sig[4])
{
    if (xsdt != NULL)
    {
        return __xsdt_find(sig);
    }

    if (rsdt != NULL)
    {
        return __rsdt_find(sig);
    }

    return NULL;
}