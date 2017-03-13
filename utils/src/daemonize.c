#include "pigeon.h"
#include "utils.h"
#include "error_process.h"

void daemonize(const char *cmd) {
    int                  i, fd0, fd1, fd2;
    pid_t                pid;
    struct rlimit        rl;
    struct sigaction     sa;
    
    /* clear file creation mask */
    umask(0);

    if (getuid() != 0) {
        err_quit("%s: Permission denied", cmd);
    }

    /* get maximum number of file descriptors */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        err_quit("%s: can't get file limit", cmd);
    }

    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    } else if (pid != 0) {
        exit(0);
    }
    /* become session leader */
    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        err_quit("%s: can't ignore SIGHUP", cmd);
    }

    if ((pid = fork()) < 0) {
        err_quit("%s: can't fork", cmd);
    } else if (pid != 0) {
        exit(0);
    }

    if (chdir("/") < 0) {
        err_quit("%s, can't change directory to /", cmd);
    }

    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for (i = 0; i < rl.rlim_max; ++i) {
        close(i);
    }

    fd0 = open("/dev/null", O_RDWR);
    if (access(LOG_FILE, 0) == -1) {
        fd1 = creat(LOG_FILE, 0777);
    } else {
        fd1 = open(LOG_FILE, O_RDWR | O_APPEND);    
    }
    
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
            fd0, fd1, fd2);
        exit(1);
    }

}
