#include <stdint.h>
#include <stdio.h>
#include "util.h"

static struct d_di t[] = {
#include "sanity/lgamma.h"

};

int main(void)
{
	int yi;
	double y;
	float d;
	int e, i, err = 0;
	struct d_di *p;

	for (i = 0; i < sizeof t/sizeof *t; i++) {
		p = t + i;
		setupfenv(p->r);
		y = lgamma(p->x);
		yi = signgam;
		e = getexcept();
		if (!checkexcept(e, p->e, p->r)) {
			printf("%s:%d: bad fp exception: %s lgamma(%a)=%a,%lld, want %s",
				p->file, p->line, rstr(p->r), p->x, p->y, p->i, estr(p->e));
			printf(" got %s\n", estr(e));
			err++;
		}
		d = ulperr(y, p->y, p->dy);
		if (!checkulp(d, p->r) || yi != p->i) {
			printf("%s:%d: %s lgamma(%a) want %a,%lld got %a,%d ulperr %.3f = %a + %a\n",
				p->file, p->line, rstr(p->r), p->x, p->y, p->i, y, yi, d, d-p->dy, p->dy);
			err++;
		}
	}
	return !!err;
}