/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			screen.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Screen initializations.
*
*	CONTRIBUTORS:
*					Lukas Lipka
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/

#include "kernel.h"
#include "internal.h"

int GSScreenWidth		= 0;
int GSScreenHeight		= 0;
int GSScreenDepth		= 0;
int GSScreenRefreshRate	= 60;

p_bitmap StartLogo = NULL;
TRect StartRect;
p_bitmap QuitLogo = NULL;
TRect QuitRect;
l_ulong FadeTime = 250;


void ScreenCrazyFadeToBlack ( void ) {
	l_ulong t = ATime;
	l_long a = 0,r=rand()%6;
	
	if ( !FadeTime ) return;
	
	set_clip_rect(screen,0,0,GSScreenWidth, GSScreenHeight);
	set_clip_state(screen, TRUE);
	
    acquire_bitmap(screen);
    
	if ( r == 0 ) {
		while ( a < GSScreenWidth<<1 ) {
			line(screen,0,a,a,0,makecol(0,0,0));
			line(screen,0,a+1,a+1,0,makecol(0,0,0));
			if ( ATime-t < a*FadeTime/(GSScreenWidth<<1) ) ThreadTimerWait(1);
			a+=2;	
		}
		
	} else if ( r == 5 ) {
		a = GSScreenWidth<<1;
		while ( a ) {
			line(screen,0,a,a,0,makecol(0,0,0));
			line(screen,0,a-1,a-1,0,makecol(0,0,0));
			if ( ATime-t < ((GSScreenWidth<<1)-a)*FadeTime/(GSScreenWidth<<1) ) ThreadTimerWait(1);
			a-=2;	
		}
		
	} else if ( r == 1 ) {
		while ( a < GSScreenWidth ) {
			vline(screen,a,0,GSScreenHeight,makecol(0,0,0));
			if ( ATime-t < a*FadeTime/GSScreenWidth ) ThreadTimerWait(1);
			a++;	
		}
	} else if ( r == 2 ) {
		a = GSScreenWidth;
		while ( a  ) {
			vline(screen,a,0,GSScreenHeight,makecol(0,0,0));
			if ( ATime-t < (GSScreenWidth-a)*FadeTime/GSScreenWidth ) ThreadTimerWait(1);
			a--;	
		}
	} else if ( r == 3 ) {
		a = GSScreenHeight;
		while ( a  ) {
			hline(screen,0,a,GSScreenWidth,makecol(0,0,0));
			if ( ATime-t < (GSScreenHeight-a)*FadeTime/GSScreenHeight ) ThreadTimerWait(1);
			a--;	
		}		
	} else if ( r == 4 ) {
		while ( a < GSScreenHeight ) {
			hline(screen,0,a,GSScreenWidth,makecol(0,0,0));
			if ( ATime-t < a*FadeTime/GSScreenHeight ) ThreadTimerWait(1);
			a++;	
		}	
	}
    release_bitmap(screen);
	DebugMessage("Fade results : Fade n°%d Time:%d msec",r,ATime-t);
}



l_bool __ScreenRestartGraphics ( void ) {
	
	set_color_depth(GSScreenDepth);
	request_refresh_rate(GSScreenRefreshRate);
	if (set_gfx_mode(GFX_AUTODETECT, GSScreenWidth, GSScreenHeight, 0, 0)< 0) {
		DebugFatal("Unable to restart screen...");
		return false;
	}
	return true;
}

l_bool __ScreenTextMode ( void ) {
	
	ScreenCrazyFadeToBlack();
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

	return true;
}


l_int GSSystemScreenQuit ( void )
{
	QuitLogo = LoadImage("system/shutdown.png");


		RectAssign(&QuitRect,(GSScreenWidth-QuitLogo->w)/2,(GSScreenHeight-QuitLogo->h)/2,(GSScreenWidth-QuitLogo->w)/2+QuitLogo->w-1,(GSScreenHeight-QuitLogo->h)/2+QuitLogo->h-1);
		set_clip_rect(screen,QuitRect.a.x,QuitRect.a.y,QuitRect.b.x,QuitRect.b.y);
		set_clip_state(screen, TRUE);
		draw_sprite(screen,QuitLogo,QuitRect.a.x,QuitRect.a.y);
}


