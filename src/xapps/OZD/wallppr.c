/**
*	Global variables
*/
p_bitmap WllBmp = NULL;
PLabel Lbl = NULL;
l_int Style = 0;

/**
*	NAME: WallpaperDraw
*	DESCRIPTION: Draws the wallpaper to screen
*	RETURN: NONE
*/
void WDraw(PWidget o, p_bitmap buffer, PRect w)
{
	rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, o->BackgroundColor);

	if (WllBmp)
	{
		if (Style == 1)
		{
			/**
			*	Tile wallpaper
			*/
			draw_sprite_tile(WllBmp, buffer, 0, 0, GSScreenWidth, GSScreenHeight);
		}
		else if (Style == 2)
		{
			/**
			*	Stretch wallpaper
			*/
			stretch_blit(WllBmp, buffer, 0, 0, WllBmp->w, WllBmp->h, 0, 0, GSScreenWidth, GSScreenHeight);
		}
		else
		{
			/**
			*	Draw normal size
			*/
			blit(WllBmp, buffer, 0, 0, (GSScreenWidth - WllBmp->w) / 2, (GSScreenHeight - WllBmp->h) / 2, GSScreenWidth, GSScreenHeight);
		}
	}
}

l_bool WEventHandler(PWidget o, PEvent Event)
{
	if (Event->Type == EV_MESSAGE)
	{
		if (Event->Message == DM_PROPERTIES)
		{
			FileRun("xapps/setwall.app", 0);
		}
	}

	if (Event->Type == EV_MOUSE)
	{
		if (Event->Message == WEvMouseRUp)
		{
			PMenu DeskMenu = NewMenu(
				NewMenuItemEx("Properties", NULL, DM_PROPERTIES, 0, NULL, NULL, NULL, NULL)
			);

			PopUpMenu(&Me, Mouse->State.p, DeskMenu, WIDGET(DeskTop), 1);

			return true;
		}
	}

	return false;
}

/**
*	NAME: WallpaperInit
*	DESCRIPTION: Loads the wallpaper, blits it on the wallpaper bitmap with the
*					selected Style.
*	RETURN: NONE
*/
void WallpaperInit(void)
{
	l_text wallpaper = KeyGetText("/USER/DESKTOP/wallpaper", "");

	Style = KeyGetInt("/USER/DESKTOP/alignment", 0);

	if (TextLen(wallpaper))
		WllBmp = LoadImage(wallpaper);
	else
		WllBmp = NULL;

	DeskTop->BackgroundColor = GetColorFromHex(KeyGetText("/USER/DESKTOP/color", "3D61AD"));

	if (WllBmp)
		DeskTop->Draw = &WDraw;
	else
		DeskTop->Draw = NULL;

	DeskTop->EventHandler = &WEventHandler;

	WidgetDraw(DeskTop, NULL);
}

void WallpaperReLoad(void)
{
	if (WllBmp) destroy_bitmap(WllBmp);
	WallpaperInit();
}

void GSWallpaperDestroy()
{
	if (WllBmp) destroy_bitmap(WllBmp);
	DeskTop->EventHandler = NULL;
	DeskTop->Draw = NULL;
}
