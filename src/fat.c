#include "sd.h" // See this file for function prototype of ata_lba_read()
#include "fat.h"
#include "rprintf.h"
#include "serial.h"
#include "hexdump.h"
#include <stdint.h>
#include "strcomparor.h"

struct boot_sector *bs;
unsigned char bootSector[512]; // Allocate a global array to store boot sector
unsigned char fat_table[8*512];

// start locations
unsigned int volume_start;
unsigned int reserved_start;
unsigned int fat_start;	
unsigned int root_start;
unsigned int data_start;

int fatInit() {
	sd_init();
	sd_readblock(0, bootSector, 1); // Read sector 0 from disk drive into bootSector array
	bs = (struct boot_sector *)bootSector; // Point boot_sector struct to the boot sector so we can read fields
	// verify boot signature and fs type
	if (bs->boot_signature == 0xaa55) {
		esp_printf(putc, "boot signature valid (%x)\n", bs->boot_signature);
	} else {
		esp_printf(putc, "boot signature invalid (expected: 0xaa55, actual: %x)\n", bs->boot_signature);
		return 0;
	}
	if (strcmp("FAT16", bs->fs_type)) {
		esp_printf(putc, "fs type valid (%s)\n", bs->fs_type);
	} else {
		esp_printf(putc, "fs type invalid (expected: 'FAT16', actual: %s)\n", bs->fs_type);
		return 0;
	}

	// initialize start locations
	volume_start = 0;
	reserved_start = volume_start;
	fat_start = reserved_start + bs->num_reserved_sectors;
	root_start = fat_start + (bs->num_fat_tables * bs->num_sectors_per_fat);
	data_start = root_start + ((bs->num_root_dir_entries * 32) / bs->bytes_per_sector);

	// initialize fat_table
	sd_readblock(fat_start, fat_table, 1);	
		
	return 1;	
}

struct file *fatOpen(struct file file, char *path) {
	// parse the name of the file from the path. we need to do some light formatting to make it match the directory entries.
	char filename[9];
	int n = 0; int i = 0;
	if (path[n] == '/') {
		path++;
	}
	while (1) {
		if (path[n] == '/' || path[n] == '.' || i == 8) {
			break;
		} else {
			filename[i++] = path[n];
		}
		n++;
	}
	while (i < 8) {
		filename[i++] = ' ';
	}
	filename[8] = '\0';

	// load the first root directory sector into sector
	unsigned char sector[512];
	sd_readblock(root_start, sector, 1);

	// parse sector, looking for an entry with a matching filename
	struct file *f = NULL;
	for (int i = 0; i < sizeof(sector); i += sizeof(struct root_directory_entry)) {
		// first char in each entry tells us if this entry is the end of the directory (0x00), free (0xE5), or in use (anything else)
		char first = *(sector + i);
		if (first == 0x00) {
			break;
		} else if (first == 0xE5) {
			continue;
		} else {
			// we need to check to see if this directory's name is the same as filename
			struct root_directory_entry *rde = (struct root_directory_entry *)(sector + i);
			char *name = rde->file_name;
			name[8] = '\0';
			if (strcmp(filename, name)) {
				// if it is, this is the file we are looking for, and we can return
				f->next = NULL;
				f->prev = NULL;
				f->rde = *rde;
				f->start_cluster = rde->cluster;
				esp_printf(putc, "start_cluster: %x, cluster: %x", f->start_cluster, rde->cluster);
				break;
			}
		}
	}
	return f;
}

char *fatRead(struct file *f) {
	// table holds the block
	unsigned char table[512];

	// calculate the block's lba
	unsigned int volume_start = 0;
	unsigned int reserved_start = volume_start;
	unsigned int fat_start = reserved_start + bs->num_reserved_sectors;	
	unsigned int root_start = fat_start + (bs->num_fat_tables * bs->num_sectors_per_fat);
	unsigned int data_start = root_start + ((bs->num_root_dir_entries * 32) / bs->bytes_per_sector);
	sd_readblock(data_start + ((f->start_cluster + 1) * 2), table, 1);

	// make table a char * so we can pass it to the caller
	char *data = (char *)table;
	return data;
}
