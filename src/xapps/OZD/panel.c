/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			panel.c
*
*	PROJECT:		Phoenix - Desktop
*
*	DESCRIPTION:	The task panel.
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

/**
*	Global variables
*/
_PUBLIC PWidget		GSPanel				= 0;
_PUBLIC PWidget		GSTaskbar			= 0;
_PUBLIC PWidget		GSTraybar			= 0;
_PUBLIC PLabel		GSClock				= 0;
_PUBLIC PList		TraybarIconList		= 0;
_PUBLIC PList		WindowList			= 0;
_PUBLIC l_color		PanelColor			= 0;
_PUBLIC PButton		GSStart				= 0;
_PUBLIC l_int		GSPanelHeight		= 0;


PWMENTRY TaskbarAdd ( PWindow w ){
	PWMENTRY p = NEW(TWMENTRY);
	CLEAR(p);

	p->Itm.Data = WIDGET(w);
 	p->Itm.FreeData = NULL;
	ListAddItem(WindowList, (PListItem)p);

	WidgetDraw(GSTaskbar, NULL);
}

l_bool TaskbarRemove ( PWindow w ){
	PListItem i = ListFoundItem(WindowList,w);
	if ( !i ) return;
	ListRemoveItem(WindowList,i);

	WidgetDraw(GSTaskbar, NULL);
}

l_bool TaskbarRedraw ( PWindow w ){

	WidgetDraw(GSTaskbar, NULL);
}

void TaskbarDraw ( PWidget o, p_bitmap buffer, PRect w ){
	l_int x = 5;
	l_int Items = 0;
	l_int ItemWidth = 0;

	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
	
	if ( THMPanelFace && UseSkins )
		DrawLikeSkin(buffer, THMPanelFace->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, THMPanelFace->Left, THMPanelFace->Right,0,0);
	else
		Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK );
	
	PListItem a = WindowList->Last, b = a;

	if (!a) return;

	do
	{
		Items++;

		a = a->Prev;
	}
	while (a != WindowList->Last);

	if (!Items) return;

	ItemWidth = min((o->Absolute.b.x - o->Absolute.a.x - 10) / Items, 150);
	Items = 0;

	b = a = WindowList->Last->Next;

	do
	{
		TRect r;
		RectAssign(&r, o->Absolute.a.x+5+((ItemWidth+3)*Items), o->Absolute.a.y+2, o->Absolute.a.x+((ItemWidth+3)*(Items+1))-3, o->Absolute.b.y-2);

		WMENTRY(a)->BufAbs = r;

		if ( Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(r) || Desktop->Last == WIDGET(a->Data)->Prev )
		{
			Rect3D(buffer, r.a.x, r.a.y, r.b.x, r.b.y, COL_3DDARK, COL_3DLIGHT);
			//masked_blit(WINDOW(a->Data)->Icon, buffer, 0, 0, r.a.x+6, (r.a.y+r.b.y)/2-8+1, r.a.x+22, (r.a.y+r.b.y)/2+8);
			//textout_centre(buffer, default_font, WINDOW(a->Data)->Caption, ((r.a.x+r.b.x)/2)+1, ((r.a.y+r.b.y)/2-text_height(default_font)/2)+1, makecol(0,0,0));
		
			DrawIcon16(buffer,WINDOW(a->Data)->Icon, r.a.x+6, (r.a.y+r.b.y)/2-8+1 );

			if ( WINDOW(a->Data)->Caption ) DrawNiceText(buffer, default_font, r.a.x + 6 + 16 + 5, (r.a.y+r.b.y)/2-text_height(default_font)/2, ((r.b.x - 6) - (r.a.x + 6 + 16 + 5)), WINDOW(a->Data)->Caption, COL_3DTEXT);
		}
		else
		{
			Rect3D(buffer, r.a.x, r.a.y, r.b.x, r.b.y, COL_3DLIGHT, COL_3DDARK);
			//masked_blit(WINDOW(a->Data)->Icon, buffer, 0, 0, r.a.x+5, (r.a.y+r.b.y)/2-8, r.a.x+21, (r.a.y+r.b.y)/2+8);
			//textout_centre(buffer, default_font, WINDOW(a->Data)->Caption, (r.a.x+r.b.x)/2, (r.a.y+r.b.y)/2-text_height(default_font)/2, makecol(0,0,0));
			
			DrawIcon16(buffer,WINDOW(a->Data)->Icon, r.a.x+5, (r.a.y+r.b.y)/2-8 );
			
			if ( WINDOW(a->Data)->Caption ) DrawNiceText(buffer, default_font, r.a.x + 5 + 16 + 5, (r.a.y+r.b.y)/2-text_height(default_font)/2, ((r.b.x - 5) - (r.a.x + 5 + 16 + 5)), WINDOW(a->Data)->Caption, COL_3DTEXT);
		}

		Items++;

		a = a->Next;
	}
	while (a != b);
}

