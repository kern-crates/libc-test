#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include "test.h"

#define TEST(c, ...) ((c) ? 1 : (error(#c" failed: " __VA_ARGS__),0))

int main(void)
{
	volatile int x = 0, r;
	jmp_buf jb;
	sigjmp_buf sjb;
	volatile sigset_t oldset;
	sigset_t set;

	if (!setjmp(jb)) {
		x = 1;
		longjmp(jb, 1);
	}
	TEST(x==1, "setjmp/longjmp seems to have been bypassed\n");

	x = 0;
	r = setjmp(jb);
	if (!x) {
		x = 1;
		longjmp(jb, 0);
	}
	TEST(r==1, "longjmp(jb, 0) caused setjmp to return %d\n", r);

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &set, &set);
	oldset = set;

	/* Improve the chances of catching failure of sigsetjmp to
	 * properly save the signal mask in the sigjmb_buf. */
	memset(&sjb, -1, sizeof sjb);

	if (!sigsetjmp(sjb, 1)) {
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		sigprocmask(SIG_BLOCK, &set, 0);
		siglongjmp(sjb, 1);
	}
	set = oldset;
	sigprocmask(SIG_SETMASK, &set, &set);
	TEST(sigismember(&set, SIGUSR1)==0, "siglongjmp failed to restore mask\n");

	return test_status;
}
