////////////////////////////////////////////////////////////////////////////////
//
//  Color selector 
//
//  (c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "colsel.h"
#include "window.h"
#include "button.h"
#include "tabbook.h"

////////////////////////////////////////////////////////////////////////////////
l_ulong AppVersion = ULONG_ID(0,0,0,1);
char    AppName[] = "Color Selector";
l_uid	nUID = "colsel";
l_uid NeededLibs[] = { "widget","window","button","tabbook","label","" };

RGB Palette[6][8] = {
{{255,0,0},		{64,0,0},	{128,0,0},	{192,0,0},	{255,64,64},	{255,128,128}, {255,192,192}, {0,0,0}		},	
{{255,255,0},	{64,64,0},	{128,128,0},{192,192,0},{255,255,64},	{255,255,128}, {255,255,192}, {32,32,32}	},
{{0,255,0},		{0,64,0},	{0,128,0},	{0,192,0},	{64,255,64},	{128,255,128}, {192,255,192}, {64,64,64}	},
{{0,255,255},	{0,64,64},	{0,128,128},{0,192,192},{64,255,255},	{128,255,255}, {192,255,255}, {128,128,128}	},
{{0,0,255},		{0,0,64},	{0,0,128},	{0,0,192},	{64,64,255},	{128,128,255}, {192,192,255}, {192,192,192}	},
{{255,0,255},	{64,0,64},	{128,0,128},{192,0,192},{255,64,255},	{255,128,255}, {255,192,255}, {255,255,255}	}
};
////////////////////////////////////////////////////////////////////////////////
PColFavItem ColFavAddItem ( PListview o, l_text name, int r, int g, int b )
{
	PImage img;
	
	PColFavItem p = malloc(sizeof(TColFavItem));
	if (!p) return;
	memset(p, 0, sizeof(TColFavItem));

	ListviewInitItem(LISTVIEWITEM(p),name,NULL);
	
	img = create_bitmap(16,16);
	rectfill(img,0,0,15,15,makecol(r,g,b));
	LISTVIEWITEM(p)->Icon = NewIcon(img,0,0);
	
	p->r = r;
	p->g = g;
	p->b = b;
	
	ListAdd (LISTVIEW(o)->Items, name, p, (void*)&FreeListviewItem);
	return p;
}

