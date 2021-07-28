//making the header file for main function

#ifndef SFS_DS_H
    #define SFS_DS_H

    #define fs_type 0x13090D15 //magic number chosen randomly
    #define fs_block_size 512 //assumed
    #define fs_entry_size 64
    #define fs_data_block_count ((fs_entry_size - (16 + 3 * 4)) /4)

    #define default_file ".system_structure"

    typedef unsigned int unit4_t;

    typedef struct super_block {
        unit4_t type; 
        unit4_t b_size;
        unit4_t partition_size;
        unit4_t entry_size;
        unit4_t entry_table_size;
        unit4_t entry_count = entry_table_size / entry_size;
        unit4_t data_block_start;
        unit4_t reserved[fs_block_size /4-8];
    } super_block_t;

    typedef struct file_entry {
        char name[16];
        unit4_t size;
        unit4_t timestamp; //of creation
        unit4_t permissions;
        unit4_t blocks[fs_data_block_count]; //Array of block numbers for up to 9 data blocks. Why 9? To make the complete entry of SIMULA_FS_ENTRY_SIZE (64).
    } file_entry_t;

#endif
