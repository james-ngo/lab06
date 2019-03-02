#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "timeit.h"
#define DECIMAL 10
#define HALFSEC 500000
#define TICK 7
#define TOCK 5
#define TIME 10


int main(int argc, char *argv[]) {
	struct sigaction sa;
	struct itimerval itval;
	sigset_t mask, oldmask;
	int num;
	num = strtol(argv[1], NULL, DECIMAL) * 2;
	sa.sa_handler = sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGALRM, &sa, NULL);
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_usec = HALFSEC;
	itval.it_value.tv_sec = 0;
	itval.it_value.tv_usec = HALFSEC;
	setitimer(ITIMER_REAL, &itval, NULL);
	sigdelset(&oldmask, SIGALRM);
	while (num--) {
		sigsuspend(&oldmask);
	}
	write(STDOUT_FILENO, "Times up!\n", TIME);
	return 0;
}

void sigint() {
	static int num = 0;
	if (num++ & 1) {
		write(STDOUT_FILENO, "Tock\n", TOCK);
	}
	else {
		write(STDOUT_FILENO, "Tick...", TICK);
	}
}
