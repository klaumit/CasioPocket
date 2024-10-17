#include <define.h>
#include <libc.h>
#include <l_libc.h>
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <l_define.h>
#include <libc.h>
#include <l_libc.h>
#include <stdrom.h>

void wait()
{
	int i;
	for (i = 0; i < 150; i++)
	{
		LibWait (IB_1SWAIT);
	}	
}

void writeln(char debug[128], int row)
{
	LibStringDsp(debug,5,row,160,IB_PFONT2);
	LibPutDisp();
	wait();
}

void main()
{
	char debug[128];
	float f1, f2, f3, f4, f5, f6, f7;
	int i, x, y, z, u1, u2, u3, u4, u5, u6, w1, w2, w3, w4, w5, w6, t1, t2, t3, counter, diff, prod;
	long a, b;
	
	x = 32469;
	y = 32468;
	z = 32467;	
		
	LibInitDisp();
	
	u1 = x + y;
	u2 = x - y;
	u3 = x * y;
	u4 = x / y;
	
	sprintf(debug, "%d %d %d %d", u1, u2, u3, u4);
	writeln(debug, 10);
	
	u1--;
	u1 = x % y;
	u1++;
	w1 = x == u1;
	w2 = x != u2;
	w3 = x > u3;
	w4 = x < u4;
	w5 = x >= u1;
	w6 = x <= u2;
	
	sprintf(debug, "%d %d %d %d %d %d %d", u1, w1, w2, w3, w4, w5, w6);
	writeln(debug, 20);
		
	t1 = x && u1;
	t2 = x || z;
	t3 = !t2;
	u1 = (x-30200) & y;
	u2 = x | (y-30100);
	
	sprintf(debug, "%d %d %d %d %d", t1,t2,t3,u1,u2);
	writeln(debug, 30);
		
	u3 = x ^ y;
	u4 = ~y;
	u5 = x << y;
	u6 = x >> y;
	b = 99999999;

	sprintf(debug, "%d %d %d %d %d", u3,u4,u5,u6,b);
	writeln(debug, 40);
	
	f1 = 10.2389;
	f2 = 5.177292;
    f3 = f1 + f2;
    f4 = f3 - f1;
    f1 = f1 * f4;
    f5 = f2 / f3;
	
	sprintf(debug, "%.2f %.2f %.2f %.2f %.2f", f1,f2,f3,f4,f5);
	writeln(debug, 50);	
	
	f6 = x % 2;
	f7 = 3.393938 * 221.2991211;
	a = 0xF0F0, b = 0x0F0F;
    w1 = a & b;
    x = a | b;
    y = a ^ b;
	
	sprintf(debug, "%d %.2f %d %d %d", f6,f7,w1,x,y);
	writeln(debug, 60);	

    u1 = ~a;
	
	counter = 4;
    for (i = 1; i <= 13; i++) {
        counter += i;
    }
    a = counter;
	
	x = debug[2];
	u1 = y + 3249;
	
	while(counter > 0) {
		w1 *= 2;
		counter--;
	}

	sprintf(debug, "%d %d %d %d", u1, counter, x, w1);
	writeln(debug, 70);	

	if (x == 3) 
		y = 5;
	else if (x == 5)
		y = 7;
	else
		y = 9;

	if (z == 3)
		w2 = w2 * 3;
	
	diff = w4;
	switch (diff) {
		case 1: return;
		case 2: prod = 23924; break;
	}
	
	do { w3 -= 7; } while (z <= u1);	
	
	sprintf(debug, "%d %d %d %d %d %d", y, z, w2, diff, prod, w3);
	writeln(debug, 80);	
					
	wait();
	LibJumpMenu();
}
