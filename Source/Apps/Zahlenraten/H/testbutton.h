Testbutton(){
	TCHSTS tsts;
	LibTchStackClr();
	LibTchStackPush( NULL );
	LibTchStackPush( TchHardIcon );
	LibTchStackPush( Tastiera );
	LibTchInit();
	LibRepOff();	

	for(;;){
		LibTchWait( &tsts );
		switch(tsts.obj){
	/*******TASTEN************************/
	case OBJ_1:
		rechne(1);	
		onscreen(1);
		break;
	case OBJ_2:
		rechne(2);
		onscreen(2);
		break;
	case OBJ_3:
		rechne(3);
		onscreen(3);
		break;
	case OBJ_4:
		rechne(4);
		onscreen(4);
		break;
	case OBJ_5:
		rechne(5);
		onscreen(5);
		break;
	case OBJ_6:
		rechne(6);
		onscreen(6);
		break;
	case OBJ_7:
		rechne(7);
		onscreen(7);
		break;
	case OBJ_8:
		rechne(8);
		onscreen(8);
		break;
	case OBJ_9:
		rechne(9);
		onscreen(9);
		break;
	case OBJ_0:
		rechne(0);
		onscreen(0);
		break;
	case OBJ_links:
		break;
	case OBJ_rechts:
		break;
	case OBJ_del:
		onscreen("DEL");
		a=a-1;
		break;
	case OBJ_next:
		exit = 1;
		break;
		}
		if (exit==1)
		break;
	}
}