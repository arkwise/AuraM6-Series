/**
*	Global variables
*/
_PUBLIC PTHEMEITEM THMPanelFace			= 0;
_PUBLIC PTHEMEITEM THMDockFace			= 0;
_PUBLIC PTHEMEITEM THMPanelleft		= 0;
_PUBLIC PTHEMEITEM THMPanelright		= 0;
_PUBLIC PTHEMEITEM THMPanelStart		= 0;
_PUBLIC PTHEMEITEM THMPanelStartOver	= 0;
_PUBLIC PTHEMEITEM THMPanelStartDown	= 0;
_PUBLIC PTHEMEITEM THMPanelCP			= 0;
_PUBLIC PTHEMEITEM THMPanelCPOver		= 0;
_PUBLIC PTHEMEITEM THMPanelCPDown		= 0;
_PUBLIC PTHEMEITEM THMPanelapp		= 0;
_PUBLIC PTHEMEITEM THMPanelappOver		= 0;
_PUBLIC PTHEMEITEM THMPanelappDown		= 0;
_PUBLIC PTHEMEITEM THMPanelUSR		= 0;
_PUBLIC PTHEMEITEM THMPanelUSROver		= 0;
_PUBLIC PTHEMEITEM THMPanelUSRDown		= 0;
_PUBLIC PTHEMEITEM THMPanelQUIT		= 0;
_PUBLIC PTHEMEITEM THMPanelQUITOver		= 0;
_PUBLIC PTHEMEITEM THMPanelQUITDown		= 0;
_PUBLIC PTHEMEITEM THMPanelnav		= 0;
_PUBLIC PTHEMEITEM THMPanelnavOver		= 0;
_PUBLIC PTHEMEITEM THMPanelnavDown		= 0;
_PUBLIC PTHEMEITEM THMSidebarFace		= 0;
_PUBLIC PTHEMEITEM THMSidebarClock		= 0;
_PUBLIC PTHEMEITEM THMTraybarFace		= 0;
_PUBLIC PTHEMEITEM THMPanelTask		= 0;
_PUBLIC PTHEMEITEM THMPanelTaskDown	= 0;
_PUBLIC PTHEMEITEM THMAlertFace	= 0;

void DrawLikeSkin(p_bitmap out, p_bitmap Skin, l_int x1, l_int y1, l_int x2, l_int y2, l_int Left, l_int Right, l_int Top, l_int Bottom)
{
	if (!Skin)
	{
		rectfill(out, x1, y1, x2, y2, makecol(0,0,0));
		return;
	}

	masked_blit(Skin, out, 0, 0, x1, y1, Left, Top);
	masked_blit(Skin, out, 0, Skin->h-Bottom, x1, y2-Bottom+1, Left, Bottom);
	masked_blit(Skin, out, Skin->w-Right, 0, x2-Right+1, y1, Right, Top);
	masked_blit(Skin, out, Skin->w-Right, Skin->h-Bottom, x2-Right+1, y2-Bottom+1, Right, Bottom);

	masked_stretch_blit(Skin, out, 0, Top, Left, Skin->h-Top-Bottom, x1, y1+Top, Left, (y2-y1+1)-Top-Bottom);
	masked_stretch_blit(Skin, out, Skin->w-Right, Top, Right, Skin->h-Top-Bottom, x2-Right+1, y1+Top, Right, (y2-y1+1)-Top-Bottom);

	masked_stretch_blit(Skin, out, Left, 0, Skin->w-Left-Right, Top, x1+Left, y1, (x2-x1+1)-Left-Right, Top);
	masked_stretch_blit(Skin, out, Left, Skin->h-Bottom, Skin->w-Left-Right, Bottom, x1+Left, y2-Bottom+1, (x2-x1+1)-Left-Right, Bottom);

	masked_stretch_blit(Skin, out, Left, Top, Skin->w-Left-Right, Skin->h-Top-Bottom, x1+Left, y1+Top, (x2-x1)-Left-Right+1, (y2-y1)-Top-Bottom+1);
}

_PUBLIC PTHEMEITEM  ThemeLoadItem ( l_text szThemeFile, l_text Item )
{
//l_text File = malloc(261);
l_text File = INIGetNew(szThemeFile, Item, "bmp", "");
//DebugMessage("IniLoad: textlen [%d]", strlen(File));
if (!strlen(File)) return NULL;  // Florian Xaver 15.05.2006: this line fixes a bug if there is no item in the file
	PImage Img = LoadImage(File);
	if ( Img ) {
		PTHEMEITEM p = NEW(TTHEMEITEM);
		p->Skin		= Img;
		p->Left		= INIGetInteger(szThemeFile, Item, "left", 0);
		p->Right	= INIGetInteger(szThemeFile, Item, "right", 0);
		p->Top		= INIGetInteger(szThemeFile, Item, "top", 0);
		p->Bottom	= INIGetInteger(szThemeFile, Item, "bottom", 0);
		return p;
	}
	return NULL;
}

_PUBLIC void  ThemeLoad ( l_text szThemeFile )
{
	// Sidebar
	THMSidebarFace		= ThemeLoadItem(szThemeFile, "sidebar_face");

	THMSidebarClock		= ThemeLoadItem(szThemeFile, "sidebar_clock");

	// Panel
	THMDockFace		= ThemeLoadItem(szThemeFile, "dock_face");
	THMPanelFace		= ThemeLoadItem(szThemeFile, "panel_face");

	THMPanelStart		= ThemeLoadItem(szThemeFile, "panel");
	THMPanelStartOver	= ThemeLoadItem(szThemeFile, "panel_over");
	THMPanelStartDown	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelCP		= ThemeLoadItem(szThemeFile, "panel");
	THMPanelCPOver	= ThemeLoadItem(szThemeFile, "panel_over");
	THMPanelCPDown	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelapp		= ThemeLoadItem(szThemeFile, "panel");
	THMPanelappOver	= ThemeLoadItem(szThemeFile, "panel_over");
	THMPanelappDown	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelUSR		= ThemeLoadItem(szThemeFile, "panel");
	THMPanelUSROver	= ThemeLoadItem(szThemeFile, "panel_over");
	THMPanelUSRDown	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelQUIT	= ThemeLoadItem(szThemeFile, "panel");
	THMPanelQUITOver	= ThemeLoadItem(szThemeFile, "panel_over");
	THMPanelQUITDown	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelnav	= ThemeLoadItem(szThemeFile, "nav");
	THMPanelnavOver	= ThemeLoadItem(szThemeFile, "nav_over");
	THMPanelnavDown	= ThemeLoadItem(szThemeFile, "nav_down");

	THMPanelleft	= ThemeLoadItem(szThemeFile, "panel_left");
	THMPanelright	= ThemeLoadItem(szThemeFile, "panel_down");

	THMPanelTask		= ThemeLoadItem(szThemeFile, "task");
	THMPanelTaskDown	= ThemeLoadItem(szThemeFile, "task_down");

	// Traybar
	THMTraybarFace		= ThemeLoadItem(szThemeFile, "traybar_face");

	//Alert
	THMAlertFace		= ThemeLoadItem(szThemeFile, "alert_face");
}

