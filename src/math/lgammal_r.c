#include <stdint.h>
#include <stdio.h>
#include "util.h"

static struct l_li t[] = {
#if LDBL_MANT_DIG == 53
#include "sanity/lgammal_r.h"

#elif LDBL_MANT_DIG == 64
#include "sanity/lgammal_r.h"

#endif
};

int main(void)
{
	int yi;
	long double y;
	float d;
	int e, i, err = 0;
	struct l_li *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;
		setupfenv(p->r);
		y = lgammal_r(p->x, &yi);
		e = getexcept();
		if (!checkexcept(e, p->e, p->r)) {
			printf("%s:%d: bad fp exception: %s lgammal_r(%La)=%La,%lld, want %s",
				p->file, p->line, rstr(p->r), p->x, p->y, p->i, estr(p->e));
			printf(" got %s\n", estr(e));
			err++;
		}
		d = ulperrl(y, p->y, p->dy);
		if (!checkulp(d, p->r) || yi != p->i) {
			printf("%s:%d: %s lgammal_r(%La) want %La,%lld got %La,%d ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->y, p->i, y, yi, d, d-p->dy, p->dy);
			err++;
		}
	}
	return !!err;
}