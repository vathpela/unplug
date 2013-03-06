/*
 * Copyright 2013 Red Hat, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author(s): Peter Jones <pjones@redhat.com>
 */

#include <dlfcn.h>
#include <elf.h>
#include <elfutils/libebl.h>
#include <fcntl.h>
#include <gelf.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "plugin.h"

static int
get_callback_function(char *filename, plugin_callback_fn *cbr)
{
	void *dl = dlopen(filename, RTLD_LAZY|RTLD_DEEPBIND|RTLD_NODELETE);

	if (!dl) {
		fprintf(stderr, "Could not open library: %s\n", dlerror());
		return -1;
	}

	void *cb = dlsym(dl, "plugin_callback");
	if (!cb) {
		fprintf(stderr, "Could not find plugin_callback: %s\n", dlerror());
		dlclose(dl);
		return -1;
	}

	*cbr = cb;

	dlclose(dl);

	return 0;
}

static int
dump_requires(char *filename, int fd)
{
	elf_version(EV_CURRENT);

	Elf *elf = elf_begin(fd, ELF_C_READ_MMAP, NULL);
	if (!elf) {
		fprintf(stderr, "Could not read \"%s\": %s\n", filename,
			elf_errmsg(elf_errno()));
		return -1;
	}

	size_t shstrndx = -1;
	int rc = elf_getshdrstrndx(elf, &shstrndx);

	int scn_no = 0;
	Elf_Scn *scn = NULL;
	do {
		scn = elf_getscn(elf, ++scn_no);
		if (!scn)
			break;

		int rc;

		GElf_Shdr *shdrp, shdr;
		shdrp = gelf_getshdr(scn, &shdr);

		if (!shdrp) {
			fprintf(stderr, "Malformed ELF file \"%s\": %s\n",
				filename, elf_errmsg(elf_errno()));
			elf_end(elf);
			return -1;
		}

		char *name = elf_strptr(elf, shstrndx, shdr.sh_name);
		if (!strcmp(name, ".plugin.filenames")) {
			Elf_Data *datap;

			datap = elf_rawdata(scn, NULL);
			if (!datap) {
				fprintf(stderr, "ELF Error: %s\n",
					elf_errmsg(elf_errno()));
				elf_end(elf);
				return -1;
			}

			size_t fsize;

			char *rawfile = elf_rawfile(elf, &fsize);

			char *scndata = rawfile + shdr.sh_offset + datap->d_off;

			plugin_callback_fn cb;
			rc = get_callback_function(filename, &cb);
			if (rc < 0) {
				fprintf(stderr, "could not get callback\n");
				elf_end(elf);
				return rc;
			}

			rc = cb(filename, scndata, datap->d_size);
			elf_end(elf);
			return rc;
		}
	} while (scn != NULL);
}

int
main(int argc, char *argv[])
{
	int rc = 0;
	int ret = 0;
	for (int i = 0; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "Could not open input file \"%s\": %m\n");
			exit(1);
		}
		rc = dump_requires(argv[i], fd);
		if (rc < 0)
			ret = -rc;
		close(fd);
	}
	return ret;
}
