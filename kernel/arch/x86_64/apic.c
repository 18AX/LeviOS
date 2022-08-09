#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/apic.h>
#include <levi/memory/memory.h>
#include <levi/utils/kprintf.h>
#include <levi/utils/string.h>

static volatile u32 *local_apic = NULL;
static volatile u32 *io_apic = NULL;
static struct madt_lapic_proc lapic_cpus[LOCAL_APIC_MAX_CPU];
static u32 lapic_cpus_count = 0;

STATUS apic_init()
{
    struct madt_header *madt = (struct madt_header *)sdt_find(SDT_SIG_MADT);

    if (madt == NULL)
    {
        return FAILED;
    }

    local_apic = (volatile u32 *)phys_to_hhdm(((u64)madt->local_apic_address));

    u8 *madt_records = (u8 *)(madt + 1);

    struct madt_record *record = NULL;

    kprintf("apic pointer %p local apic address %lx\n", madt,
            (u64)madt->local_apic_address);

    while ((record = (struct madt_record *)madt_records)->len != 0)
    {
        switch (record->type)
        {
        case MADT_IO_APIC: {
            struct madt_ioapic *s = (struct madt_ioapic *)record;
            io_apic = (volatile u32 *)phys_to_hhdm((u64)s->io_apic_address);
            break;
        }
        case MADT_LOCAL_APIC_ADDR_OVERRIDE: {
            struct madt_lapic_addr_override *s =
                (struct madt_lapic_addr_override *)record;
            local_apic = (volatile u32 *)phys_to_hhdm(s->address);
            break;
        }
        case MADT_LOCAL_APIC_PROCESSOR: {
            struct madt_lapic_proc *s = (struct madt_lapic_proc *)record;

            memcpy(&lapic_cpus[lapic_cpus_count++], s,
                   sizeof(struct madt_lapic_proc));
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

    kprintf(
        "local apic pointer %p io apic pointer %p, local apic cpu count %u\n",
        local_apic, io_apic, lapic_cpus_count);

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

u32 lapic_cpu_count()
{
    return lapic_cpus_count;
}

STATUS lapic_cpu_info(u8 cpuid, struct madt_lapic_proc *res)
{
    if (cpuid >= lapic_cpus_count)
    {
        return FAILED;
    }

    memcpy(res, &lapic_cpus[cpuid], sizeof(struct madt_lapic_proc));

    return SUCCESS;
}