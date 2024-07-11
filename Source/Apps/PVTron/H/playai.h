playai(int playerposx, int playerposy, int aiposx, int aiposy)
{

/* testen wo am meisten platz ist, links, rechts, oben oder unten 

links=TestDot(aiposx-1,aiposy)
rechts=TestDot(aiposx+1,aiposy)
unten=TestDot(aiposx,aiposy+1)
oben=TestDot(aiposx,aiposy-1)
*/

/* DRIVING UP */
if (aidirection==0 && TestDot(aiposx,aiposy-1)==1)
{
	if (TestDot(aiposx-1,aiposy)==0) aidirection=3;
	if (TestDot(aiposx+1,aiposy)==0) aidirection=1;
}

if (aidirection==0 && TestDot(aiposx,aiposy-3)==1)
{ 
	if (TestDot(aiposx-1,aiposy)==0) aidirection=3;
	if (TestDot(aiposx+1,aiposy)==0) aidirection=1;
}


/* DRIVING DOWN */
if (aidirection==2 && TestDot(aiposx,aiposy+1)==1)
{
	if (TestDot(aiposx-1,aiposy)==0) aidirection=1;
	if (TestDot(aiposx+1,aiposy)==0) aidirection=3;
}

if (aidirection==2 && TestDot(aiposx,aiposy+2)==1)
{
	if (TestDot(aiposx-1,aiposy)==0) aidirection=1;
	if (TestDot(aiposx+1,aiposy)==0) aidirection=3;
}

/* DRIVING RIGHT */
if (aidirection==1 && TestDot(aiposx+1,aiposy)==1)
{
	if (TestDot(aiposx,aiposy-1)==0) aidirection=0;
	if (TestDot(aiposx,aiposy+1)==0) aidirection=2;
}
if (aidirection==1 && TestDot(aiposx+3,aiposy)==1)
{
	if (TestDot(aiposx,aiposy-1)==0) aidirection=0;
	if (TestDot(aiposx,aiposy+1)==0) aidirection=2;
}

/* DRIVING LEFT */
if (aidirection==3 && TestDot(aiposx-1,aiposy)==1)
{
	if (TestDot(aiposx,aiposy-1)==0) aidirection=2;
	if (TestDot(aiposx,aiposy+1)==0) aidirection=0;
}
if (aidirection==3 && TestDot(aiposx-4,aiposy)==1)
{
	if (TestDot(aiposx,aiposy-1)==0) aidirection=2;
	if (TestDot(aiposx,aiposy+1)==0) aidirection=0;
}




/*
if (aidirection==0 && TestDot(aiposx,aiposy-3)==1) aidirection=3;
if (aidirection==3 && TestDot(aiposx-4,aiposy)==1) aidirection=2;
if (aidirection==2 && TestDot(aiposx,aiposy+2)==1) aidirection=1;
if (aidirection==1 && TestDot(aiposx+3,aiposy)==1) aidirection=0;

if (aidirection==0 && TestDot(aiposx,aiposy-3)==1) aidirection=1;
if (aidirection==2 && TestDot(aiposx,aiposy+2)==1) aidirection=3;
if (aidirection==1 && TestDot(aiposx+3,aiposy)==1) aidirection=2;
if (aidirection==3 && TestDot(aiposx-4,aiposy)==1) aidirection=0;
*/


}
