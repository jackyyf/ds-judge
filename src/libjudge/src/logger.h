#ifndef __LOGGER_H
#define __LOGGER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "macros.h"

extern int _log_level;
extern FILE* _log_file;

#define _DEBUG		4
#define _INFO		3
#define _WARN		2
#define _ERROR		1
/* Note: 
 *   Supress level supress all logging output except fatal errors.
 *   Fatal errors still generates output, no matter which level is set.
 */
#define _SUPRESS	0 

#define log_debug(fmt, ...) do { \
	if (_log_level < _DEBUG) break; \
	if (_log_file == NULL) break; \
	if (isatty(fileno(_log_file))) { \
		fprintf(_log_file, STYLE_BRIGHT FORE_GREEN "[DEBUG %s:%d] " fmt STYLE_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} else { \
		fprintf(_log_file, "[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} \
} while(0)

#define log_info(fmt, ...) do { \
	if (_log_level < _INFO) break; \
	if (_log_file == NULL) break; \
	if (isatty(fileno(_log_file))) { \
		fprintf(_log_file, STYLE_BRIGHT FORE_BLUE "[INFO %s:%d] " fmt STYLE_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} else { \
		fprintf(_log_file, "[INFO %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} \
} while(0)

#define log_warn(fmt, ...) do { \
	if (_log_level < _WARN) break; \
	if (_log_file == NULL) break; \
	if (isatty(fileno(_log_file))) { \
		fprintf(_log_file, FORE_YELLOW "[WARN %s:%d] " fmt STYLE_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} else { \
		fprintf(_log_file, "[WARN %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} \
} while(0)

#define log_error(fmt, ...) do { \
	if (_log_level < _ERROR) break; \
	if (_log_file == NULL) break; \
	if (isatty(fileno(_log_file))) { \
		fprintf(_log_file, FORE_RED "[ERROR %s:%d] " fmt STYLE_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} else { \
		fprintf(_log_file, "[ERROR %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} \
} while(0)

#define log_fatal(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, STYLE_BRIGHT FORE_RED "[FATAL %s:%d] " fmt STYLE_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
		} else { \
			fprintf(_log_file, "[FATAL %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
		} \
	} \
	exit(254); \
} while(0)

#define log_pdebug(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, STYLE_BRIGHT FORE_GREEN "[DEBUG %s:%d] [errno %d] " fmt ": %s" STYLE_RESET "\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} else { \
			fprintf(_log_file, "[DEBUG %s:%d] [errno %d] " fmt ": %s\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} \
	} \
} while(0)

#define log_pinfo(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, STYLE_BRIGHT FORE_BLUE "[INFO %s:%d] [errno %d] " fmt ": %s" STYLE_RESET "\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} else { \
			fprintf(_log_file, "[INFO %s:%d] [errno %d] " fmt ": %s\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} \
	} \
} while(0)

#define log_pwarn(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, FORE_YELLOW "[WARN %s:%d] [errno %d] " fmt ": %s" STYLE_RESET "\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} else { \
			fprintf(_log_file, "[WARN %s:%d] [errno %d] " fmt ": %s\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} \
	} \
} while(0)

#define log_perror(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, FORE_RED "[ERROR %s:%d] [errno %d] " fmt ": %s" STYLE_RESET "\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} else { \
			fprintf(_log_file, "[ERROR %s:%d] [errno %d] " fmt ": %s\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} \
	} \
} while(0)

#define log_pfatal(fmt, ...) do { \
	if (_log_file != NULL) { \
		if (isatty(fileno(_log_file))) { \
			fprintf(_log_file, STYLE_BRIGHT FORE_RED "[FATAL %s:%d] [errno %d] " fmt ": %s" STYLE_RESET "\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} else { \
			fprintf(_log_file, "[FATAL %s:%d] [errno %d] " fmt ": %s\n", __FILE__, __LINE__, errno, ##__VA_ARGS__, strerror(errno)); \
		} \
	} \
	exit(253); \
} while(0)

int log_fd(const int);
int log_file(const char *);
int log_level(const int);
int log_init();
int log_reset();
int log_finalize();

#endif /* ifndef __LOGGER_H */
