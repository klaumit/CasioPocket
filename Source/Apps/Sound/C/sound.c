/* Sound Effect * 
* ------------ */ 
/* by Alessandro Dorigatti (Free Software) */ 
#include <stdrom.h> 
#include "define.h" 
#include "libc.h" 
#include "L_define.h" 
#include "L_libc.h" 
#include "hello.c"

/* Function declaration */ 
void note(int ptch, unsigned long dur, int vol); 
/* Function definition */ 

/* Play a note with "pitch"tch, "duration":dur, "volume":vol */ 
void note(int ptch, unsigned long dur, int vol) 
{ 
unsigned long t; /* Total note time */ 
unsigned long cyc; /* Internal waveform counter */ 
unsigned long stp; /* ON->OFF Point */ 
/* Valid duration range: [1..1000] */ 
if (dur>1000 || dur<1) 
return; 
/* Valid volume range: [1..100] */ 
if (vol>100 || vol<1) 
return; 
/* Valid pitch range: [1..3000] */ 
if (ptch>3000 || ptch<1) 
return; 

ptch = 3050 - ptch; 
vol+=10; 
/* Max Duty Cycle: 25%! */ 
stp = (ptch * vol) / 400; 
dur = (dur+10) * 100; 
cyc = 0; 
for(t=0; t<dur; t++) 
{ 
if(cyc==0) 
LibBuzzerOn(IB_BEEP0_SET); 
else if(cyc==stp) 
LibBuzzerOff(); 
else if(cyc==ptch) 
cyc=-1; 
cyc++; 
} 

LibBuzzerOff(); 
} 
/* Test program */ 
void sound(void) 
{ 
int cnt; 
for(cnt=0; cnt<50; cnt++) 
{ 
note(2700-cnt*16,1,1); 
note(2800-cnt*8,1,1); 
note(2900-cnt*4,1,1); 
note(3000-cnt*2,1,1); 
} 
} 
