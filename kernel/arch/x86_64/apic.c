#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/apic.h>
#include <levi/memory/memory.h>
#include <levi/utils/kprintf.h>

static volatile u32 *local_apic = NULL;
static volatile u32 *io_apic = NULL;

STATUS apic_init()
{
    struct acpi_madt *madt = (struct acpi_madt *)sdt_find(SDT_SIG_MADT);

    if (madt == NULL)
    {
        return FAILED;
    }

    local_apic = (volatile u32 *)phys_to_hhdm(((u64)madt->local_apic_address));

    u8 *madt_records = (u8 *)(madt + 1);

    struct acpi_madt_record *record = NULL;

    kprintf("apic pointer %p local apic address %lx\n", madt,
            (u64)madt->local_apic_address);

    while ((record = (struct acpi_madt_record *)madt_records)->len != 0)
    {
        kprintf("recored type : %d len %d\n", record->type, record->len);
        switch (record->type)
        {
        case MADT_IO_APIC: {
            struct acpi_madt_io_apic *s = (struct acpi_madt_io_apic *)record;
            io_apic = (volatile u32 *)phys_to_hhdm((u64)s->io_apic_address);
            break;
        }
        case MADT_LOCAL_APIC_ADDR_OVERRIDE: {
            struct acpi_madt_local_apic_addr_override *s =
                (struct acpi_madt_local_apic_addr_override *)record;
            local_apic = (volatile u32 *)phys_to_hhdm(s->address);
            break;
        }
        default:
            break;
        }

        madt_records += record->len;
    }

    if (local_apic == NULL || io_apic == NULL)
    {
        return FAILED;
    }

    kprintf("local apic pointer %p io apic pointer %p\n", local_apic, io_apic);

    return SUCCESS;
}

void lapic_write(u32 reg, u32 data)
{
    local_apic[reg] = data;
}

u32 lapic_read(u32 reg)
{
    return local_apic[reg];
}

void ioapic_write(u32 reg, u32 data)
{
    io_apic[reg] = data;
}

u32 ioapic_read(u32 reg)
{
    return io_apic[reg];
}