////////////////////////////////////////////////////////////////////////////////
//
//  Label Widget - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "label.h"
#include "gfx.h"
#include "internal.h"

l_ulong	AppVersion = ULONG_ID(0,2,0,0);
l_char	AppName[] = "Label Widget";
l_uid	nUID = "label";
l_uid NeededLibs[] = { "widget", "grfx", "" };

PWidgetClass LabelClass;
PWidgetClass ImageLabelClass;

void LabelDraw ( PWidget o, p_bitmap buffer, PRect w )
{

	rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, o->BackgroundColor);
	
	if ( LABEL(o)->Caption )
	{
		l_text t  = LABEL(o)->Caption;
		l_text lt = LABEL(o)->Caption;
		l_char tmp = 0;
		l_ulong y = o->Absolute.a.y;
		l_ulong ln = 0;
		l_ulong h = text_height(default_font);

		while ( t )
		{
			t = TextChr(t,'\n');
			if ( t ) t++;
			ln++;
		}


		t  = LABEL(o)->Caption;

		while ( t )
		{
			lt = t;

			t = TextChr(t,'\n');

  		if ( t ) {
				DrawTextEx(buffer,default_font,o->Absolute.a.x, y,lt,t,LABEL(o)->FontColor,true);
				t++;
				y += h;
			} else
			  textout_ex(buffer, default_font, lt, o->Absolute.a.x, y, LABEL(o)->FontColor, -1);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void LabelSetTextEx ( PLabel l, l_text Caption ){
  if ( l->Caption ) free(l->Caption);
	l->Caption = TextDup(Caption);
}
////////////////////////////////////////////////////////////////////////////////
void LabelSetText ( PLabel l, l_text Format, ... ) {
  va_list arg;
  va_start(arg, Format);
  vsprintf(CommonBufferText, Format, arg);
  LabelSetTextEx(l,CommonBufferText);
  WidgetDraw(WIDGET(l),NULL);
  va_end(arg);
}
////////////////////////////////////////////////////////////////////////////////
PLabel CreateLabel( PApplication App, TRect r, l_text Caption )
{
	PLabel b = malloc(sizeof(TLabel));
	if (!b) return NULL;
	memset(b,0,sizeof(TLabel));
	b->Caption = TextDup(Caption);
	b->FontColor = makecol(0,0,0);

	IntialiseWidget(App, LabelClass, WIDGET(b), r);
	WIDGET(b)->Draw = &LabelDraw;
	WIDGET(b)->BackgroundColor = COL_3DFACE;
	return b;
}
////////////////////////////////////////////////////////////////////////////////
void ImageLabelDraw ( PWidget o, p_bitmap buffer, PRect w )
{

	rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, o->BackgroundColor);

	if ( IMAGELABEL(o)->Ico ) draw_sprite(buffer,IMAGELABEL(o)->Ico,o->Absolute.a.x,o->Absolute.a.y);

	if ( IMAGELABEL(o)->Flags & ILF_FRAME ) Rect3D(buffer, o->Absolute.a.x,o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, COL_3DLIGHT, COL_3DDARK);
}
////////////////////////////////////////////////////////////////////////////////
PImageLabel CreateImageLabel( PApplication App, TRect r, BITMAP *Ico )
{
	PImageLabel b = malloc(sizeof(TImageLabel));
	if (!b) return NULL;
	memset(b,0,sizeof(TImageLabel));
	b->Ico = Ico;
	IntialiseWidget(App, ImageLabelClass, WIDGET(b), r );
	WIDGET(b)->Draw = &ImageLabelDraw;
	WIDGET(b)->BackgroundColor = COL_3DFACE;

	return b;
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateImageLabel ( PApplication App, TRect r )
{
	return WIDGET(CreateImageLabel(App,r,NULL));
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateLabel ( PApplication App, TRect r )
{
	return WIDGET(CreateLabel(App,r,NULL));
}
////////////////////////////////////////////////////////////////////////////////
l_bool	LabelSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"caption") ) {
		if ( LABEL(o)->Caption ) free(LABEL(o)->Caption);
		LABEL(o)->Caption = TextDup((l_text)Value);
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	return false;
}

void	*LabelGetRProp ( PWidget o, l_text Name ) {
	if ( !TextCompare(Name,"caption") ) return TextDup(LABEL(o)->Caption);
	return NULL;
	
}

////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	LabelClass = CreateWidgetClass( ULONG_ID('L','b','l',' '), StdWidgetClass, 0 );
	LabelClass->StdCreate = &StdCreateLabel;
	LabelClass->SetRProp = &LabelSetRProp;
	LabelClass->GetRProp = &LabelGetRProp;

	WClassRegisterProp(LabelClass,PTYPE_STRING,"caption");

	ImageLabelClass = CreateWidgetClass( ULONG_ID('L','b','l','I'), StdWidgetClass, 0 );
	ImageLabelClass->StdCreate = &StdCreateImageLabel;

	
	
	APPEXPORT(CreateLabel);
	APPEXPORT(LabelSetText);
	APPEXPORT(LabelSetTextEx);
	APPEXPORT(CreateImageLabel);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void) {

}
