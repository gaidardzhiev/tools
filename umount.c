#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define NMNT  512
#define SIZE  1024

typedef struct {
	char mp[SIZE];
	char d[SIZE];
} e;

e t[NMNT];

int main(int c, char** argv) {
	e* current_entry;
	char* path_ptr;
	int mount_file;

	sync();
	mount_file = open("/etc/mtab", O_RDONLY);
	read(mount_file, (char*)t, NMNT * sizeof(e));

	if (c != 2) {
		fprintf(stderr, "usage: %s <mount_point>\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (umount(argv[1]) < 0) {
		perror("failed to unmount...");
		return EXIT_FAILURE;
	}

	path_ptr = argv[1];
	while (*path_ptr++);
	path_ptr--;
	while (path_ptr > argv[1] && *--path_ptr == '/') *path_ptr = '\0';
	while (path_ptr > argv[1] && *--path_ptr != '/');
	if (*path_ptr == '/') path_ptr++;
	argv[1] = path_ptr;

	for (current_entry = t; current_entry < &t[NMNT]; current_entry++) {
		path_ptr = argv[1];
		char* device_ptr = current_entry->d;

		while (*path_ptr++ == *device_ptr) {
			if (*device_ptr++ == '\0') {
				for (path_ptr = current_entry->mp; path_ptr < &current_entry->mp[SIZE];) {
					*path_ptr++ = '\0';
				}
				current_entry = &t[NMNT];
				while ((--current_entry)->mp[0] == '\0');
				mount_file = creat("/etc/mtab", 0644);
				write(mount_file, (char*)t, (current_entry - t + 1) * sizeof(e));
				return EXIT_SUCCESS;
			}
		}
	}

	fprintf(stderr, "%s not found in mount table\n", argv[1]);
	return EXIT_FAILURE;
}
