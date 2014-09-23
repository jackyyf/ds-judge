#include "logger.h"

int _log_level = 2; /* Default to warning. */
FILE *_log_file = NULL;

int log_init() {
	if (rarely(_log_file != NULL)) {
		log_warn("Logger already initialized.");
		return -1;
	}
	int new_fd = dup(fileno(stderr));
	if (rarely(new_fd < 0)) {
		return -1;
	}
	_log_file = fdopen(new_fd, "a");
	if (rarely(_log_file == NULL)) {
		return -1;
	}
	log_info("Logger initialized.");
	return 0;
}

int log_fd(const int fd) {
	int new_fd = dup(fd);
	if (rarely(new_fd < 0)) {
		log_error("Unable to dup fd %d", fd);
		return -1;
	}
	FILE *new_file = fdopen(new_fd, "a");
	if (rarely(new_file == NULL)) {
		log_error("Dup ok, but open as file failed.");
		return -1;
	}
	fclose(_log_file);
	_log_file = new_file;
	return 0;
}

int log_file(const char *filename) {
	FILE *new_file = fopen(filename, "a");
	if (rarely(new_file == NULL)) {
		log_error("Unable to open %s", filename);
		return -1;
	}
	fclose(_log_file);
	_log_file = new_file;
	return 0;
}

int log_reset() {
	int stderr_fileno = fileno(stderr);
	if (rarely(stderr_fileno < 0)) {
		log_error("Unable to get stderr fileno (stderr closed?)");
		return -1;
	}
	return log_fd(stderr_fileno);
}

int log_level(const int level) {
	int old_level = _log_level;
	_log_level = level;
	return old_level;
}

int log_finalize() {
	fclose(_log_file);
	_log_file = NULL;
	return 0;
}
