#include "libjudge.h"
#include "limit.h"
#include "logger.h"

#include <string.h>

struct limit lim;

int main(void) {
	log_level(_DEBUG);
	memset(&lim, 0, sizeof(struct limit));
	lim.memory_limit = 16777216;
	lim.user = -1;
	lim.group = -1;
	struct result *res = libjudge_compile(0, "/tmp/test.c", "/tmp/test", &lim);
	log_info("exitcode=%d", res->status);
	return 0;
}
