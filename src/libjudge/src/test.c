#include "libjudge.h"
#include "limit.h"
#include "logger.h"

#include <string.h>
#include <stdlib.h>

struct limit lim;

int main(int argc, char *argv[]) {
	log_level(_DEBUG);
	memset(&lim, 0, sizeof(struct limit));
	if (argc != 4) {
		log_error("Accept 3 arguments");
		exit(1);
	}
	if (sscanf(argv[2], "%ld", &lim.memory_limit) != 1) {
		log_error("Memory should be an integer");
		exit(1);
	}
	lim.memory_limit <<= 10;
	log_debug("memory limit: %ld KiB", lim.memory_limit);
	if (sscanf(argv[3], "%ld", &lim.cpu_limit) != 1) {
		log_error("CPU Time should be an integer");
		exit(1);
	}
	lim.time_limit = lim.cpu_limit + 1000;
	log_debug("cpu limit: %ld seconds", lim.cpu_limit);
	lim.user = 0;
	lim.group = 0;
	struct result *res = libjudge_compile(0, argv[1], "/dev/null", &lim);
	log_info("cpu usage: %ld.%06ld seconds user, %ld.%06ld seconds sys",
		res -> usage.ru_utime.tv_sec, res -> usage.ru_utime.tv_usec,
		res -> usage.ru_stime.tv_sec, res -> usage.ru_stime.tv_usec
	);
	log_info("peak memory usage: %ld KiB", res -> usage.ru_maxrss);
	log_info("exitcode=%d", res->status);
	printf("exitcode=%d\n", res->status);
	exit(!!res->status);
	return 0;
}
