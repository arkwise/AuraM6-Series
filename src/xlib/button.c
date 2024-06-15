/////////////////////////////////////////////////////////////////////////////////
//
//	Button Core File
//
//	Aura Milestone 4
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "button.h"
#include "grfx.h"
#include "internal.h"

l_ulong AppVersion = ULONG_ID(0, 10, 0, 0);
l_char AppName[] = "Button Widget";
l_uid nUID = "button";
l_uid NeededLibs[] = {"widget", "skin", "grfx", ""};

/** @defgroup button Button widget
	@ingroup widgets
	@note Text is center by default.
	@{
*/

/**	@file
	Button core file.
*/

PSkin SkinButtonUp = 0;
PSkin SkinButtonDown = 0;
PSkin SkinButtonOver = 0;
PSkin SkinButtonDisabled = 0;

PWidgetClass ButtonClass;

/*
*	NAME: ButtonDraw
*	DESCRIPTION: Default button draw function
*/
_PUBLIC void ButtonDraw(PWidget o, p_bitmap buffer, PRect w)
{
	FONT *Font = default_font;
	l_color FontColor = COL_3DTEXT;
	l_int OffsetX = 5;
	l_int OffsetY = 3;
	l_int Shift = 0;
	p_bitmap Icon = BUTTON(o)->Icon;

	if (BUTTON(o)->Flags & BF_DISABLED)
	{
		if (Icon)
			Icon = ImageNewGreyScaleWithMask(Icon);
	}

	if (UseSkins)
	{
		PSkin SkinItem = SkinButtonUp;

		if (BUTTON(o)->State == BS_OVER)
			SkinItem = SkinButtonOver;
		if (BUTTON(o)->Flags & BF_DISABLED)
			SkinItem = SkinButtonDisabled;
		if (BUTTON(o)->State == BS_DOWN)
		{
			SkinItem = SkinButtonDown;
			Shift = 1;
		}

		DrawSkin(buffer, SkinItem, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

		FontColor = SkinItem->FontColor;
		Font = SkinItem->Font;
		OffsetX = SkinItem->Left;
		OffsetY = SkinItem->Top;
	}
	else
	{
		l_color Face = COL_3DFACE;
		l_color Light = COL_3DLIGHT;
		l_color Dark = COL_3DDARK;

		if (BUTTON(o)->State == BS_OVER)
			Face = COL_3DOVER;
		if (BUTTON(o)->State == BS_DOWN)
		{
			Light = COL_3DDARK;
			Dark = COL_3DLIGHT;
			Shift = 1;
		}

		rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, Face);
		Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y, Light, Dark);
	}

	if (BUTTON(o)->Caption)
	{
		if (Icon)
		{
			if (BUTTON(o)->Flags & BF_TEXTUNDERICON)
			{
				draw_sprite(buffer, Icon, (o->Absolute.b.x + o->Absolute.a.x) / 2 - (Icon->w / 2) + Shift, (o->Absolute.a.y + o->Absolute.b.y) / 2 + Shift - (Icon->h + OffsetY + text_height(Font)) / 2);
				textout_centre_ex(buffer, Font, BUTTON(o)->Caption, (o->Absolute.b.x + o->Absolute.a.x) / 2 + Shift, (o->Absolute.a.y + o->Absolute.b.y) / 2 + Shift - (Icon->h + OffsetY + text_height(Font)) / 2 + Icon->h + OffsetY, FontColor, -1);
			}
			else if (BUTTON(o)->Flags & BF_ALIGNLEFT)
			{
				draw_sprite(buffer, Icon, o->Absolute.a.x + OffsetX + Shift, (o->Absolute.b.y + o->Absolute.a.y - Icon->h) / 2 + Shift);
				textout_ex(buffer, Font, BUTTON(o)->Caption, o->Absolute.a.x + OffsetX + Shift + Icon->w + OffsetX, (o->Absolute.a.y + o->Absolute.b.y - text_height(Font)) / 2 + Shift, FontColor, -1);
			}
			else
			{
				draw_sprite(buffer, Icon, (o->Absolute.a.x + o->Absolute.b.x) / 2 + Shift - (Icon->w + OffsetX + text_length(Font, BUTTON(o)->Caption)) / 2, (o->Absolute.b.y + o->Absolute.a.y - Icon->h) / 2 + Shift);
				textout_ex(buffer, Font, BUTTON(o)->Caption, (o->Absolute.a.x + o->Absolute.b.x) / 2 + Shift - (Icon->w + OffsetX + text_length(Font, BUTTON(o)->Caption)) / 2 + Icon->w + OffsetX, (o->Absolute.a.y + o->Absolute.b.y - text_height(Font)) / 2 + Shift, FontColor, -1);
			}
		}
		else
			textout_centre_ex(buffer, Font, BUTTON(o)->Caption, (o->Absolute.b.x + o->Absolute.a.x) / 2 + Shift, (o->Absolute.a.y + o->Absolute.b.y - text_height(Font)) / 2 + Shift, FontColor, -1);
	}
	else if (!BUTTON(o)->Caption && Icon)
	{
		draw_sprite(buffer, Icon, (o->Absolute.b.x + o->Absolute.a.x - Icon->w) / 2 + Shift, (o->Absolute.b.y + o->Absolute.a.y - Icon->h) / 2 + Shift);
	}
}

