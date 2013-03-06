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
#ifndef PLUGIN_H
#define PLUGIN_H 1

#define _PLUGIN_CONCAT(foo, bar) foo ## bar
#define _PLUGIN_STRINGIFY_2(foo) #foo
#define _PLUGIN_STRINGIFY(foo) _PLUGIN_STRINGIFY_2(foo)

#define _PLUGIN_DIRECTORY(counter, dirname) \
	static const char \
		_PLUGIN_CONCAT(_plugin_directory_name_, count) [] \
		__attribute ((section (".plugin.directories"))) = \
		#dirname
#define PLUGIN_DIRECTORY(dirname) \
	_PLUGIN_DIRECTORY(__COUNTER__, dirname)

#define _PLUGIN_FILE(count, filename) \
	static const char \
		_PLUGIN_CONCAT(_plugin_requirement_, count)  [] \
		__attribute__ ((section (".plugin.filenames"))) = \
		#filename ; \
		extern void * plugin_list_start __attribute__ ((weak, alias ("_plugin_requirement_0")))
#define PLUGIN_FILE(filename) \
	_PLUGIN_FILE(__COUNTER__, filename)

typedef int (*plugin_callback_fn)(char *filename, char *liblist, size_t size);

#endif /* PLUGIN_H */
