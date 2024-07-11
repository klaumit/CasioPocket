leverpush(){
 int far getleverpush();
 int i;

 i = getleverpush();
 switch (i)
 {
   case 0:
	pressed=0;
	break;
   case 1:
     /* leverpush UP */
if (pressed==0)
{
	if (direction==0) direction=3;
else
	if (direction==1) direction=0;
else
	if (direction==2) direction=1;
else
	if (direction==3) direction=2;
	knack();
	pressed=1;
}
     break;

   case 2:
     /* leverpush PRESSED */
     break;

   case 3:
     /* leverpush DOWN */
if (pressed==0)
{
	if (direction==1) direction=2;
else
	if (direction==0) direction=1;
else
	if (direction==2) direction=3;
else
	if (direction==3) direction=0;
	knack();
	pressed=1;
}
     break;

     /* return values for S460/660 according to Stephan Rath
      * thank you for this contribution */
   case 6:
     /* leverpush LEFT (S460/660 only) */
if (pressed==0)
{
	if (direction==0) direction=3;
else
	if (direction==1) direction=0;
else
	if (direction==2) direction=1;
else
	if (direction==3) direction=2;
	knack();
	pressed=1;
}
     break;

   case 7:
     /* leverpush RIGHT (S460/660 only) */
if (pressed==0)
{
	if (direction==1) direction=2;
else
	if (direction==0) direction=1;
else
	if (direction==2) direction=3;
else
	if (direction==3) direction=0;
	knack();
	pressed=1;
}
     break;
 }
}
