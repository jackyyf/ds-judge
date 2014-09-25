#define _BSD_SOURCE /* for wait4 */

#include "macros.h"
#include "logger.h"
#include "lang.h"
#include "limit.h"
#include "libjudge.h"
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static pid_t child_pid;

static void libjudge_init() __attribute__((constructor)) ;
static void libjudge_finalize() __attribute__((destructor)) ;

static void libjudge_init() {
	log_init();
}

static void libjudge_finalize() {
	log_finalize();
}

static void alarm_handler(const int signal) {
	if (rarely(signal != SIGALRM)) {
		log_warn("unexpected signal %d", signal);
		return ;
	}
	log_info("SIGALRM received.");
	if (rarely(kill(child_pid, SIGKILL)) < 0) {
		log_perror("kill child pid=%d", child_pid);
	}
	log_info("pid=%d killed", child_pid);
}

struct result *libjudge_compile(const int lang, const char *source, const char *binary, struct limit *lim) {
	pid_t pid;
	if (rarely(lang >= LANG_LENGTH)) {
		log_fatal("language id %d is longer than installed language (%d)", lang, LANG_LENGTH);
	}
	if (rarely(source == NULL) || rarely(binary == NULL)) {
		log_fatal("source and binary path should be specified");
	}
	if (rarely(lim == NULL)) {
		log_warn("no limitation applied.");
	}
	log_info("language: %s, source: \"%s\", binary: \"%s\"", lang_name[lang], source, binary);
	pid = fork();
	if (rarely(pid < 0)) {
		log_pfatal("vfork failed");
	}
	if (pid != 0) {
		/* parent */
		struct result *res = calloc(sizeof(struct result), 1);
		struct itimerval timer;
		if (likely(lim != NULL && lim -> time_limit > 0)) {
			child_pid = pid; /* use kill in SIGALRM handler */
			if (rarely(signal(SIGALRM, alarm_handler) == SIG_ERR)) {
				log_error("unable to install SIGALRM handler");
			}
			memset(&timer.it_interval, 0, sizeof(struct timeval));
			timer.it_value.tv_sec = lim -> time_limit / 1000;
			timer.it_value.tv_sec = (lim -> time_limit % 1000) * 1000;
			setitimer(ITIMER_REAL, &timer, NULL);
		}
		if (rarely(wait4(pid, &(res->status), 0, &(res->usage)) < 0)) {
			log_perror("wait4 on pid=%u", pid);
		} else {
			log_info("child exited.");
		}
		if (likely(lim != NULL && lim -> time_limit > 0)) {
			memset(&timer, 0, sizeof(struct itimerval));
			/* clear timer */
			setitimer(ITIMER_REAL, &timer, NULL);
		}
		return res;
	} else {
		/* child */
		log_info("child pid=%d", getpid());
		const char *cmd[1024] = { NULL };
		int i;
		if (likely(lim != NULL)) {
			/* force remove some limitation. */
			lim -> fd_limit = LIM_NOLIMIT;
			lim -> fs_limit = LIM_NOLIMIT;
			lim -> chroot = NULL;
			apply_limit(lim);
		}
		for (i = 0; compile_command[lang][i] != NULL; ++ i) {
			if (strcmp(compile_command[lang][i], "{source}") == 0) {
				cmd[i] = source;
			} else if (strcmp(compile_command[lang][i], "{binary}") == 0) {
				cmd[i] = binary;
			} else {
				cmd[i] = compile_command[lang][i];
			}
			log_debug("arg %d: %s", i, cmd[i]);
		}
		log_debug("arg length=%d", i);
		cmd[i] = NULL;
		/* do not check seccomp flag, compilers should not be jailed with seccomp filter. */
		if (rarely(execvp(cmd[0], (char * const *)cmd) < 0)) {
			log_perror("execvp %s failed", cmd[0]);
			exit(253);
		}
		/* exec successfully, should not reach here, at all case. */
		return NULL;
	}
}
