#include <dlfcn.h>

#include "plugin.h"

PLUGIN_FILE(libnss_dns.so.2);
PLUGIN_FILE(libnss_hesiod.so.2);

void *bullshit(void) {
	void *x = dlsym;
}