/*
*	NAME: ButtonEventHandler
*	DESCRIPTION: Default event handler for button
*/
_PUBLIC l_bool ButtonEventHandler(PWidget o, PEvent Ev)
{
	if (Ev->Type == EV_MOUSE)
	{
		if ((BUTTON(o)->Flags & BF_DISABLED))
			return true;

		if (Ev->Message == WEvMouseEnter)
		{
			BUTTON(o)->State = BS_OVER;
			WidgetDraw(o, NULL);
			return true;
		}

		if (Ev->Message == WEvMouseLeft)
		{
			BUTTON(o)->State = 0;
			WidgetDraw(o, NULL);
			return true;
		}

		if (Ev->Message == WEvMouseLDown)
		{
			BUTTON(o)->State = BS_DOWN;
			WidgetDraw(o, NULL);
			return true;
		}

		if (Ev->Message == WEvMouseLUp && BUTTON(o)->State == BS_DOWN)
		{
			BUTTON(o)->State = BS_OVER;
			WidgetDraw(o, NULL);
			if (BUTTON(o)->Message)
				WidgetSendSEvent(o, EV_MESSAGE, BUTTON(o)->Message, BUTTON(o)->MessageExtra);
			return true;
		}
	}

	return false;
}

void ButtonFreeEx(PWidget o)
{
	if (BUTTON(o)->Caption)
		free(BUTTON(o)->Caption);
}

/**
	Change the caption of a button
	@param b Button to affect
	@param Caption New caption
	@note Redraw the button
	@see CreateButton
*/
void ButtonSetCaption(PButton b, l_text Caption)
{
	if (b->Caption)
		free(b->Caption);
	b->Caption = TextDup(Caption);
	WidgetDraw(WIDGET(b), NULL);
}

void ButtonSetCaptionEx(PButton b, l_text Format, ...)
{
	va_list arg;
	va_start(arg, Format);
	vsprintf(CommonBufferText, Format, arg);
	ButtonSetCaption(b, CommonBufferText);
	WidgetDraw(WIDGET(b), NULL);
	va_end(arg);
}

/** 
	Creates a new button
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param r Relative coordinates of new widget
	@param Caption Caption of button
	@param Message Message send when button is clicked
	@see WidgetInsert
	@see buttonflags
*/
_PUBLIC PButton CreateButton(PApplication App, TRect r, l_text Caption, l_ulong Message)
{
	PButton b = malloc(sizeof(TButton));

	if (!b)
		return NULL;

	memset(b, 0, sizeof(TButton));

	if (Caption)
		b->Caption = TextDup(Caption);

	b->Message = Message;

	IntialiseWidget(App, ButtonClass, WIDGET(b), r);

	WIDGET(b)->Draw = &ButtonDraw;
	WIDGET(b)->EventHandler = &ButtonEventHandler;
	WIDGET(b)->FreeEx = &ButtonFreeEx;

	return b;
}

PWidget StdCreateButton(PApplication App, TRect r)
{
	return WIDGET(CreateButton(App, r, NULL, 0));
}

l_bool ButtonSetRProp(PWidget o, l_text Name, void *Value, l_bool Redraw)
{
	if (!TextCompare(Name, "caption"))
	{
		if (BUTTON(o)->Caption)
			free(BUTTON(o)->Caption);
		BUTTON(o)->Caption = TextDup((l_text)Value);
		if (Redraw)
			WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	if (!TextCompare(Name, "message"))
	{
		BUTTON(o)->Message = *((l_ulong *)Value);
		return true;
	}
	return false;
}

void *ButtonGetRProp(PWidget o, l_text Name)
{
	if (!TextCompare(Name, "caption"))
		return TextDup(BUTTON(o)->Caption);
	if (!TextCompare(Name, "message"))
		return DuplicateInt(&(BUTTON(o)->Message));
	return NULL;
}

/**
@}
*/

l_bool LibMain(int argc, l_text *argv)
{
	ButtonClass = CreateWidgetClass(ULONG_ID('B', 't', 'n', ' '), StdWidgetClass, 0);
	ButtonClass->StdCreate = &StdCreateButton;
	ButtonClass->SetRProp = &ButtonSetRProp;
	ButtonClass->GetRProp = &ButtonGetRProp;

	WClassRegisterProp(ButtonClass, PTYPE_STRING, "caption");
	WClassRegisterProp(ButtonClass, PTYPE_MESSAGE, "message");

	SkinButtonUp = GetSkinItem("Button");
	SkinButtonDown = GetSkinItem("ButtonClick");
	SkinButtonOver = GetSkinItem("ButtonOver");
	SkinButtonDisabled = GetSkinItem("ButtonDisabled");

	APPEXPORT(CreateButton);
	APPEXPORT(ButtonSetCaption);
	APPEXPORT(ButtonSetCaptionEx);
	return true;
}

void Close(void)
{
}
