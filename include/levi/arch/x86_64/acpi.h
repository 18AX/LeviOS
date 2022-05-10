#ifndef ACPI_HEADER
#define ACPI_HEADER

#include <levi/stivale2.h>
#include <levi/types.h>

#define SDT_SIG_MADT "APIC"
#define SDT_SIG_BGRT "BGRT"
#define SDT_SIG_BERT "BERT"
#define SDT_SIG_CPEP "CPEP"
#define SDT_SIG_DSDT "DSDT"
#define SDT_SIG_ECDT "ECDT"
#define SDT_SIG_EINJ "EINJ"
#define SDT_SIG_ERST "ERST"
#define SDT_SIG_FACP "FACP"
#define SDT_SIG_FACS "FACS"
#define SDT_SIG_HEST "HEST"
#define SDT_SIG_MSCT "MSCT"
#define SDT_SIG_MPST "MPST"
#define SDT_SIG_PMTT "PMTT"
#define SDT_SIG_PSDT "PSDT"
#define SDT_SIG_RASF "RASF"
#define SDT_SIG_RSDT "RSDT"
#define SDT_SIG_SBST "SBST"
#define SDT_SIG_SLIT "SLIT"
#define SDT_SIG_SRAT "SRAT"
#define SDT_SIG_SSDT "SSDT"
#define SDT_SIG_XSDT "XSDT"
#define SDT_SIG_MCFG "MCFG"

struct rsdp
{
    char signature[8];
    u8 checksum;
    char oemid[6];
    u8 revision;
    u32 rsdt_address;
} __attribute__((packed));

struct rsdp2
{
    struct rsdp rsdp;
    u32 len;
    u64 xsdt_address;
    u8 extended_checksum;
    u8 reserved[3];
} __attribute__((packed));

struct acpi_sdt_header
{
    char signature[4];
    u32 len;
    u8 revision;
    u8 checksum;
    char oemid[6];
    char oemid_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
} __attribute__((packed));

struct xsdt
{
    struct acpi_sdt_header header;
    u64 xsdt_table[0];
} __attribute__((packed));

struct rsdt
{
    struct acpi_sdt_header header;
    u32 rsdt_table[0];
} __attribute__((packed));

STATUS acpi_init(struct stivale2_struct *boot_info);

struct acpi_sdt_header *sdt_find(const char sig[4]);

#endif