#include <dlfcn.h>

#include "plugin.h"

PLUGIN_SYMBOL(_nss_dns_getcanonname_r);
PLUGIN_SYMBOL(hesiod_init);

void *bullshit(void) {
	void *x = dlsym;
}
