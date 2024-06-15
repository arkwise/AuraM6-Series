////////////////////////////////////////////////////////////////////////////////
//
//  Slider widget - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "slider.h"
////////////////////////////////////////////////////////////////////////////////
l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Slider Widget";
l_uid	nUID = "slider";
l_uid NeededLibs[] = { "widget", "skin","" };
////////////////////////////////////////////////////////////////////////////////
PWidgetClass SliderClass;
PSkin SkinSliderLine	= 0;
PSkin SkinSliderBackground	= 0;
PSkin SkinSliderButton	= 0;
////////////////////////////////////////////////////////////////////////////////
#define DOING_DRAG 1

#define SLIDERWIDTH 	(UseSkins?SkinSliderButton->Skin->w:10)
#define SLIDERHEIGHT 	(UseSkins?SkinSliderButton->Skin->h:20)
////////////////////////////////////////////////////////////////////////////////
TPoint p;
l_int Doing = 0;
////////////////////////////////////////////////////////////////////////////////
void SliderDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	l_long Value = SLIDER(o)->Value;
	l_long Pos = o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-20)*Value)/SLIDER(o)->Steps);

	if ( SLIDER(o)->State == DOING_DRAG ) {
		/*if ( Pos+(Mouse->State.p.x-p.x) <= o->Absolute.b.x-10 ) {
			if ( Pos+(Mouse->State.p.x-p.x) >= o->Absolute.a.x+10 )
				Value = (Mouse->State.p.x-o->Absolute.a.x-10)*SLIDER(o)->Steps /(o->Absolute.b.x-o->Absolute.a.x-20);
			else
				Value = 0;
		} else
			Value = SLIDER(o)->Steps;*/
		Value = SLIDER(o)->PointValue;
		
		
	}
	Pos = o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-20)*Value)/SLIDER(o)->Steps);

	if ( !UseSkins ) {
		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);
		Rect3D(buffer,o->Absolute.a.x+5,(o->Absolute.a.y+o->Absolute.b.y)/2-1,o->Absolute.b.x-5,(o->Absolute.a.y+o->Absolute.b.y)/2+1,COL_3DDARK,COL_3DLIGHT);
		rectfill(buffer,Pos-5,(o->Absolute.a.y+o->Absolute.b.y)/2-10,Pos+5,(o->Absolute.a.y+o->Absolute.b.y)/2+10,COL_3DFACE);
		Rect3D(buffer,Pos-5,(o->Absolute.a.y+o->Absolute.b.y)/2-10,Pos+5,(o->Absolute.a.y+o->Absolute.b.y)/2+10,COL_3DLIGHT,COL_3DDARK);
	} else {
		DrawSkin(buffer, SkinSliderBackground, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
		DrawSkin(buffer, SkinSliderLine, o->Absolute.a.x+10, o->Absolute.a.y+((o->Absolute.b.y-o->Absolute.a.y)/2-SkinSliderLine->Skin->h/2), o->Absolute.b.x-10, o->Absolute.a.y+((o->Absolute.b.y-o->Absolute.a.y)/2+SkinSliderLine->Skin->h/2));
		DrawSkin(buffer, SkinSliderButton, Pos-SkinSliderButton->Skin->w/2, o->Absolute.a.y+SkinSliderBackground->Top, Pos+SkinSliderButton->Skin->w/2, o->Absolute.b.y - SkinSliderBackground->Bottom);
	}
}
////////////////////////////////////////////////////////////////////////////////
l_bool SliderEventHandler ( PWidget o, PEvent Ev )
{
	TRect tmp;

	if (Ev->Type == EV_MOUSE)
	{
		if ( ( Ev->Message == WEvMouseMove ) && (Mouse->State.b & BUTTON_LEFT) && (SLIDER(o)->State == DOING_DRAG) ){
			
			l_long Value = SLIDER(o)->Value;
			l_long Pos = o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-20)*Value)/SLIDER(o)->Steps);
			if ( Pos+(Mouse->State.p.x-p.x) <= o->Absolute.b.x-10 ) {
				if ( Pos+(Mouse->State.p.x-p.x) >= o->Absolute.a.x+10 )
					Value = (Mouse->State.p.x-o->Absolute.a.x-10)*SLIDER(o)->Steps /(o->Absolute.b.x-o->Absolute.a.x-20);
				else
					Value = 0;
			} else
				Value = SLIDER(o)->Steps;
				
			if ( SLIDER(o)->Notify ) SLIDER(o)->Notify(SLIDER(o),SCTX_SLIDING,Value);
		
			SLIDER(o)->PointValue = Value;
		
			WidgetDraw(o, NULL);
			
			return true;
		}

		if ( Ev->Message == WEvMouseLUp ){
			if ( SLIDER(o)->State == DOING_DRAG  ) {
				if (o->Absolute.a.x+10 + (((o->Absolute.b.x-o->Absolute.a.x-20)*(SLIDER(o)->Value))/SLIDER(o)->Steps) + (Mouse->State.p.x-p.x) <= o->Absolute.b.x-10){
					if (o->Absolute.a.x+10 + (((o->Absolute.b.x-o->Absolute.a.x-20)*(SLIDER(o)->Value))/SLIDER(o)->Steps) + (Mouse->State.p.x-p.x) >= o->Absolute.a.x+10)
						SLIDER(o)->Value = (Mouse->State.p.x-o->Absolute.a.x-10)*SLIDER(o)->Steps /(o->Absolute.b.x-o->Absolute.a.x-20);
					else
						SLIDER(o)->Value = 0;
				}
				else
					SLIDER(o)->Value = SLIDER(o)->Steps;
					
				if ( SLIDER(o)->Notify ) SLIDER(o)->Notify(SLIDER(o),SCTX_SET,SLIDER(o)->Value);
					
				MasterWidget = NULL;
				Doing = 0;
				return true;
			}
		}

		if (Ev->Message == WEvMouseLDown)
		{
			RectAssign(&tmp, o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-20)*(SLIDER(o)->Value))/SLIDER(o)->Steps)-SLIDERWIDTH/2, o->Absolute.a.y+((o->Absolute.b.y-o->Absolute.a.y)/2-SLIDERHEIGHT/2), o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-20)*(SLIDER(o)->Value))/SLIDER(o)->Steps)+SLIDERWIDTH/2, o->Absolute.a.y+((o->Absolute.b.y-o->Absolute.a.y)/2+SLIDERHEIGHT/2));
			if (CURSOR_IN_RECT(tmp)){
				p = Mouse->State.p;
				MasterWidget = o;
				SLIDER(o)->State = DOING_DRAG;
				SLIDER(o)->PointValue = SLIDER(o)->Value;
				if ( SLIDER(o)->Notify ) SLIDER(o)->Notify(SLIDER(o),SCTX_START,SLIDER(o)->Value);
				return true;
			}
			else if (Mouse->State.p.x < o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-10)*(SLIDER(o)->Value))/SLIDER(o)->Steps))
			{
				if ( (SLIDER(o)->Value - 1) != -1 )
				{
					SLIDER(o)->Value--;
					if ( SLIDER(o)->Notify ) SLIDER(o)->Notify(SLIDER(o),SCTX_SET,SLIDER(o)->Value);
					WidgetDraw(o, NULL);
					return true;
				}
			}
			else if (Mouse->State.p.x > o->Absolute.a.x+10+(((o->Absolute.b.x-o->Absolute.a.x-10)*(SLIDER(o)->Value))/SLIDER(o)->Steps))
			{
				if ( (SLIDER(o)->Value + 1) <= SLIDER(o)->Steps )
				{
					SLIDER(o)->Value++;
					if ( SLIDER(o)->Notify ) SLIDER(o)->Notify(SLIDER(o),SCTX_SET,SLIDER(o)->Value);
					WidgetDraw(o, NULL);
					return true;
				}
			}
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
PSlider CreateSlider( PApplication App, TRect r, l_ulong Steps )
{
	PSlider p = malloc(sizeof(TSlider));

	if (!p) return NULL;

	memset(p, 0, sizeof(TSlider));

	p->Steps = Steps;

	IntialiseWidget(App, SliderClass, WIDGET(p), r );

	WIDGET(p)->Draw = &SliderDraw;
	WIDGET(p)->EventHandler = &SliderEventHandler;

	return p;
}
////////////////////////////////////////////////////////////////////////////////
l_bool	SliderSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"value") ) {
		SLIDER(o)->Value = *((l_int*)Value);
		if ( SLIDER(o)->Value > SLIDER(o)->Steps ) SLIDER(o)->Value = SLIDER(o)->Steps;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if ( !TextCompare(Name,"max") ) {
		SLIDER(o)->Steps = *((l_int*)Value);
		if ( SLIDER(o)->Value > SLIDER(o)->Steps ) SLIDER(o)->Value = SLIDER(o)->Steps;
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}	
	
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void	*SliderGetRProp ( PWidget o, l_text Name ) {
	if ( !TextCompare(Name,"value") ) return DuplicateInt(&(SLIDER(o)->Value));
	if ( !TextCompare(Name,"max") ) return DuplicateInt(&(SLIDER(o)->Steps));
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateSlider ( PApplication App, TRect r )
{
	return WIDGET(CreateSlider(App,r,10));
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain ( int argc, l_text *argv )
{
	SliderClass = CreateWidgetClass( ULONG_ID('S','l','d','r'), StdWidgetClass, 0 );
	SliderClass->StdCreate = &StdCreateSlider;
	SliderClass->SetRProp = &SliderSetRProp;
	SliderClass->GetRProp = &SliderGetRProp;

	WClassRegisterProp(SliderClass,PTYPE_INT,"value");
	WClassRegisterProp(SliderClass,PTYPE_INT,"max");
	
	/**
	*	Skin items sizes
	*/
	SkinSliderLine			= GetSkinItem("SliderLine");
	SkinSliderButton		= GetSkinItem("SliderButton");
	SkinSliderBackground	= GetSkinItem("SliderBackground");

	APPEXPORT(CreateSlider);
/*
	TRect r; RectAssign(&r, 750,300,1000,340);
	PSlider p = CreateSlider(&Me, r);
	InsertWidget(WIDGET(DeskTop), WIDGET(p));
	WidgetDraw(WIDGET(p), NULL);
*/
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{

}
////////////////////////////////////////////////////////////////////////////////
