#ifndef ISO9660_HEADER
#define ISO9660_HEADER

#include <levi/fs/fs.h>
#include <levi/types.h>

#define ISO_BLK_SIZE 2048
#define ISO_BLK_OFFSET(x) (x * ISO_BLK_SIZE)
#define ISO_SIG "CD001"
#define ISO_SIG_LEN 5
#define ISO_PRIMARY_DESC_BLK 0x10
#define ISO_IS_DIR(x) ((x & 1 << 1) != 0)

struct iso_directory
{
    u8 record_len;
    u8 ext_record_len;
    u32 location_blk_lsb;
    u32 location_blk_msb;
    u32 data_len_lsb;
    u32 data_len_msb;
    char date[7];
    u8 file_flags;
    u8 file_unit;
    u8 gap_size;
    u16 vol_seq_lsb;
    u16 vol_seq_msb;
    u8 identifier_len;
    char identifier[0];
} __attribute__((packed));

struct iso_pathtable
{
    u8 identifier_len;
    u8 ext_record_len;
    u32 location_blk;
    u16 parent_id;
    char identifier[0];
} __attribute__((packed));

struct iso_primary_desc
{
    u8 type_code;
    char identifier[5];
    u8 version;
    u8 unused0;
    char system_identifier[32];
    char volume_identifier[32];
    u8 unused1[8];
    u32 volume_space_size_lsb;
    u32 volume_space_size_msb;
    u8 unused2[32];
    u16 volume_set_size_lsb;
    u16 volume_set_size_msb;
    u16 volume_seq_nbr_lsb;
    u16 volume_seq_nbr_msb;
    u16 logical_blk_size_lsb;
    u16 logical_blk_size_msb;
    u32 pathtable_size_lsb;
    u32 pathtable_size_msb;
    u32 pathtable_blk_lsb;
    u32 o_pathtable_blk_lsb;
    u32 pathtable_blk_msb;
    u32 o_pathtable_blk_msb;
    struct iso_directory root_directory;
    u8 padding[34 - sizeof(struct iso_directory)];
    char volume_set_identifier[128];
    char publisher_identifier[128];
    char data_preparer_identifier[128];
    char application_identifier[128];
    char copyright_file_identifier[37];
    char abstract_file_identifier[37];
    char bibliographic_file_identifier[37];
    char volume_creation_date[17];
    char volume_modification_date[17];
    char volume_expiration_date[17];
    char volume_effective_date[17];
    u8 file_structure_version;
} __attribute__((packed));

STATUS iso_mount(u32 fd, const char *name);

STATUS iso_unmount(vfs *iso_vfs);

#endif