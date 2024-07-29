#include <stdio.h>
#include <string.h>
#include <define.h>
#include <l_define.h>
#include <libc.h>
#include <l_libc.h>
#include <stdrom.h>

void main ()
{
	char* str1;
	int x, y, z, u, w, t, l, a, sum, diff, prod, quot, rem, b, counter, i;

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

	b = 99999999;

	a = 10, b = 5;
    sum = a + b;
    diff = a - b;
    prod = a * b;
    quot = a / b;
    rem = a % b;
	l = 3.393938 * 221.2991211;

	a = 0xF0F0, b = 0x0F0F;
    w = a & b;
    x = a | b;
    y = a ^ b;
    u = ~a;

	counter = 0;
    for (i = 1; i <= 10; i++) {
        counter += i;
    }
    a = counter;

	x = str1[2];
	u = y + 324939393;

	while(t < u) w *= 2;

	if (x == 3) 
		y = 5;
	else if (x == 5)
		y = 7;
	else
		y = 9;

	if (z == 3)
		w = w * 3;

	switch (diff) {
		case 1: return;
		case 2: prod = 23924; break;
	}
	
	do { w -= 7; } while (z <= u);	
}
