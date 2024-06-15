#include <stdio.h>

void DebugMessage ( char* format, ... );
void DebugWarning ( char* format, ... );
void DebugError ( char* format, ... );
void DebugFatal ( char* format, ... );

void GSSystemDebugInstall ( l_text szFilename );
void GSSystemDebugUninstall ( void );

l_bool DebugCheckPtr ( l_text inf, void *ptr );

#define DCkPt(i,p) DebugCheckPtr(i,p)
