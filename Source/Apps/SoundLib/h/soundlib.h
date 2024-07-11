/* Mini SoundLib v0.04 - by Alessandro Dorigatti (FreeSoftware)    *
 * --------------------------------------------------------------- */

/* WARNING!: here we write directly to the hardware port #0...     *
 * --------  Moreover, to set buzzer "on/off" we write a byte with *
 *           bit 7 on/off: normally only bits 0..2 are used in     *
 *           PVOS. Then USE THIS FUNCTION ON YOUR OWN RISK!        */

/* Thank you, Johannes (Steingraeber) ---------------------------- */
#define outpsound(c) _asm_sound("\n OUT 00h,AL\n",c)
void _asm_sound(unsigned char *, unsigned char);
/* --------------------------------------------------------------- */

void sound(unsigned int ptch, unsigned int dur, unsigned char wav);
void noise(unsigned int ptch, unsigned int dur, unsigned char wav);
void note(char *n, unsigned int dur, unsigned char wav);







