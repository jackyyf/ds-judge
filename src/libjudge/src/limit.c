#include "limit.h"
#include "logger.h"
#include <sys/stat.h>
#include <fcntl.h>

int apply_limit(struct limit *limit) {
	struct rlimit lim = {
		.rlim_cur = 0L,
		.rlim_max = 0L
	};
	if (likely(limit -> close_fd)) {
		/* first, lets get fd limit */
		int i;
		int maxfd = 1024;
		getrlimit(RLIMIT_NOFILE, &lim);
		if (likely(lim.rlim_cur != RLIM_INFINITY)) {
			maxfd = lim.rlim_cur;
			log_warn("no fd limit for current process, assume maxfd is 1024.");
		}
		log_debug("max_fd: %d", maxfd);
		/* keep 0 ~ 2 open (stdin, stdout, stderr) */
		for(i = 3; i < maxfd; ++ i) close(i);
	}
	if (likely(limit -> memory_limit > 0)) {
		lim.rlim_cur = limit ->memory_limit + 16777216; /* since we are using virtual page size, some overhead is required. */
		lim.rlim_max = lim.rlim_cur + 16777216;
		log_debug("page size limit: soft=%ld bytes, hard=%ld bytes", lim.rlim_cur, lim.rlim_max);
		if (rarely(setrlimit(RLIMIT_AS, &lim) < 0)) {
			log_perror("apply memory limit");
		} else {
			log_info("memory limit ok.");
		}
	}
	if (likely(limit -> cpu_limit > 0)) {
		lim.rlim_cur = (limit -> cpu_limit + 999) / 1000; /* percision is 1 second. add some small overhead */
		lim.rlim_max = lim.rlim_cur + 1;
		log_debug("cpu limit: soft=%ld seconds, hard=%ld seconds", lim.rlim_cur, lim.rlim_max);
		if (rarely(setrlimit(RLIMIT_CPU, &lim) < 0)) {
			log_perror("apply cpu limit");
		} else {
			log_info("cpu limit ok.");
		}
	}

	if (limit -> stdin_file != NULL) {
		int fd = -1;
		if (startswith(limit -> stdin_file, "file:")) {
			log_debug("redirect stdin to file: %s", limit -> stdin_file + 5);
			fd = open(limit -> stdin_file + 5, O_RDONLY); /* remove file: prefix */
			if (rarely(fd < 0)) {
				log_perror("open %s", limit -> stdin_file + 5);
				_exit(254);
			}
			log_debug("open file %s, fd=%d", limit -> stdin_file, fd);
		} else if (startswith(limit -> stdin_file, "fd:")) {
			log_debug("redirect stdin to fd: %s", limit -> stdin_file + 3);
			if (rarely(sscanf(limit -> stdin_file + 3, "%d", &fd) != 1)) {
				fd = -1;
				log_error("invalid fd number: %s", limit -> stdin_file + 3);
			}
		}
		if (likely(fd >= 0)) {
			if (rarely(dup2(fd, 0) < 0)) {
				log_perror("redirect stdin to fd %d", fd);
				_exit(254);
			}
			if (likely(fd != 0)) /* rare case that fd is 0 */
				close(fd);
		}
	}

	if (limit -> stdout_file != NULL) {
		int fd = -1;
		if (startswith(limit -> stdout_file, "file:")) {
			log_debug("redirect stdout to file: %s", limit -> stdout_file + 5);
			fd = open(limit -> stdout_file + 5, O_WRONLY|O_CREAT, S_IRWXU); /* remove file: prefix, permission 700 */
			if (rarely(fd < 0)) {
				log_perror("open %s", limit -> stdout_file + 5);
				_exit(254);
			}
			log_debug("open file %s, fd=%d", limit -> stdout_file, fd);
		} else if (startswith(limit -> stdout_file, "fd:")) {
			log_debug("redirect stdout to fd: %s", limit -> stdout_file + 3);
			if (rarely(sscanf(limit -> stdout_file + 3, "%d", &fd) != 1)) {
				fd = -1;
				log_error("invalid fd number: %s", limit -> stdout_file + 3);
			}
		}
		if (likely(fd >= 0)) {
			if (rarely(dup2(fd, 1) < 0)) {
				log_perror("redirect stdout to fd %d", fd);
				_exit(254);
			}
			if (likely(fd != 0)) /* rare case that fd is 0 */
				close(fd);
		}
	}

	if (limit -> stderr_file != NULL) {
		int fd = -1;
		if (startswith(limit -> stderr_file, "file:")) {
			log_debug("redirect stderr to file: %s", limit -> stderr_file + 5);
			fd = open(limit -> stderr_file + 5, O_WRONLY|O_CREAT, S_IRWXU); /* remove file: prefix, permission 700 */
			if (rarely(fd < 0)) {
				log_perror("open %s", limit -> stderr_file + 5);
				_exit(254);
			}
			log_debug("open file %s, fd=%d", limit -> stderr_file, fd);
		} else if (startswith(limit -> stderr_file, "fd:")) {
			log_debug("redirect stderr to fd: %s", limit -> stderr_file + 3);
			if (rarely(sscanf(limit -> stderr_file + 3, "%d", &fd) != 1)) {
				fd = -1;
				log_error("invalid fd number: %s", limit -> stderr_file + 3);
			}
		}
		if (likely(fd >= 0)) {
			if (rarely(dup2(fd, 2) < 0)) {
				log_perror("redirect stderr to fd %d", fd);
				_exit(254);
			}
			log_level(_SUPRESS);
			if (likely(fd != 0)) /* rare case that fd is 0 */
				close(fd);
		}
	}

	if (likely(limit -> fd_limit > 0)) {
		if (rarely(limit -> fd_limit < 3)) { /* no enough file descriptor for stdin, stdout and stderr */
			log_warn("your fd_limit %ld is smaller than 3, which is not enough for stdin, stdout and stderr.", limit -> fd_limit);
		}
		lim.rlim_cur = lim.rlim_max = limit -> fd_limit;
		log_debug("fd limit: soft=%ld, hard=%ld", lim.rlim_cur, lim.rlim_max);
		if (rarely(setrlimit(RLIMIT_NOFILE, &lim) < 0)) {
			log_perror("apply fd number limit");
		} else {
			log_info("fd number limit ok.");
		}
	}
	if (likely(limit -> fs_limit > 0)) {
		lim.rlim_cur = lim.rlim_max = limit -> fs_limit;
		log_debug("filesize limit: soft=%ld bytes, hard=%ld bytes", lim.rlim_cur, lim.rlim_max);
		if (rarely(setrlimit(RLIMIT_FSIZE, &lim) < 0)) {
			log_perror("apply filesize limit");
		} else {
			log_info("filesize limit ok.");
		}
	}
	if (getuid() != 0) { /* i'm not root! */
		if (limit -> user > 0 && limit -> user != getuid()) {
			log_warn("unable to setuid %d, i'm not root.", limit -> user);
		} else {
			log_info("we are currently uid %d, not changing.", getuid());
		}
		if (limit -> group > 0 && limit -> group != getgid()) {
			log_warn("unable to setgid %d, i'm not root.", limit -> group);
		} else {
			log_info("we are currently gid %d, not changing.", getgid());
		}
		if (limit -> chroot != NULL) {
			log_warn("unable to chroot to %s, i'm not root.", limit -> chroot);
		}
	} else { /* yes, i have root privilege. */
		if (limit -> user == 0) {
			log_warn("run program with root is VERY dangerous!");
		} else {
			if (rarely(setuid(limit -> user) < 0)) {
				log_perror("setuid to %d", limit -> user);
			} else {
				log_info("setuid ok");
			}
		}
		if (limit -> group == 0) {
			log_warn("run program with root group is dangerous!");
		} else {
			if (rarely(setgid(limit -> group) < 0)) {
				log_perror("setgid to %d", limit -> group);
			} else {
				log_info("setgid ok");
			}
		}
		if (limit -> chroot != NULL) {
			if (rarely(chroot(limit -> chroot) < 0)) {
				log_perror("chroot to %s", limit -> chroot);
			} else {
				log_info("chroot ok");
			}
		}
	}
	return 0;
}
