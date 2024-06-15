#ifndef _SCREEN_H_INCLUDED_
#define _SCREEN_H_INCLUDED_

extern l_int GSScreenWidth;
extern l_int GSScreenHeight;
extern l_int GSScreenDepth;
extern l_int GSScreenRefreshRate;

l_int GSSystemScreenInit ( void );

extern p_bitmap StartLogo;
extern TRect    StartRect;

l_bool __ScreenRestartGraphics ( void );
l_bool __ScreenTextMode ( void );


#endif /* _SCREEN_H_INCLUDED */