l_bool TaskbarEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MOUSE )
	{
		/*if ( Ev->Message == WEvMouseLDown ) {
			WidgetDraw(o,NULL);
			return true;
		}*/

		if ( Ev->Message == WEvMouseLUp )
		{
			PListItem a = WindowList->Last;

			if (!a) return true;

			do
			{
				if ( PointInRect(Mouse->State.p, WMENTRY(a)->BufAbs) )
					WidgetSetFirst(WIDGET(a->Data));

				a = a->Prev;
			}
			while (a != WindowList->Last);
			WidgetDraw(o,NULL);
			return true;
		}
	}

	return false;
}

void PanelDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
	
	if ( THMPanelFace && UseSkins )
		DrawLikeSkin(buffer, THMPanelFace->Skin, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, THMPanelFace->Left, THMPanelFace->Right,0,0);
	else
		Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
}

l_bool PanelEventHandler ( PWidget o, PEvent Ev )
{
	return false;
}

void StartButtonDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE );
	
	if ( THMPanelStartDown && THMPanelStartOver && THMPanelStart && UseSkins ) {
	
		if ( BUTTON(o)->State == BS_OVER )
			blit(THMPanelStartOver->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		else if ( BUTTON(o)->State == BS_DOWN || (Mouse->State.b & BUTTON_LEFT && CURSOR_IN_RECT(*w)))
			blit(THMPanelStartDown->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		else
			blit(THMPanelStart->Skin, buffer, 0, 0, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
	} else {
		if ( BUTTON(o)->State == BS_OVER )
			rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DOVER);
		
		if ( BUTTON(o)->State != BS_DOWN ) {
			Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
			//textout_ex(buffer, default_font, "oZone", (o->Absolute.a.x+o->Absolute.b.x)/2, ((o->Absolute.a.y+o->Absolute.b.y)/2-text_height(default_font)/2), COL_3DTEXT);
		} else {
			Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK, COL_3DLIGHT);
			//textprintf_ex(buffer, default_font, "oZone", (o->Absolute.a.x+o->Absolute.b.x)/2+1, (o->Absolute.a.y+o->Absolute.b.y-text_height(default_font))/2+1, COL_3DTEXT);
			// these need to be fixed.
		}
	}
}

void RefreshPanelClock ( void *A )
{
	//struct time t;
	//ctime(t);

	//LabelSetText(GSClock, "%s", ctime(NULL));//%d:%02d", t.ti_hour, t.ti_min);

	time_t 			 t = time(NULL);
	struct tm 	*tme;

	DebugMessage("pretime");

	tme = localtime(&t);
	if ( tme )
		LabelSetText(GSClock, "%d:%02d", tme->tm_hour,tme->tm_min);
	DebugMessage("posttime");

}

void TraybarDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	rectfill(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DFACE);
	rect(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DDARK);

	PListItem a = TraybarIconList->Last;

	if (!a) return;

	#define DEFAULT_ICONW 16
	#define DEFAULT_ICONH 16

	l_int x = o->Absolute.a.x + 3;

	do
	{
		RectAssign(&((PTRAYBARICON)(a->Data))->BufAbs,x, o->Absolute.a.y + (o->Absolute.b.y-o->Absolute.a.y)/2 - (DEFAULT_ICONH)/2, x + DEFAULT_ICONW, o->Absolute.a.y + (o->Absolute.b.y-o->Absolute.a.y)/2 + (DEFAULT_ICONH)/2);

		masked_blit(((PTRAYBARICON)(a->Data))->Icon, buffer, 0, 0, ((PTRAYBARICON)(a->Data))->BufAbs.a.x, ((PTRAYBARICON)(a->Data))->BufAbs.a.y, ((PTRAYBARICON)(a->Data))->BufAbs.b.x, ((PTRAYBARICON)(a->Data))->BufAbs.b.y);

		x += DEFAULT_ICONW + 2;

		a = a->Prev;
	}
	while (a != TraybarIconList->Last);
}

l_bool TraybarEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{
		if (Ev->Message == WEvMouseLDown || Ev->Message == WEvMouseRDown )
		{
			PListItem a = TraybarIconList->Last;

			if (!a) return;

			do
			{
				if ( ((PTRAYBARICON)(a->Data))->Menu && CURSOR_IN_RECT( ((PTRAYBARICON)(a->Data))->BufAbs) )
				{
					PopUpMenu(&Me, Mouse->State.p, ((PTRAYBARICON)(a->Data))->Menu, ((PTRAYBARICON)(a->Data))->MsgDest, 0);
				}

				a = a->Prev;
			}
			while (a != TraybarIconList->Last);

			return true;
		}
	}

	return false;
}

