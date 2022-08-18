#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/apic.h>
#include <levi/arch/x86_64/cpuregs.h>
#include <levi/arch/x86_64/msr.h>
#include <levi/memory/memory.h>
#include <levi/utils/kprintf.h>
#include <levi/utils/string.h>

static volatile u32 *lapic_ptr = NULL;

static volatile u32 *ioapic_ptr = NULL;
static u32 ioapic_global_interrupt_base = 0;

static struct madt_ioapic_interrupt_override
    interrupts_override[IO_REDIRECTION_TABLE_SIZE] = { 0 };

static struct madt_lapic_proc lapic_cpus[LOCAL_APIC_MAX_CPU] = { 0 };
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

    lapic_ptr = (volatile u32 *)phys_to_hhdm(((u64)madt->local_apic_address));

    u8 *madt_records = (u8 *)(madt + 1);

    struct madt_record *record = NULL;

    /** We are looking for the address of the io apic, the local apic, lapic
     * cpus and apic irq overrides. **/
    while ((record = (struct madt_record *)madt_records)->len != 0)
    {
        switch (record->type)
        {
        case MADT_IO_APIC: {
            struct madt_ioapic *s = (struct madt_ioapic *)record;

            ioapic_ptr = (volatile u32 *)phys_to_hhdm((u64)s->io_apic_address);
            ioapic_global_interrupt_base = s->global_system_interrupt_base;
            break;
        }
        case MADT_LOCAL_APIC_ADDR_OVERRIDE: {
            struct madt_lapic_addr_override *s =
                (struct madt_lapic_addr_override *)record;
            lapic_ptr = (volatile u32 *)phys_to_hhdm(s->address);
            break;
        }
        case MADT_LOCAL_APIC_PROCESSOR: {
            struct madt_lapic_proc *s = (struct madt_lapic_proc *)record;

            if (lapic_cpus_count + 1 < LOCAL_APIC_MAX_CPU)
            {
                memcpy(&lapic_cpus[lapic_cpus_count++], s,
                       sizeof(struct madt_lapic_proc));
            }
            break;
        }
        case MADT_IO_APIC_INTERRUPT_OVERRIDE: {
            struct madt_ioapic_interrupt_override *s =
                (struct madt_ioapic_interrupt_override *)record;

            memcpy(&interrupts_override[s->irq_source], s,
                   sizeof(struct madt_ioapic_interrupt_override));
            break;
        }
        default:
            break;
        }

        madt_records += record->len;
    }

    if (lapic_ptr == NULL || ioapic_ptr == NULL)
    {
        return FAILED;
    }

    kprintf("local apic pointer %p io apic pointer %p, local apic cpu count %u "
            "global base %x\n",
            lapic_ptr, ioapic_ptr, lapic_cpus_count,
            ioapic_global_interrupt_base);

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
    lapic_ptr[reg] = data;
}

u32 lapic_read(u32 reg)
{
    return lapic_ptr[reg];
}

void ioapic_write(u32 reg, u32 data)
{
    ioapic_ptr[0] = reg;
    ioapic_ptr[4] = data;
}

u32 ioapic_read(u32 reg)
{
    ioapic_ptr[0] = reg;
    return ioapic_ptr[4];
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

STATUS ioapic_irq_set(u8 cpuid, u8 apic_pin, u8 interrupts_vector, u32 flags)
{
    if (apic_pin >= IO_REDIRECTION_TABLE_SIZE || cpuid >= lapic_cpus_count)
    {
        return FAILED;
    }

    struct apic_redirection redirection = { 0 };
    redirection.destination_field = lapic_cpus[cpuid].apic_id;
    redirection.interrupt_vector = interrupts_vector;

    if ((flags & IOAPIC_TRIGGER_HIGH) != 0)
    {
        redirection.polarity = 0;
    }

    if ((flags & IOAPIC_TRIGGER_LOW) != 0)
    {
        redirection.polarity = 1;
    }

    if ((flags & IOAPIC_TRIGGER_LEVEL) != 0)
    {
        redirection.trigger_mode = 1;
    }

    if ((flags & IOAPIC_TRIGGER_EDGE) != 0)
    {
        redirection.trigger_mode = 0;
    }

    if ((flags & IOAPIC_INTERRUPT_MASK_SET) != 0)
    {
        redirection.interrupt_mask = 1;
    }

    /** x << 1 = x * 2 the structure needs to be written on two entries **/
    u32 offset = (IO_REDIRECTION_TABLE_BASE
                  + interrupts_override[apic_pin].global_system_interrupt
                  - ioapic_global_interrupt_base)
        << 1;

    ioapic_write(offset, redirection.low);
    ioapic_write(offset + 1, redirection.high);

    return SUCCESS;
}

void lapic_eoi(void)
{
    lapic_ptr[LAPIC_EOI] = 0x0;
}