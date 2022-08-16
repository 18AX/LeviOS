#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/apic.h>
#include <levi/arch/x86_64/cpuregs.h>
#include <levi/arch/x86_64/msr.h>
#include <levi/memory/memory.h>
#include <levi/utils/kprintf.h>
#include <levi/utils/string.h>

static volatile u32 *local_apic = NULL;
static volatile u32 *io_apic = NULL;
static struct madt_lapic_proc lapic_cpus[LOCAL_APIC_MAX_CPU];
static u32 lapic_cpus_count = 0;

static STATUS apic_check_lapic(void)
{
    u32 eax, ebx, ecx, edx = 0;

    cpuid(0, &eax, &ebx, &ecx, &edx);

    if ((edx & CPUID_LAPIC_FEATURE) == 0)
    {
        return FAILED;
    }

    return SUCCESS;
}

STATUS apic_init()
{
    /** Checking for the presence of local apic **/
    if (apic_check_lapic() == FAILED)
    {
        return SUCCESS;
    }

    struct madt_header *madt = (struct madt_header *)sdt_find(SDT_SIG_MADT);

    if (madt == NULL)
    {
        return FAILED;
    }

    local_apic = (volatile u32 *)phys_to_hhdm(((u64)madt->local_apic_address));

    u8 *madt_records = (u8 *)(madt + 1);

    struct madt_record *record = NULL;

    /** We are looking for the address of the io apic and the local apic. **/
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

            if (lapic_cpus_count + 1 < LOCAL_APIC_MAX_CPU)
            {
                memcpy(&lapic_cpus[lapic_cpus_count++], s,
                       sizeof(struct madt_lapic_proc));
            }
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

void apic_enable()
{
    /** bit 11 is APIC global enable/disable and we don't want to override the
     * base address of the local apic **/
    u64 msr_value = rdmsr64(MSR_IA32_APIC_BASE);
    msr_value |= MSR_APIC_BASE_ENABLE;
    wrmsr64(MSR_IA32_APIC_BASE, msr_value);

    /** Bit 8 of the spurious interrupt vector can enable/disable apic **/
    u64 spurious_reg_value = lapic_read(LAPIC_SPURIOUS_INTERRUPT_VECTOR);
    spurious_reg_value |= SPURIOUS_VECTOR_APIC_ENABLE;
    lapic_write(LAPIC_SPURIOUS_INTERRUPT_VECTOR, spurious_reg_value);
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

void lapic_eoi(void)
{
    local_apic[LAPIC_EOI] = 0x0;
}