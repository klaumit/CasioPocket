/* TestDot from Johannes Steingräber for testing for set or unset pixel */
int TestDot(unsigned int x, unsigned int y)
{
  byte far * p = MK_FP(0xF000,0x0100);
  byte mask = 0x80;

  p += y * 20;
  p += x / 8;
  mask = mask >> (x % 8);

  if (*p & mask)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}