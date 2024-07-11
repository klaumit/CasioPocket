DrawHead()
{
  /* Clear Screen */
  LibInitDisp();
  /* Hausicon links oben */
  LibPutFarData(0,0,81);
  /* Summenicon rechts oben */
  LibPutFarData(140,0,50);
  /* Box zeichnen */
  LibGdsBox(0,0,159,159);
  /* Überschrift */
  LibStringDsp(GetMsg(0),50,10,158,IB_PFONT3);
  LibPutDisp();
}


DrawHeadNeu()
{
  /* Clear Screen */
  LibInitDisp();
  /* Box zeichnen */
  LibGdsBox(0,0,159,159);
  /* Überschrift */
  LibStringDsp(GetMsg(3),45,10,158,IB_PFONT3);
  LibPutDisp();
}