/**
*	NAME:	GSSystemScreenInit
*	DESCRIPTION: Initializes the screen and graphics mode using the values set
*					in the registry.
*	RETURN: 0 on success, else -1
*/
l_int GSSystemScreenInit ( void )
{

	if ( KernelMode == KERNEL_RESETUP )
	{
		l_int c = 0;
		printf("re-setup mode.\n");
		printf("Please select your screen configuration. You hardware may not support all listed modes.");
		printf("\nPlease select screen definition :\n");
		printf("1   640x480\n");
		printf("2   800x600\n");
		printf("3  1024x768\n");
		printf("4  1152x864\n");
		printf("5  1280x1024\n");

		while ( !c )
		{
			c = readkey() & 0xFF;
			if ( c == '1' ) { GSScreenWidth = 640;  GSScreenHeight = 480; }
			else if ( c == '2' ) { GSScreenWidth = 800;  GSScreenHeight = 600; }
			else if ( c == '3' ) { GSScreenWidth = 1024; GSScreenHeight = 768; }
			else if ( c == '4' ) { GSScreenWidth = 1152; GSScreenHeight = 864; }
			else if ( c == '5' ) { GSScreenWidth = 1280; GSScreenHeight = 1024; }
			else c = 0;
		}

		printf("\nPlease select screen color depth :\n");
		printf("1  256 Colors  (8 bpp)\n");
		printf("2  High Colors (15 bpp)\n");
		printf("3  High Colors (16 bpp)\n");
		printf("4  True Colors (24 bpp)\n");
		printf("5  True Colors (32 bpp)\n");

		c = 0;

		while ( !c )
		{
			c = readkey() & 0xFF;
			if ( c == '1' ) { GSScreenDepth = 8;  }
			else if ( c == '2' ) { GSScreenDepth = 15; }
			else if ( c == '3' ) { GSScreenDepth = 16; }
			else if ( c == '4' ) { GSScreenDepth = 24; }
			else if ( c == '5' ) { GSScreenDepth = 32; }
			else c = 0;
		}

	}
	else if ( KernelMode == KERNEL_SAFE )
	{
		GSScreenWidth 	= 1024;
		GSScreenHeight 	= 768;
		GSScreenDepth 	= 16;
		GSScreenRefreshRate = 60;
	}
	else
	{
		GSScreenWidth 		= KeyGetInt("/SYSTEM/SCREEN/WIDTH",640);
		GSScreenHeight 		= KeyGetInt("/SYSTEM/SCREEN/HEIGHT",480);
		GSScreenDepth 		= KeyGetInt("/SYSTEM/SCREEN/DEPTH",8);
		GSScreenRefreshRate	= KeyGetInt("/SYSTEM/SCREEN/REFRESH",60);
	}


	DebugMessage("Initializing screen by these parameters:");
	DebugMessage("- Width:\t\t%i", GSScreenWidth);
	DebugMessage("- Height:\t\t%i", GSScreenHeight);
	DebugMessage("- Depth:\t\t%i", GSScreenDepth);
	DebugMessage("- Refresh Rate:\t\t%i", GSScreenRefreshRate);
	DebugMessage("Installing screen...");

	/**
	 * Set color depth
	 */
	set_color_depth(GSScreenDepth);

	/**
	 * Request refresh rate
	 */
	request_refresh_rate(GSScreenRefreshRate);

	if (set_gfx_mode(GFX_AUTODETECT, GSScreenWidth, GSScreenHeight, 0, 0)< 0)
	{
		/**
		*	If user required mode fails, try a standard one
		*/
		allegro_message("Could not set requested graphics mode.\n");

		GSScreenWidth 	= 640;
		GSScreenHeight 	= 480;
		GSScreenDepth 	= 8;

		set_color_depth(GSScreenDepth);
		if (set_gfx_mode(GFX_AUTODETECT, GSScreenWidth, GSScreenHeight, 0, 0)< 0)
		{
			/**
			*	No graphic mode has been able to be set
			*/
			DebugFatal("Unable to initialise graphics mode");
			return -1;
		}
	}

	if (get_refresh_rate() != GSScreenRefreshRate)
	{
		DebugError("Could not set requested refresh rate, using %iHz instead", get_refresh_rate());
	}

	set_color_conversion(COLORCONV_TOTAL|COLORCONV_KEEP_TRANS);

	StartLogo = LoadImage("system/start.png");


	if ( StartLogo )
	{
		RectAssign(&StartRect,(GSScreenWidth-StartLogo->w)/2,(GSScreenHeight-StartLogo->h)/2,(GSScreenWidth-StartLogo->w)/2+StartLogo->w-1,(GSScreenHeight-StartLogo->h)/2+StartLogo->h-1);
		set_clip_rect(screen,StartRect.a.x,StartRect.a.y,StartRect.b.x,StartRect.b.y);
		set_clip_state(screen, TRUE);
		draw_sprite(screen,StartLogo,StartRect.a.x,StartRect.a.y);

	}
	

	return 0;
}
