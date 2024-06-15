/////////////////////////////////////////////////////////////////////////////////
//
//	Groupbox Core File 
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "groupbox.h"

l_ulong	UID = ULONG_ID('G','R','P','B');
l_ulong	AppVersion = ULONG_ID(0,0,0,1);
l_char	AppName[] = "Groupbox Widget";
l_uid	nUID = "groupbox";
l_uid NeededLibs[] = { "widget", "skin","" };

PSkin SkinGroupbox = 0;

PWidgetClass GroupboxClass;

void GroupboxDraw ( PWidget o, p_bitmap buffer, PRect w )
{
	DrawSkin(buffer, SkinGroupbox, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);

	if ( GROUPBOX(o)->Caption ) {
		rectfill(buffer, o->Absolute.a.x+10, o->Absolute.a.y+1, o->Absolute.a.x+20+text_length(SkinGroupbox->Font, GROUPBOX(o)->Caption), o->Absolute.a.y+2+text_height(SkinGroupbox->Font), getpixel(SkinGroupbox->Skin, SkinGroupbox->Skin->w/2, SkinGroupbox->Skin->h/2));
		textout_ex(buffer, SkinGroupbox->Font, GROUPBOX(o)->Caption, o->Absolute.a.x+18, o->Absolute.a.y+1, SkinGroupbox->FontColor, -1);
	}
}

l_bool GroupboxEventHandler ( PWidget o, PEvent Ev )
{
	return false;
}

PGroupbox CreateGroupbox( PApplication App, TRect r, l_text Caption)
{
	TRect a;
	PGroupbox p = malloc(sizeof(TGroupbox));

	if (!p) return NULL;

	memset(p, 0, sizeof(TGroupbox));

	p->Caption = (l_text)strdup(Caption);

	IntialiseWidget(App, GroupboxClass, WIDGET(p), r);

	WIDGET(p)->Draw = &GroupboxDraw;
	WIDGET(p)->EventHandler = &GroupboxEventHandler;
	
	RectAssign(&a, SkinGroupbox->Left, 3+text_height(SkinGroupbox->Font), r.b.x-r.a.x-SkinGroupbox->Right, r.b.y-r.a.y-SkinGroupbox->Bottom);
	WidgetSetChildArea(WIDGET(p), a );
	
	return p;
}
/*
	Set a real groupbox property 
*/
l_bool	GroupboxSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"caption") ) {
		if ( GROUPBOX(o)->Caption ) free(GROUPBOX(o)->Caption);
		GROUPBOX(o)->Caption = TextDup((l_text)Value);
		if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
		return true;
	}	
	return false;
}
/*
	Get a property from real groupbox  
*/
void	*GroupboxGetRProp ( PWidget o, l_text Name ) {
	if ( !TextCompare(Name,"caption") ) return TextDup(GROUPBOX(o)->Caption);
	return NULL;
	
}

PWidget  StdCreateGroupbox ( PApplication App, TRect r )
{
	return WIDGET(CreateGroupbox(App,r,"Caption"));
}

l_bool LibMain ( int argc, l_text *argv )
{
	GroupboxClass = CreateWidgetClass( ULONG_ID('G','r','p','B'), StdWidgetClass, WCF_CONTAINER );
	GroupboxClass->StdCreate = &StdCreateGroupbox;
	GroupboxClass->SetRProp = &GroupboxSetRProp;
	GroupboxClass->GetRProp = &GroupboxGetRProp;
	
	WClassRegisterProp(GroupboxClass,PTYPE_STRING,"caption");
	
	SkinGroupbox = GetSkinItem("Groupbox");

	APPEXPORT(CreateGroupbox);

	return true;
}

void Close (void)
{

}
