#include "macros.h"
#include "logger.h"
#include "autoconf/lang.h"
#include <sys/time.h>
#include <sys/resource.h>

void libjudge_init() __attribute__((constructor)) ;
void libjudge_finalize() __attribute__((destructor)) ;

void libjudge_init() {
	log_init();
}

void libjudge_finalize() {
	log_finalize();
}

void libjudge_compile(const int lang, const char *source, const char *binary) {
	if (lang >= LANG_LENGTH) {
		log_fatal("language id %d is longer than installed language (%d)", lang, LANG_LENGTH);
	}
	log_info("language: %s", lang_name[lang]);
}