////////////////////////////////////////////////////////////////////////////////
void  RDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int x=0;
	while ( x <= o->Absolute.b.x-o->Absolute.a.x ) {
		vline(buffer,o->Absolute.a.x+x,o->Absolute.a.y,o->Absolute.b.y,makecol(255*x/(o->Absolute.b.x-o->Absolute.a.x),0,0));
		x++;	
	} 
	x = COLSELEL(o)->a->r*(o->Absolute.b.x-o->Absolute.a.x)/255;
	rect(buffer,o->Absolute.a.x+x-1,o->Absolute.a.y,o->Absolute.a.x+x+1,o->Absolute.b.y,makecol(255,255,255));
}
////////////////////////////////////////////////////////////////////////////////
void  GDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int x=0;
	while ( x <= o->Absolute.b.x-o->Absolute.a.x ) {
		vline(buffer,o->Absolute.a.x+x,o->Absolute.a.y,o->Absolute.b.y,makecol(0,255*x/(o->Absolute.b.x-o->Absolute.a.x),0));
		x++;	
	} 
	x = COLSELEL(o)->a->g*(o->Absolute.b.x-o->Absolute.a.x)/255;
	rect(buffer,o->Absolute.a.x+x-1,o->Absolute.a.y,o->Absolute.a.x+x+1,o->Absolute.b.y,makecol(255,255,255));
}
////////////////////////////////////////////////////////////////////////////////
void  BDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int x=0;
	while ( x <= o->Absolute.b.x-o->Absolute.a.x ) {
		vline(buffer,o->Absolute.a.x+x,o->Absolute.a.y,o->Absolute.b.y,makecol(0,0,255*x/(o->Absolute.b.x-o->Absolute.a.x)));
		x++;	
	} 
	x = COLSELEL(o)->a->b*(o->Absolute.b.x-o->Absolute.a.x)/255;
	rect(buffer,o->Absolute.a.x+x-1,o->Absolute.a.y,o->Absolute.a.x+x+1,o->Absolute.b.y,makecol(255,255,255));
}
////////////////////////////////////////////////////////////////////////////////
void  HueSelDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int r,g,b,x=0;
	while ( x <= o->Absolute.b.x-o->Absolute.a.x ) {
		hsv_to_rgb(360*x/(o->Absolute.b.x-o->Absolute.a.x),1,1,&r,&g,&b);
		vline(buffer,o->Absolute.a.x+x,o->Absolute.a.y,o->Absolute.b.y,makecol(r,g,b));
		x++;	
	} 
	x = COLSELEL(o)->a->h*(o->Absolute.b.x-o->Absolute.a.x)/360;
	rect(buffer,o->Absolute.a.x+x-1,o->Absolute.a.y,o->Absolute.a.x+x+1,o->Absolute.b.y,makecol(0,0,0));
}
////////////////////////////////////////////////////////////////////////////////
void  SalLghSelDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int r,g,b,x=0,y=0;
	while ( x <= o->Absolute.b.x-o->Absolute.a.x ) {
		y=0;
		while ( y <= o->Absolute.b.y-o->Absolute.a.y ) {
			hsv_to_rgb(COLSELEL(o)->a->h,
			((float)(x))/((float)(o->Absolute.b.x-o->Absolute.a.x)),
			((float)(y))/((float)(o->Absolute.b.y-o->Absolute.a.y)),
			&r,&g,&b);
			putpixel(buffer,o->Absolute.a.x+x,o->Absolute.a.y+y,makecol(r,g,b));
			y++;	
		} 
		x++;	
	} 
	x = COLSELEL(o)->a->s*(o->Absolute.b.x-o->Absolute.a.x);
	y = COLSELEL(o)->a->v*(o->Absolute.b.y-o->Absolute.a.y);
	putpixel(buffer,o->Absolute.a.x+x,o->Absolute.a.y+y,makecol(255,255,255));
	rect(buffer,o->Absolute.a.x+x-2,o->Absolute.a.y+y-2,o->Absolute.a.x+x+2,o->Absolute.a.y+y+2,makecol(0,0,0));
	rect(buffer,o->Absolute.a.x+x-3,o->Absolute.a.y+y-3,o->Absolute.a.x+x+3,o->Absolute.a.y+y+3,makecol(255,255,255));
}
////////////////////////////////////////////////////////////////////////////////
void  PaletteSelDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	int x=0,y;
	while ( x < 8 ) {
		y=0;
		while ( y < 6 ) {
			
			rectfill(buffer,
			o->Absolute.a.x+(x*10),
			o->Absolute.a.y+(y*10),
			o->Absolute.a.x+(x*10)+10,
			o->Absolute.a.y+(y*10)+10,
			makecol(Palette[y][x].r,Palette[y][x].g,Palette[y][x].b));

			y++;	
		} 
		
		x++;	
	} 
}
////////////////////////////////////////////////////////////////////////////////
void UpdateColor ( PColSelDat a ) {
	
	a->col->BackgroundColor = makecol(a->r,a->g,a->b);
	LabelSetText(a->l,"r:%d g:%d b:%d\nh:%.0f s:%1.3f v:%1.3f",a->r,a->g,a->b,a->h,a->s,a->v);
	TextBoxSetText(a->th,"%.0f",a->h);
	TextBoxSetText(a->ts,"%1.3f",a->s);
	TextBoxSetText(a->tv,"%1.3f",a->v);
	
	TextBoxSetText(a->tr,"%d",a->r);
	TextBoxSetText(a->tg,"%d",a->g);
	TextBoxSetText(a->tb,"%d",a->b);

	TextBoxSetText(a->thtml,"#%02x%02x%02x",a->r,a->g,a->b);

	WidgetDraw(WIDGET(a->hue), NULL);
	WidgetDraw(WIDGET(a->sallgh), NULL);
	WidgetDraw(WIDGET(a->col), NULL);
	WidgetDraw(WIDGET(a->sr), NULL);
	WidgetDraw(WIDGET(a->sg), NULL);
	WidgetDraw(WIDGET(a->sb), NULL);

}
////////////////////////////////////////////////////////////////////////////////
l_bool  REventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )){
			COLSELEL(o)->a->r = (Mouse->State.p.x-o->Absolute.a.x)*255/(o->Absolute.b.x-o->Absolute.a.x);
			rgb_to_hsv(COLSELEL(o)->a->r,COLSELEL(o)->a->g,COLSELEL(o)->a->b,&COLSELEL(o)->a->h,&COLSELEL(o)->a->s,&COLSELEL(o)->a->v);
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  GEventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )){
			COLSELEL(o)->a->g = (Mouse->State.p.x-o->Absolute.a.x)*255/(o->Absolute.b.x-o->Absolute.a.x);
			rgb_to_hsv(COLSELEL(o)->a->r,COLSELEL(o)->a->g,COLSELEL(o)->a->b,&COLSELEL(o)->a->h,&COLSELEL(o)->a->s,&COLSELEL(o)->a->v);
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  BEventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )){
			COLSELEL(o)->a->b = (Mouse->State.p.x-o->Absolute.a.x)*255/(o->Absolute.b.x-o->Absolute.a.x);
			rgb_to_hsv(COLSELEL(o)->a->r,COLSELEL(o)->a->g,COLSELEL(o)->a->b,&COLSELEL(o)->a->h,&COLSELEL(o)->a->s,&COLSELEL(o)->a->v);
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
l_bool  HueSelEventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )){
			COLSELEL(o)->a->h = (Mouse->State.p.x-o->Absolute.a.x)*360/(o->Absolute.b.x-o->Absolute.a.x);
			hsv_to_rgb(COLSELEL(o)->a->h,COLSELEL(o)->a->s,COLSELEL(o)->a->v,&COLSELEL(o)->a->r,&COLSELEL(o)->a->g,&COLSELEL(o)->a->b);
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  SalLghSelEventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )) {
			COLSELEL(o)->a->s = (float)(Mouse->State.p.x-o->Absolute.a.x)/(float)(o->Absolute.b.x-o->Absolute.a.x);
			COLSELEL(o)->a->v = (float)(Mouse->State.p.y-o->Absolute.a.y)/(float)(o->Absolute.b.y-o->Absolute.a.y);
			hsv_to_rgb(COLSELEL(o)->a->h,COLSELEL(o)->a->s,COLSELEL(o)->a->v,&COLSELEL(o)->a->r,&COLSELEL(o)->a->g,&COLSELEL(o)->a->b);
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  PaletteSelEventHandler ( PWidget o, PEvent Ev ){
	if (Ev->Type == EV_MOUSE){
		if (Ev->Message == WEvMouseLDown || ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )) {
			int x=(Mouse->State.p.x-o->Absolute.a.x)/10,
			y=(Mouse->State.p.y-o->Absolute.a.y)/10;
			
			COLSELEL(o)->a->r = Palette[y][x].r;
			COLSELEL(o)->a->g = Palette[y][x].g;
			COLSELEL(o)->a->b = Palette[y][x].b;
			rgb_to_hsv(COLSELEL(o)->a->r,COLSELEL(o)->a->g,COLSELEL(o)->a->b,&COLSELEL(o)->a->h,&COLSELEL(o)->a->s,&COLSELEL(o)->a->v);
			
			UpdateColor(COLSELEL(o)->a);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
PColSelEl  CreateHueSel ( PApplication App, TRect r, PColSelDat a ) {
	PColSelEl o = malloc(sizeof(TColSelEl));
	if (!o) return NULL;
	memset(o, 0, sizeof(TColSelEl));
	IntialiseWidget(App, StdWidgetClass, WIDGET(o), r);
	WIDGET(o)->Draw = &HueSelDraw;
	WIDGET(o)->EventHandler = &HueSelEventHandler;
	o->a = a;
	WIDGET(o)->Flags |= WFForceBuffer;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PColSelEl CreateSalLghSel ( PApplication App, TRect r, PColSelDat a ) {
	PColSelEl o = malloc(sizeof(TColSelEl));
	if (!o) return NULL;
	memset(o, 0, sizeof(TColSelEl));
	IntialiseWidget(App, StdWidgetClass, WIDGET(o), r);
	WIDGET(o)->Draw = &SalLghSelDraw;
	WIDGET(o)->EventHandler = &SalLghSelEventHandler;
	o->a = a;
	WIDGET(o)->Flags |= WFForceBuffer;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PColSelEl CreateRGBSel ( PApplication App, TRect r, PColSelDat a, l_ulong id ) {
	PColSelEl o = malloc(sizeof(TColSelEl));
	if (!o) return NULL;
	memset(o, 0, sizeof(TColSelEl));
	IntialiseWidget(App, StdWidgetClass, WIDGET(o), r);
	
	
	switch (id ) {
		case 1 : {	
			WIDGET(o)->Draw = &RDraw;
			WIDGET(o)->EventHandler = &REventHandler;
		} break;
		case 2 : {	
			WIDGET(o)->Draw = &GDraw;
			WIDGET(o)->EventHandler = &GEventHandler;
		} break;
		case 3 : {	
			WIDGET(o)->Draw = &BDraw;
			WIDGET(o)->EventHandler = &BEventHandler;
		} break;
	
 	}
	
	
	o->a = a;
	WIDGET(o)->Flags |= WFForceBuffer;
	return o;
}

////////////////////////////////////////////////////////////////////////////////
PColSelEl CreatePaletteSel ( PApplication App, TRect r, PColSelDat a ) {
	PColSelEl o = malloc(sizeof(TColSelEl));
	if (!o) return NULL;
	memset(o, 0, sizeof(TColSelEl));
	IntialiseWidget(App, StdWidgetClass, WIDGET(o), r);
	WIDGET(o)->Draw = &PaletteSelDraw;
	WIDGET(o)->EventHandler = &PaletteSelEventHandler;
	o->a = a;
	WIDGET(o)->Flags |= WFForceBuffer;
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void BackUpFavorites ( PColSelDat aa ) {
	PListItem a,b;
	l_text Key, Val;
	
	DeleteKey("/USER/FAVCOLORS");
	CreateKey("/USER/FAVCOLORS");
	
	if ( !aa->lv->Items->Last ) return;
	
	a = b = aa->lv->Items->Last->Next;
	
	do {
		PColFavItem i = a->Data;
		Val = TextArgs("%02x%02x%02x",i->r,i->g,i->b);
		Key = TextArgs("/USER/FAVCOLORS/%s",i->o.Caption);
		KeySetText(Key, Val);
		free(Key);
		free(Val);
		a = a->Next;
	} while ( a != b );
}
////////////////////////////////////////////////////////////////////////////////
void LoadFavorites ( PColSelDat aa ) {
	PRegKey o = ResolveKey("/USER/FAVCOLORS"),a,b;
	
	if ( !o ) return;
	if ( !o->Last ) return;
	
	a = b = o->Last->Next;
	do {
		
		if ( a->Type == RKT_TEXT ) {
			l_int c = strtol((l_text)a->Data, NULL, 16);
			ColFavAddItem(aa->lv,a->Name,(c >> 16) & 0xFF,(c >> 8) & 0xFF,c & 0xFF);
			
		}
		
		a = a->Next;
	} while ( a != b );

}
////////////////////////////////////////////////////////////////////////////////
#define MSG_APPLYRGB 0xF0030001
#define MSG_APPLYHSL 0xF0030002
#define MSG_APPLYHTML 0xF0030003
#define MSG_ADDFAV 0xF0030004
#define MSG_REMOVE 0xF0030005
#define MSG_SELCOL 0xF0030006
////////////////////////////////////////////////////////////////////////////////
l_bool DialogColor ( RGB *rgb, l_color *col )
{
 	TRect r;
	PTabBook bk;
	PTab t;
 	PLabel l;
 	PButton b;
	l_ulong msg;
 	PColSelDat a = malloc(sizeof(TColSelDat));
 	
 	if ( rgb ) {
		a->r = rgb->r;
		a->g = rgb->g;
		a->b = rgb->b;
 	} else if ( col ) {
		a->r = getr(*col);
		a->g = getg(*col);
		a->b = getb(*col);
 	} else {
		a->r = 255;
		a->g = 255;
		a->b = 0;
	}
	rgb_to_hsv(a->r,a->g,a->b,&a->h,&a->s,&a->v);
	
	
	RectAssign(&r, 0, 0, 265, 270);
	a->w = CreateWindow(&Me, r, "Color selector", WF_CAPTION|WF_FRAME|WF_CENTERED|WF_MINIMIZE);
	InsertWidget(DeskTop, WIDGET(a->w));
	

	RectAssign(&r, 0, 0, 30, 30);
	a->col = CreateWidget(&Me,r);
	a->col->BackgroundColor = makecol(a->r,a->g,a->b);
	InsertWidget(WIDGET(a->w), a->col);
	
	RectAssign(&r, 35, 0, 320,30);
	a->l = CreateLabel(&Me,r,"");
	WIDGET(a->l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(a->w), WIDGET(a->l));

	RectAssign(&r, 0, 35, 265, 240 );
	bk = CreateTabBook(&Me,r);
	InsertWidget(WIDGET(a->w), WIDGET(bk));

	t = TabBookAddTab(bk,"HSV",NULL);

	RectAssign(&r, 0, 0, 255, 20);
	a->hue = CreateHueSel(&Me, r,a);
	InsertWidget(WIDGET(t), WIDGET(a->hue));
	
	RectAssign(&r, 0, 30, 128, 158);
	a->sallgh = CreateSalLghSel(&Me, r,a);
	InsertWidget(WIDGET(t), WIDGET(a->sallgh));
	

	RectAssign(&r, 135, 30, 190,50);
	l = CreateLabel(&Me,r,"Hue");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 195, r.a.y, 255, r.b.y);
	a->th = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->th));
	
	RectAssign(&r, 135, r.b.y+5, 190,r.b.y+25 );
	l = CreateLabel(&Me,r,"Sat");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 195, r.a.y, 255, r.b.y);
	a->ts = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->ts));

	RectAssign(&r, 135, r.b.y+5, 190,r.b.y+25 );
	l = CreateLabel(&Me,r,"Value");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 195, r.a.y, 255, r.b.y);
	a->tv = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->tv));

	RectAssign(&r, 135, r.b.y+5, 190,r.b.y+25 );
	b = CreateButton(&Me,r,"Apply",MSG_APPLYHSL);
	InsertWidget(WIDGET(t), WIDGET(b));

	
	t = TabBookAddTab(bk,"RGB & HTML",NULL);
	
	RectAssign(&r, 0, 0, 55,20);
	l = CreateLabel(&Me,r,"Red");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 60, r.a.y, 123, r.b.y);
	a->tr = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->tr));
	
	RectAssign(&r, 128, r.a.y, 255, r.b.y);
	a->sr = CreateRGBSel(&Me,r,a,1);
	InsertWidget(WIDGET(t), WIDGET(a->sr));
	
	RectAssign(&r, 0, r.b.y+5, 55,r.b.y+25 );
	l = CreateLabel(&Me,r,"Green");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 60, r.a.y, 123, r.b.y);
	a->tg = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->tg));

	RectAssign(&r, 128, r.a.y, 255, r.b.y);
	a->sg = CreateRGBSel(&Me,r,a,2);
	InsertWidget(WIDGET(t), WIDGET(a->sg));

	RectAssign(&r, 0, r.b.y+5, 55,r.b.y+25 );
	l = CreateLabel(&Me,r,"Blue");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 60, r.a.y, 123, r.b.y);
	a->tb = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->tb));
	
	RectAssign(&r, 128, r.a.y, 255, r.b.y);
	a->sb = CreateRGBSel(&Me,r,a,3);
	InsertWidget(WIDGET(t), WIDGET(a->sb));
	
	RectAssign(&r, 0, r.b.y+5, 55,r.b.y+25 );
	b = CreateButton(&Me,r,"Apply",MSG_APPLYRGB);
	InsertWidget(WIDGET(t), WIDGET(b));
	
	RectAssign(&r, 0, r.b.y+5+10, 55,r.b.y+25+10 );
	l = CreateLabel(&Me,r,"HTML");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 60, r.a.y, 123, r.b.y);
	a->thtml = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->thtml));

	RectAssign(&r, 0, r.b.y+5, 55,r.b.y+25 );
	b = CreateButton(&Me,r,"Apply",MSG_APPLYHTML);
	InsertWidget(WIDGET(t), WIDGET(b));
	
	t = TabBookAddTab(bk,"Favorites",NULL);
	
	RectAssign(&r, 5, 5, 255, 100);
	a->lv = CreateListview(&Me,r,"Color name",LVS_LIST,0);
	a->lv->OnValMsg = MSG_SELCOL;
	InsertWidget(WIDGET(t), WIDGET(a->lv));
	
	LoadFavorites(a);
	
	RectAssign(&r, 5, r.b.y+5, 125, r.b.y+25);
	b = CreateButton(&Me,r,"Remove",MSG_REMOVE);
	InsertWidget(WIDGET(t), WIDGET(b));
	
	RectAssign(&r, 130, r.a.y, 255, r.b.y);
	b = CreateButton(&Me,r,"Use selected",MSG_SELCOL);
	InsertWidget(WIDGET(t), WIDGET(b));

	
	RectAssign(&r, 5, r.b.y+5, 200, r.b.y+25);
	a->tfav = CreateTextbox(&Me,r,TBF_EDITABLE);
	InsertWidget(WIDGET(t), WIDGET(a->tfav));
	
	RectAssign(&r, r.b.x+5, r.a.y, 255,r.b.y );
	b = CreateButton(&Me,r,"Add",MSG_ADDFAV);
	InsertWidget(WIDGET(t), WIDGET(b));
	
	t = TabBookAddTab(bk,"Palette",NULL);
	
	RectAssign(&r, 0, 0, 200,20);
	l = CreateLabel(&Me,r,"Classic pallette");
	WIDGET(l)->BackgroundColor = COL_3DFACE;
	InsertWidget(WIDGET(t), WIDGET(l));

	RectAssign(&r, 0, 20, 80, 80);
	a->pal = CreatePaletteSel(&Me, r,a);
	InsertWidget(WIDGET(t), WIDGET(a->pal));
	
	
	RectAssign(&r, 5, 245, 100,265 );
	b = CreateButton(&Me,r,"OK",MSG_OK);
	InsertWidget(WIDGET(a->w), WIDGET(b));
	
	RectAssign(&r, 110, 245, 210,265 );
	b = CreateButton(&Me,r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(a->w), WIDGET(b));

	WidgetDrawAll(WIDGET(a->w));
	TabBookSelectTab(bk,t);
	UpdateColor(a);
	
	
	
	while ( (msg = WidgetExecute(WIDGET(a->w))) != WM_CLOSE )
	{

		switch ( msg )
		{
			
			case MSG_OK:
			{
				if ( rgb ) {
					rgb->r = a->r;
					rgb->g = a->g;
					rgb->b = a->b;
				}
				if ( col ) *col = makecol(a->r,a->g,a->b);
				BackUpFavorites(a);
				WidgetDispose(WIDGET(a->w));
				free(a);
				return true;
				
			} break;

			case MSG_REMOVE:
			{
				PListviewItem i = GetSelectedItem(a->lv);				
				if ( i ) {
					ListviewItemRemoveItem(a->lv,i);
					WidgetDrawAll(WIDGET(a->lv));
					if ( SLLWDG(a->lv)->Vertical ) WidgetDraw(WIDGET(SLLWDG(a->lv)->Vertical), NULL);
				}
			} break;
	
			case MSG_SELCOL:
			{
				PColFavItem i = (PColFavItem)GetSelectedItem(a->lv);
				if ( i ) {
					a->r = i->r;
					a->g = i->g;
					a->b = i->b;
					rgb_to_hsv(a->r,a->g,a->b,&a->h,&a->s,&a->v);
					UpdateColor(a);
				}
			} break;
			
			case MSG_ADDFAV:
			{
				ColFavAddItem(a->lv,a->tfav->Text,a->r,a->g,a->b);
				WidgetDrawAll(WIDGET(a->lv));
				if ( SLLWDG(a->lv)->Vertical ) WidgetDraw(WIDGET(SLLWDG(a->lv)->Vertical), NULL);
			}
			break;			
			case MSG_APPLYHTML:
			{
				if ( TextLen(a->thtml->Text) > 6 ) { 
					l_int c = strtol(a->thtml->Text+1, NULL, 16);
					a->r = (c >> 16) & 0xFF;
					a->g = (c >> 8) & 0xFF;
					a->b = c & 0xFF;
					rgb_to_hsv(a->r,a->g,a->b,&a->h,&a->s,&a->v);
					UpdateColor(a);
				}
			}
			break;			
			
			case MSG_APPLYRGB:
			{
				a->r = atoi(a->tr->Text);
				a->g = atoi(a->tg->Text);
				a->b = atoi(a->tb->Text);
				if ( a->r < 0 ) a->r = 0;
				if ( a->g < 0 ) a->g = 0;
				if ( a->b < 0 ) a->b = 0;
				if ( a->r > 255 ) a->r = 255;
				if ( a->g > 255 ) a->g = 255;
				if ( a->b > 255 ) a->b = 255;
				rgb_to_hsv(a->r,a->g,a->b,&a->h,&a->s,&a->v);
				UpdateColor(a);
			}
			break;
			case MSG_APPLYHSL:
			{
				a->h = atof(a->th->Text);
				a->s = atof(a->ts->Text);
				a->v = atof(a->tv->Text);
				
				if ( a->h < 0 ) a->h = 0;
				if ( a->s < 0 ) a->s = 0;
				if ( a->v < 0 ) a->v = 0;
				
				if ( a->h > 360 ) a->h = 360;
				if ( a->s > 1 ) a->s = 1;
				if ( a->v > 1 ) a->v = 1;
								
				hsv_to_rgb(a->h,a->s,a->v,&a->r,&a->g,&a->b);
				UpdateColor(a);
			}
			break;
		}
		
	}
	
	
	BackUpFavorites(a);
	WidgetDispose(WIDGET(a->w));
	free(a);
	
	
	return false;
	
}
	
	
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	/*RGB rgb;
	DialogColor(&rgb,NULL);*/
	
	APPEXPORT(DialogColor);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////