l_bool TraybarAdd ( p_bitmap Icon, l_text Tooltip, PMenu Menu, PWidget MsgDest )
{
	if (!GSTraybar)
	{
		TRect r;
		RectAssign(&r,GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00")-10-16-6, 1, GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00")-10, GSPanel->Absolute.b.y-1);
		GSTraybar = CreateWidget(&Me, r);
		GSTraybar->Draw = &TraybarDraw;
		GSTraybar->EventHandler = &TraybarEventHandler;
		InsertWidget(GSPanel, WIDGET(GSTraybar));
	}
	else
		WidgetSetMetrics(GSTraybar, GSTraybar->Absolute.a.x-(DEFAULT_ICONW+2), GSTraybar->Absolute.a.y, GSTraybar->Absolute.b.x, GSTraybar->Absolute.b.y);

	PTRAYBARICON p = NEW(TTRAYBARICON);

	if (!p || !Icon) return 0;

	CLEAR(p);

	p->Icon = Icon;
	p->Tooltip = TextDup(Tooltip);
	p->Menu = Menu;
	p->MsgDest = MsgDest;

	ListAdd(TraybarIconList, Tooltip, p, &free);

	WidgetDraw(GSTraybar, NULL);

	return true;
}

l_bool TraybarRemove ( l_text Key )
{
	ListRemoveItem(TraybarIconList, ListKeyItem(TraybarIconList, Key));

	if ( !TraybarIconList->Last ){
		WidgetDispose(GSTraybar);
		GSTraybar = 0;
	}
	else{
		WidgetSetMetrics(GSTraybar, GSTraybar->Absolute.a.x+(DEFAULT_ICONW+2), GSTraybar->Absolute.a.y, GSTraybar->Absolute.b.x, GSTraybar->Absolute.b.y);
		WidgetDraw(GSTraybar, NULL);
	}
}

_PUBLIC void  GSPanelInit()
{
	l_ulong Height = 30;
	l_ulong BHeight = 24;
	l_ulong BWidth = 40;
	TRect r;

	TraybarIconList = NewList();
	WindowList = NewList();

	if ( UseSkins && THMPanelFace ) Height = THMPanelFace->Skin->h;
	if ( UseSkins && THMPanelStart ) BHeight = THMPanelStart->Skin->h;
	if ( UseSkins && THMPanelStart ) BWidth = THMPanelStart->Skin->w;
	
	RectAssign(&r,0, 0, GSScreenWidth, Height);
	GSPanel = CreateWidget(&Me, r );
	GSPanel->Draw = &PanelDraw;
	GSPanel->EventHandler = &PanelEventHandler;
	GSPanel->Flags |= WFOnTop;
	InsertWidget(DeskTop, GSPanel);

	RectAssign(&r,0, GSScreenHeight - Height, GSScreenWidth, GSScreenHeight);
	GSTaskbar = CreateWidget(&Me, r );
	GSTaskbar->Draw = &TaskbarDraw;
	GSTaskbar->EventHandler = &TaskbarEventHandler;
	GSTaskbar->Flags |= WFOnTop;
	InsertWidget(DeskTop, GSTaskbar);

	RectAssign(&r,5, (Height-BHeight)/2, 5+BWidth-1, (Height+BHeight)/2-1);
	GSStart = CreateButton(&Me, r, "OZD::Start", BTN_START);
	WIDGET(GSStart)->Draw = &StartButtonDraw;
	WIDGET(GSStart)->AppEvHdl = &AppEventHandler;
	WidgetSetTooltipText(WIDGET(GSStart),"Click here to start");
	InsertWidget(GSPanel, WIDGET(GSStart));

	RectAssign(&r,GSPanel->Absolute.b.x - 7 - text_length(default_font, "12:00"), (GSPanel->Absolute.b.y - text_height(default_font))/2, GSPanel->Absolute.b.x - 7, (GSPanel->Absolute.b.y - text_height(default_font))/2 + text_height(default_font) );
	GSClock = CreateLabel(&Me, r, "12:00");
	LABEL(GSClock)->FontColor = COL_3DTEXT;
	WIDGET(GSClock)->BackgroundColor = COL_3DFACE;
	InsertWidget(GSPanel, WIDGET(GSClock));
	RefreshPanelClock(NULL);

	RectAssign(&WINDOW_MaximizeArea, 0,Height+1, GSScreenWidth, GSScreenHeight - Height-1);

	WidgetDrawAll(DeskTop);
}

void GSPanelDestroy()
{
	WidgetDispose(GSPanel);
	WidgetDispose(GSTaskbar);
	FreeList(TraybarIconList);
	FreeList(WindowList);
}
