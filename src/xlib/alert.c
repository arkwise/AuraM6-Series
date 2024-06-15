#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "internal.h"
#include "desktop.h"
#include "theme.c"


l_ulong	AppVersion = ULONG_ID(0, 1, 0, 0);
l_char	AppName[] = "Alert Widget";
l_uid	nUID = "alert";
l_uid NeededLibs[] = { "widget", "skin","button","label","progress","" };

