////////////////////////////////////////////////////////////////////////////////
//
//	Dos runner
//
//		Can be used to run windows app.(on windows build)
//
//	(c) Copyright 2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "rundos";
l_uid	nUID		= "rundos";
l_uid NeededLibs[]	= { "widget", "" };


l_int Main ( int argc, l_text *argv )
{
	l_char Cmd[2048];
	int i;
	l_text command, a = TextDup(argv[1]),b=a;
	
	if ( TextLen(a) < 5 ) { free(a); return false; }
	
	DebugMessage("Going on...");
	
	if ( a[0] == '/' && a[2] == '/' ) {

		while ( *b ) {
			if ( *b =='/' ) *b = '\\';
			b++;	
		}
		
		command = TextArgs("%c:\\%s",a[1],a+3);
		
		sprintf(Cmd,"%c:\\%s",a[1],a+3);
		for(i = 2; i < argc; i++)
		{
			strcat(Cmd," \"");
			strcat(Cmd,argv[i]);
			strcat(Cmd,"\"");
		}

		free(a);
	} else {
		free(a);
		return false;
	}
	
	SwitchTextMode();
	
	printf("Running %s...\n",Cmd);
	
	DebugMessage("Run dos app : %s\n",Cmd);
	
	system(Cmd);
	
	DebugMessage("done\n");
	
	printf("\n\nPress any key to continue...\n");
	
	getchar();
	
	SwitchGraphicsMode();
	
	free(command);
	
	return false;
}

void Close (void)
{

}
