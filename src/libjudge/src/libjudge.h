#ifndef __LIBJUDGE_H
#define __LIBJUDGE_H

#include "limit.h"
#include <sys/time.h>
#include <sys/resource.h>

struct result {
	struct rusage usage;
	int status;
};

struct result *libjudge_compile(const int lang, const char *source, const char *binary, struct limit *lim);
struct result *libjudge_execute(const char *cmd[], struct limit *lim);

#endif /* ifndef __LIBJUDGE_H */
