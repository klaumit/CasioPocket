#include <define.h>
#include <libc.h>
#include <l_libc.h>

void main ()
{
	int x, y, z, u, w, t;
	x = 32469;
	y = 32468;
	z = 32467;

	u = x + y;
	u = x - y;
	u = x * y;
	u = x / y;
	u--;
	u = x % y;
	u++;
	w = x == u;
	w = x != u;
	w = x > u;
	w = x < u;
	w = x >= u;
	w = x <= u;
	t = x && u;
	t = x || z;
	t = !t;
	u = x & y;
	u = x | y;
	u = x ^ y;
	u = ~y;
	u = x << y;
	u = x >> y;	
}
