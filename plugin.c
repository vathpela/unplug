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
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <link.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "plugin.h"

int
plugin_callback(char *filename, char *liblist, size_t size)
{
	char *str = liblist;

	for (off_t o = 0; o < size; o++) {
		size_t l = strlen(str);

		void *dl = dlopen(str, RTLD_LOCAL|RTLD_NOW);
		if (dl == NULL) {
			fprintf(stderr, "dlerror: %s\n", dlerror());
			goto next;
		}

		struct link_map *lm = NULL;

		int rc = dlinfo(dl, RTLD_DI_LINKMAP, &lm);
		if (rc < 0) {
			fprintf(stderr, "dlerror: %s\n", dlerror());
			goto next;
		}

		while (lm) {
			printf("%s\n", lm->l_name);

#if 0
			Dl_info di;
			memset(&di, '\0', sizeof (di));

			rc = dladdr((void *)lm->l_addr, &di);
			if (rc == 0)
				goto next_map;
			printf("%s\n", di.dli_fname);
next_map:
#endif
			lm = lm->l_next;
		}

		//printf("%s\n", dlerror());

next:
		str += l + 1;
		o += l;
	}

	return 0;
}
