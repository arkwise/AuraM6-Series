/////////////////////////////////////////////////////////////////////////////////
//
//	Textbox Core File 
//
//	Aura Milestone 3 
//
//	(c) Copyright 2012 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "widget.h"
#include "tbox.h"
#include "label.h"
#include "sbar.h"
#include "menu.h"

////////////////////////////////////////////////////////////////////////////////
l_ulong	AppVersion = ULONG_ID(0,1,1,0);
l_char	AppName[] = "Textbox Widget";
l_uid	nUID = "textbox";
l_uid NeededLibs[] = { "widget", "clipbrd","grfx","skin", "menu", "scrllbar", "" };

PSkin SkinTextbox = 0;
PWidgetClass TextboxClass;

////////////////////////////////////////////////////////////////////////////////
void TextBoxGetTextArea ( PTextbox o, PRect r ) {
	if ( UseSkins )
		RectAssign(r,
			WIDGET(o)->Absolute.a.x+SkinTextbox->Left,
			WIDGET(o)->Absolute.a.y+SkinTextbox->Top,
			WIDGET(o)->Absolute.b.x-SkinTextbox->Right,
			WIDGET(o)->Absolute.b.y-SkinTextbox->Bottom);
	else
		RectAssign(r,
			WIDGET(o)->Absolute.a.x+2,
			WIDGET(o)->Absolute.a.y+2,
			WIDGET(o)->Absolute.b.x-2,
			WIDGET(o)->Absolute.b.y-2);
}
////////////////////////////////////////////////////////////////////////////////
void Scroll ( PScrollWidget o, TPoint p ) {
	if ( WIDGET(o)->Flags & WFForceBuffer )
		WidgetDraw(WIDGET(o),NULL);
	else {
		WIDGET(o)->Flags |= WFForceBuffer;
		WidgetDraw(WIDGET(o),NULL);
		WIDGET(o)->Flags &= ~WFForceBuffer;
	}
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxDraw ( PWidget o, p_bitmap buffer, PRect w ) {
	TRect a,t;
	
	TextBoxGetTextArea(TEXTBOX(o),&a);
	
	if ( UseSkins )
		DrawSkin(buffer, SkinTextbox, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y);
	else {
		rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_BOXFACE);
		Rect3D(buffer, o->Absolute.a.x, o->Absolute.a.y, o->Absolute.b.x, o->Absolute.b.y,COL_BOXLIGHT,COL_BOXDARK);
	}
	
	GetClip(buffer,&t);
	SetClip(buffer,a);
	set_clip_state(buffer, TRUE);

	
	if ( TEXTBOX(o)->Text ) {
		l_int x = a.a.x-SLLWDG(o)->Pos.x, y = (a.a.y)-SLLWDG(o)->Pos.y;
		l_text t = TEXTBOX(o)->Text;
		l_text f = t;
		l_text ss = min(TEXTBOX(o)->SelPosEnd,TEXTBOX(o)->SelPos);
		l_text se = max(TEXTBOX(o)->SelPosEnd,TEXTBOX(o)->SelPos);
		l_bool end = 0;
		while ( !end ) {
			f = TextChr(t,'\n');
			if ( !f ) { f = t+TextLen(t); end = 1; }
			if ( y >= w->a.y - text_height(TEXTBOX(o)->Font) && y <= w->b.y ) {
				if ( !( t > se || f < ss ) ) {
					if ( se == ss ) {
						if ( !(TEXTBOX(o)->Flags & TBF_TIMERHIDECURSOR) ) {
							l_int x1 = x + FontTextLengthEx(TEXTBOX(o)->Font,t,ss,TEXTBOX(o)->Flags & TBF_PASSWORD);
							vline(buffer,x1,y,y+text_height(TEXTBOX(o)->Font),makecol(128,128,255));
						}
					} else {
						l_text a = max(ss,t), b = min(se,f);
						l_int x1 = x + FontTextLengthEx(TEXTBOX(o)->Font,t,a,TEXTBOX(o)->Flags & TBF_PASSWORD),
									x2 = x + FontTextLengthEx(TEXTBOX(o)->Font,t,b,TEXTBOX(o)->Flags & TBF_PASSWORD);
						rectfill(buffer,x1,y,x2,y+text_height(TEXTBOX(o)->Font),makecol(128,128,255));
						if ( TEXTBOX(o)->Flags & TBF_TIMERHIDECURSOR ) {
							if ( !( t > TEXTBOX(o)->SelPosEnd || f < TEXTBOX(o)->SelPosEnd ) ) {
								l_int x1 = x + FontTextLengthEx(TEXTBOX(o)->Font,t,TEXTBOX(o)->SelPosEnd,TEXTBOX(o)->Flags & TBF_PASSWORD);
								vline(buffer,x1,y,y+text_height(TEXTBOX(o)->Font),makecol(255,255,255));
							}
						}

					}
				}
				if ( o->Flags & WF_DRAGFOCUS )
					if ( !( t > TEXTBOX(o)->DragPos || f < TEXTBOX(o)->DragPos ) ) {
						l_int x1 = x + FontTextLengthEx(TEXTBOX(o)->Font,t,TEXTBOX(o)->DragPos,TEXTBOX(o)->Flags & TBF_PASSWORD);
						vline(buffer,x1,y,y+text_height(TEXTBOX(o)->Font),makecol(128,128,128));
					}
				DrawTextEx(buffer,TEXTBOX(o)->Font,x,y,t,f,makecol(0,0,0),TEXTBOX(o)->Flags & TBF_PASSWORD);
			}
			y += text_height(TEXTBOX(o)->Font)+1;
			t = f+1;
		}
	}
	SetClip(buffer,t);
	set_clip_state(buffer, TRUE);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxWriteChar ( PTextbox o , l_char c ) {
	l_text Temp = TextDup(o->SelPosEnd);
	l_text Ex = realloc(o->Text,TextLen(o->Text)+2+o->SelPos-o->SelPosEnd);
	TextCopy(Ex+(o->SelPos-o->Text+1),Temp);
  free(Temp);
	*(Ex+(o->SelPos-o->Text)) = c;
	o->SelPos = Ex+(o->SelPos-o->Text+1);
	o->SelPosEnd = o->SelPos;
	o->DragPos = o->Text;
	o->Text = Ex;
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxInserText ( PTextbox o , l_text Txt ) {
	l_ulong l = TextLen(Txt);
	l_text Temp = TextDup(o->SelPosEnd);
	l_text Ex = realloc(o->Text,TextLen(o->Text)+1+l+o->SelPos-o->SelPosEnd);
	TextCopy(Ex+(o->SelPos-o->Text),Txt);
	TextCopy(Ex+(o->SelPos-o->Text+l),Temp);
  	free(Temp);
	o->SelPos = Ex+(o->SelPos-o->Text+l);
	o->SelPosEnd = o->SelPos;
	o->DragPos = Ex;
	o->Text = Ex;
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxDeleteSelection ( PTextbox o ) {
	l_text Temp = TextDup(o->SelPosEnd);
	l_text Ex = realloc(o->Text,TextLen(o->Text)+o->SelPos-o->SelPosEnd+1);
	TextCopy(Ex+(o->SelPos-o->Text),Temp);
  	free(Temp);
  	if ( o->DragPos >= o->SelPos ) {
	  	if ( o->DragPos <= o->SelPosEnd )
	  		o->DragPos = Ex+(o->SelPos-o->Text);
		else
	  		o->DragPos = Ex+(o->SelPos-o->Text+o->DragPos-o->SelPosEnd);
	} else
		o->DragPos = Ex+(o->DragPos-o->Text);
	o->SelPos = Ex+(o->SelPos-o->Text);
	o->SelPosEnd = o->SelPos;
	o->Text = Ex;
}
////////////////////////////////////////////////////////////////////////////////
l_text TextBoxGetSelection ( PTextbox o ) {
	return TextNDup(o->SelPos,o->SelPosEnd-o->SelPos);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxDeleteMove ( PTextbox o, l_char Pos, l_char EndPos ) {
	l_text Temp = TextDup(o->SelPosEnd+EndPos);
	l_text Ex = realloc(o->Text,TextLen(o->Text)+o->SelPos-o->SelPosEnd);
	TextCopy(Ex+(o->SelPos-o->Text)+Pos,Temp);
  	free(Temp);
	o->SelPos = Ex+(o->SelPos-o->Text+Pos);
	o->SelPosEnd = o->SelPos;
	o->DragPos = Ex;
	o->Text = Ex;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextBoxCalculateMax ( PTextbox o ) {
	l_int x = 0, y = 0;
	TPoint Old = SLLWDG(o)->Max;
	
	TRect a;
	TextBoxGetTextArea(TEXTBOX(o),&a);

	
	if ( o->Text ) {
		l_text t = o->Text,f = t;
		l_bool end = 0;
		while ( !end ) {
			f = TextChr(t,'\n');
			if ( !f ) end = 1;
			y += text_height(o->Font)+1;
			x = max(x,FontTextLengthEx(o->Font,t,f,TEXTBOX(o)->Flags & TBF_PASSWORD));
			t = f+1;
		}
	}

	if ( y > a.b.y-a.a.y )
		SLLWDG(o)->Max.y = y - (a.b.y-a.a.y);
	else
		SLLWDG(o)->Max.y = 0;

	if ( x > a.b.x-a.a.x )
		SLLWDG(o)->Max.x = x - (a.b.x-a.a.x);
	else
		SLLWDG(o)->Max.x = 0;

	if ( Old.x == SLLWDG(o)->Max.x && Old.y == SLLWDG(o)->Max.y ) return false;

	if ( SLLWDG(o)->Pos.x > SLLWDG(o)->Max.x  ) ScrollBarSetPosX(SLLWDG(o),SLLWDG(o)->Max.x);
	if ( SLLWDG(o)->Pos.y > SLLWDG(o)->Max.y  ) ScrollBarSetPosY(SLLWDG(o),SLLWDG(o)->Max.y);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_text ReachStartOfLine ( l_text t, l_text l ) {
	l--;
	while ( l > t ) {
		if ( *l == '\n' ) return l+1;
		l--;
	}
	return t;
}
////////////////////////////////////////////////////////////////////////////////
void CheckSelVisibility ( PTextbox o, l_bool RedrawScll ) {
	
	TRect a;
	TextBoxGetTextArea(TEXTBOX(o),&a);
	
	
	if ( SLLWDG(o)->Max.x || SLLWDG(o)->Max.y ) {
		l_long x,y;
		LocatePointText(o->Font,o->Text,o->SelPosEnd,&y,&x,TEXTBOX(o)->Flags & TBF_PASSWORD);
		x -= SLLWDG(o)->Pos.x;
		y -= SLLWDG(o)->Pos.y;
		if ( x < 0 ) {
			ScrollBarSetPosX(SLLWDG(o),max(0,SLLWDG(o)->Pos.x+x));
			if ( RedrawScll ) WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);
		} else if ( x >= a.b.x-a.a.x ) {
			ScrollBarSetPosX(SLLWDG(o),min(SLLWDG(o)->Max.x,SLLWDG(o)->Pos.x+x-(a.b.x-a.a.x)));
			if ( RedrawScll ) WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);
		}
		if ( y < 0 ) {
			ScrollBarSetPosY(SLLWDG(o),max(0,SLLWDG(o)->Pos.y+y));
			if ( RedrawScll ) WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
		} else {
			y += text_height(o->Font);
			if ( y >= a.b.y-a.a.y ) {
				ScrollBarSetPosY(SLLWDG(o),min(SLLWDG(o)->Max.y,SLLWDG(o)->Pos.y+y-(a.b.y-a.a.y)));
				if ( RedrawScll ) WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
			}
		}
	}
	
}

////////////////////////////////////////////////////////////////////////////////
void TextBoxRefreshFromTo ( PTextbox o, l_text st, l_text fn ) {
	TRect r;
	
	TRect a;
	TextBoxGetTextArea(TEXTBOX(o),&a);
	
	if ( st > fn ) {
		l_text t = fn;
		fn = st;
		st = t;
	}

	LocatePointText(o->Font,o->Text,st,(l_ulong*)&r.a.y,(l_ulong*)&r.a.x,TEXTBOX(o)->Flags & TBF_PASSWORD);
	LocatePointText(o->Font,o->Text,fn,(l_ulong*)&r.b.y,(l_ulong*)&r.b.x,TEXTBOX(o)->Flags & TBF_PASSWORD);
	r.b.x += FontCharLength(o->Font,(l_int)*fn);
	if ( r.a.y != r.b.y ) {
		r.a.x = a.a.x;
		r.b.x = a.b.x;
	} else {
		r.a.x = max(a.a.x-SLLWDG(o)->Pos.x+r.a.x,a.a.x);
		r.b.x = min(a.b.x-SLLWDG(o)->Pos.x+r.b.x,a.b.x);
	}
	r.b.y += text_height(o->Font);
	r.a.y = max(a.a.y-SLLWDG(o)->Pos.y+r.a.y,a.a.y);
	r.b.y = max(a.b.y-SLLWDG(o)->Pos.y+r.b.y,a.b.y);
	WidgetDraw(WIDGET(o), &r);


}
////////////////////////////////////////////////////////////////////////////////
l_bool TextBoxDropAccept ( PWidget o, PWidget Caller, l_ulong Type, void *Data, l_ulong SupportedMethods, l_ulong *Method ) {
	if ( (SupportedMethods & DMETHOD_MOVE) && (Type == DTYPE_TEXT) ) {
		*Method = DMETHOD_MOVE;
		return true;	
	}
	if ( (SupportedMethods & DMETHOD_COPY) && (Type == DTYPE_TEXT) ) {
		*Method = DMETHOD_COPY;
		return true;	
	}	
	return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextBoxDragBeforeDrop    ( PWidget o, PWidget Dest, void *Data, l_ulong Type, l_ulong Method ) {
	if ( o == Dest ) if ( (TEXTBOX(o)->DragPos >= TEXTBOX(o)->SelPos) && (TEXTBOX(o)->DragPos <= TEXTBOX(o)->SelPosEnd) ) return false;
	if ( Method == DMETHOD_MOVE ) TextBoxDeleteSelection(TEXTBOX(o));
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxDropData    ( PWidget o, PWidget Caller, void *Data, l_ulong Type, l_ulong Method ) {
	if ( o == Caller ) if ( (TEXTBOX(o)->DragPos >= TEXTBOX(o)->SelPos) && (TEXTBOX(o)->DragPos <= TEXTBOX(o)->SelPosEnd) ) return;
	TEXTBOX(o)->SelPos = TEXTBOX(o)->DragPos;
	TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->DragPos;
	TextBoxInserText(TEXTBOX(o),Data);
	WidgetDraw(o, NULL);	
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxDragAfterDrop ( PWidget o, PWidget Dest, void*Data, l_ulong Type, l_ulong Method ) {
	if ( o != Dest )
		WidgetDraw(o, NULL);	
}
////////////////////////////////////////////////////////////////////////////////
l_bool TextBoxEventHandler ( PWidget o, PEvent Ev )
{
	
	if ( Ev->Type == EV_MESSAGE ){
		if ( Ev->Message == MSG_COPY ){
			if ( !( TEXTBOX(o)->Flags & TBF_PASSWORD ) )
			ClipboardPut(TYPE_TEXT,TextBoxGetSelection(TEXTBOX(o)),0);
		}
		else if ( (Ev->Message == MSG_CUT) && (TEXTBOX(o)->Flags & TBF_EDITABLE) ){
			if ( !( TEXTBOX(o)->Flags & TBF_PASSWORD ) )
			ClipboardPut(TYPE_TEXT,TextBoxGetSelection(TEXTBOX(o)),0);
			TextBoxDeleteSelection(TEXTBOX(o));
			WidgetDraw(o, NULL);
		}
		else if ( (Ev->Message == MSG_PASTE) && (TEXTBOX(o)->Flags & TBF_EDITABLE) ){
			TextBoxInserText(TEXTBOX(o),(l_text)ClipboardGet(TYPE_TEXT));
			WidgetDraw(o, NULL);
		}
		else if ( Ev->Message == MSG_DELETE ){
			TextBoxDeleteSelection(TEXTBOX(o));
			WidgetDraw(o, NULL);
		} else if ( Ev->Message == WEvNotifyFocused ){
			TEXTBOX(o)->Flags &= ~TBF_TIMERHIDECURSOR;
			WidgetDraw(o, NULL);
	 	} else if ( Ev->Message == WEvNotifyLostFocus ){
			TEXTBOX(o)->Flags |= TBF_TIMERHIDECURSOR;
			WidgetDraw(o, NULL);
		} else if ( Ev->Message == MSG_NOTIFY_AUTO_RESIZE ) {
			TextBoxCalculateMax(TEXTBOX(o));
			return true;
		}

		
		return true;
	}


	if ( Ev->Type == EV_MOUSE )
	{
		l_text t = TEXTBOX(o)->Text;
		
		TRect a;
		TextBoxGetTextArea(TEXTBOX(o),&a);
	
		if ( Ev->Message == WEvMouseLeft )
		{ 
			TEXTBOX(o)->Flags &= ~TBF_PREDRAG;
			return true;
		}
		if ( Ev->Message == MSG_DRAGCANCEL )
		{ 
			WidgetDraw(o, NULL);
			return true;
		}
	
		if ( (Ev->Message == WEvMouseLeft) && (o->Flags & WF_DRAGFOCUS) )
		{ 
			WidgetDraw(o, NULL);
			return true;
		}
		
		if ( (Ev->Message == WEvMouseMove) && (o->Flags & WF_DRAGFOCUS) )
		{ 
			l_text w = WhereInTextMultiLines(default_font,Mouse->State.p.x+SLLWDG(o)->Pos.x-a.a.x,Mouse->State.p.y+SLLWDG(o)->Pos.y-a.a.y,t,NULL,TEXTBOX(o)->Flags & TBF_PASSWORD);
			TEXTBOX(o)->DragPos = w;
			WidgetDraw(o, NULL);
			return true;
		}
		
		if ( Ev->Message == WEvMouseLDown )
		{
			l_text w = WhereInTextMultiLines(default_font,Mouse->State.p.x+SLLWDG(o)->Pos.x-a.a.x,Mouse->State.p.y+SLLWDG(o)->Pos.y-a.a.y,t,NULL,TEXTBOX(o)->Flags & TBF_PASSWORD);
			
			if ( (w > TEXTBOX(o)->SelPos) && (w <= TEXTBOX(o)->SelPosEnd) && ( TEXTBOX(o)->SelPosEnd != TEXTBOX(o)->SelPos )  ) {
				TEXTBOX(o)->Flags |= TBF_PREDRAG;
			} else {
				TEXTBOX(o)->SelPos = w;
				TEXTBOX(o)->SelPosEnd = w;
			}
			WidgetDraw(o, NULL);
			MasterWidget = o;
			return true;
		}
		if ( Ev->Message == WEvMouseLUp )
		{
			l_text w = WhereInTextMultiLines(default_font,Mouse->State.p.x+SLLWDG(o)->Pos.x-a.a.x,Mouse->State.p.y+SLLWDG(o)->Pos.y-a.a.y,t,NULL,TEXTBOX(o)->Flags & TBF_PASSWORD);
			/*if ( (TEXTBOX(o)->Flags & TBF_DRAGING) && (TEXTBOX(o)->Flags & TBF_EDITABLE) ) {
				TEXTBOX(o)->Flags &= ~TBF_DRAGING;
				if ( (TEXTBOX(o)->DragPos < TEXTBOX(o)->SelPos) || (TEXTBOX(o)->DragPos > TEXTBOX(o)->SelPosEnd) ) {
					l_text Temp = TextBoxGetSelection(TEXTBOX(o));
					TextBoxDeleteSelection(TEXTBOX(o));
					TEXTBOX(o)->SelPos = TEXTBOX(o)->DragPos;
					TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->DragPos;
					TextBoxInserText(TEXTBOX(o),Temp);
					free(Temp);
				}
			} else */if ( (TEXTBOX(o)->Flags & TBF_PREDRAG) ) {	
				TEXTBOX(o)->SelPos = w;
				TEXTBOX(o)->SelPosEnd = w;
				TEXTBOX(o)->Flags &= ~TBF_PREDRAG;
			} else {
				TEXTBOX(o)->SelPosEnd = max(TEXTBOX(o)->SelPos,w);
				TEXTBOX(o)->SelPos = min(TEXTBOX(o)->SelPos,w);
			}
			WidgetDraw(o, NULL);
			MasterWidget = NULL;
			return true;
		}
		if ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) ) {
			l_text w = WhereInTextMultiLines(default_font,Mouse->State.p.x+SLLWDG(o)->Pos.x-a.a.x,Mouse->State.p.y+SLLWDG(o)->Pos.y-a.a.y,t,NULL,TEXTBOX(o)->Flags & TBF_PASSWORD);
			/*if ( TEXTBOX(o)->Flags & TBF_DRAGING ) 
				TEXTBOX(o)->DragPos = w;
			else */if ( (TEXTBOX(o)->Flags & TBF_PREDRAG) && ( TEXTBOX(o)->SelPosEnd != TEXTBOX(o)->SelPos ) ) {	
				/*TEXTBOX(o)->DragPos = w;
				TEXTBOX(o)->Flags &= ~TBF_PREDRAG;
				TEXTBOX(o)->Flags |= TBF_DRAGING;*/
				TEXTBOX(o)->Flags &= ~TBF_PREDRAG;
				l_text t = TextBoxGetSelection(TEXTBOX(o));
				DragStart(o,DTYPE_TEXT,t,DMETHOD_COPY|DMETHOD_MOVE);
				free(t);
				MasterWidget = NULL;
			} else
				TEXTBOX(o)->SelPosEnd = w;
			WidgetDraw(o, NULL);
			CheckSelVisibility(TEXTBOX(o),( TEXTBOX(o)->Flags & TBF_MULTILINE ));
			return true;
		}

		if ( Ev->Message == WEvMouseRUp ) { // I love right click ! and you ??

			PMenu m = NewMenu(
     							NewMenuItem( "Copy", 		NULL, 	MSG_COPY, 	(TEXTBOX(o)->SelPos!=TEXTBOX(o)->SelPosEnd)&&(!(TEXTBOX(o)->Flags & TBF_PASSWORD ))?0:MI_DISABLE,NULL,
     							NewMenuItem( "Cut", 		NULL, 	MSG_CUT, 		(TEXTBOX(o)->SelPos!=TEXTBOX(o)->SelPosEnd)&&(TEXTBOX(o)->Flags & TBF_EDITABLE)&&(!(TEXTBOX(o)->Flags & TBF_PASSWORD ))?0:MI_DISABLE,NULL,
     							NewMenuItem( "Paste", 	NULL, 	MSG_PASTE, 	((TEXTBOX(o)->Flags & TBF_EDITABLE) && ClipboardGet(TYPE_TEXT,NULL))?0:MI_DISABLE,NULL,
     							NewMenuItemSeparator(
     							NewMenuItem( "Delete", 	NULL, 	MSG_DELETE, (TEXTBOX(o)->SelPos!=TEXTBOX(o)->SelPosEnd)&&(TEXTBOX(o)->Flags & TBF_EDITABLE)?0:MI_DISABLE,NULL,
     							NULL))))));

			PopUpMenu(o->AppOwner,Mouse->State.p,m,o,0);
			return true;
		}


	}

	if ( Ev->Type == EV_KEYBOARD )
	{
		if ( Ev->Keyb.Shift & KB_CTRL ) {
			if ( SCANCODE(Ev,KEY_C) ) {
				WidgetSendSEvent(o, EV_MESSAGE, MSG_COPY, NULL);
			}	else if ( SCANCODE(Ev,KEY_V) ) {
				WidgetSendSEvent(o, EV_MESSAGE, MSG_PASTE, NULL);
			}	else if ( SCANCODE(Ev,KEY_X) ) {
				WidgetSendSEvent(o, EV_MESSAGE, MSG_CUT, NULL);
			}
			return true;		
		}
		
		if ( SCANCODE(Ev, KEY_ESC) && TEXTBOX(o)->ValidMsg ) {
			WidgetSendSEvent(o, EV_MESSAGE, MSG_CANCEL, NULL);
			return true;
		}



		if ( SCANCODE(Ev,KEY_LEFT))
		{
			if ( Ev->Keyb.Shift & KB_SHIFT )
			{
				if ( TEXTBOX(o)->SelPosEnd > TEXTBOX(o)->Text ) TEXTBOX(o)->SelPosEnd--;
			}
			else
			{
				if ( TEXTBOX(o)->SelPos > TEXTBOX(o)->Text ) TEXTBOX(o)->SelPos--;
				TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
			}
		}
		else if ( SCANCODE(Ev,KEY_UP) && TEXTBOX(o)->Flags & TBF_MULTILINE )
		{
			l_text SOL = ReachStartOfLine(TEXTBOX(o)->Text,TEXTBOX(o)->SelPosEnd);
			if ( TEXTBOX(o)->Text != SOL ) {
				l_text sol = ReachStartOfLine(TEXTBOX(o)->Text,SOL-2);
				l_text pos = sol;
				if ( TEXTBOX(o)->SelPosEnd-SOL <= SOL-sol ) pos += TEXTBOX(o)->SelPosEnd-SOL;
				if ( Ev->Keyb.Shift & KB_SHIFT )
				{
					TEXTBOX(o)->SelPosEnd = pos;
				}
				else
				{
					TEXTBOX(o)->SelPos = pos;
					TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
				}
			}
		}
		else if ( SCANCODE(Ev,KEY_DOWN) && TEXTBOX(o)->Flags & TBF_MULTILINE )
		{
			l_text EOL = TextChr(TEXTBOX(o)->SelPosEnd,'\n');
			if ( EOL ) {
				l_text pos = EOL+1;
				l_text eol = TextChr(pos,'\n');
				l_text SOL = ReachStartOfLine(TEXTBOX(o)->Text,TEXTBOX(o)->SelPosEnd);
				if ( !eol ) eol = TEXTBOX(o)->Text+TextLen(TEXTBOX(o)->Text);
				if ( TEXTBOX(o)->SelPosEnd-SOL <= eol-EOL ) pos += TEXTBOX(o)->SelPosEnd-SOL;
				if ( Ev->Keyb.Shift & KB_SHIFT )
				{
					TEXTBOX(o)->SelPosEnd = pos;
				}
				else
				{
					TEXTBOX(o)->SelPos = pos;
					TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
				}
			}
		}
		else if ( SCANCODE(Ev,KEY_RIGHT) )
		{
			if ( Ev->Keyb.Shift & KB_SHIFT )
			{
				if ( TEXTBOX(o)->SelPosEnd < TEXTBOX(o)->Text+TextLen(TEXTBOX(o)->Text) ) TEXTBOX(o)->SelPosEnd++;
			}
			else
			{
				if ( TEXTBOX(o)->SelPos < TEXTBOX(o)->Text+TextLen(TEXTBOX(o)->Text) ) TEXTBOX(o)->SelPos++;
				TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
			}
		} else {

			if ( TEXTBOX(o)->SelPos > TEXTBOX(o)->SelPosEnd )
			{
				l_text t = TEXTBOX(o)->SelPosEnd;
				TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
				TEXTBOX(o)->SelPos = t;
			}

			if ( SCANCODE(Ev,KEY_BACKSPACE) && ( TEXTBOX(o)->SelPosEnd == TEXTBOX(o)->SelPos) && TEXTBOX(o)->Flags & TBF_EDITABLE  )
			{
				if ( TEXTBOX(o)->SelPos != TEXTBOX(o)->Text )
				{
					TextBoxDeleteMove(TEXTBOX(o),-1,0);
				}
			}
			else if (SCANCODE(Ev, KEY_HOME) && TEXTBOX(o)->Flags & TBF_EDITABLE)
			{
				TEXTBOX(o)->SelPos = TEXTBOX(o)->Text;
				TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
			}
			else if (SCANCODE(Ev, KEY_END) && TEXTBOX(o)->Flags & TBF_EDITABLE)
			{
				TEXTBOX(o)->SelPos = TEXTBOX(o)->Text;
				TEXTBOX(o)->SelPosEnd = TEXTBOX(o)->SelPos;
			}
			else if ( SCANCODE(Ev,KEY_DEL) && ( TEXTBOX(o)->SelPosEnd == TEXTBOX(o)->SelPos) && TEXTBOX(o)->Flags & TBF_EDITABLE )
			{
				if ( TEXTBOX(o)->SelPosEnd != (TEXTBOX(o)->Text+TextLen(TEXTBOX(o)->Text)) )
				{
					TextBoxDeleteMove(TEXTBOX(o),0,1);
				}
			}
			else if ( (SCANCODE(Ev,KEY_DEL) || SCANCODE(Ev,KEY_BACKSPACE)) && TEXTBOX(o)->Flags & TBF_EDITABLE ) {
		  	TextBoxDeleteSelection(TEXTBOX(o));
			}
			else if (SCANCODE(Ev, KEY_ENTER) && TEXTBOX(o)->Flags & TBF_EDITABLE )
			{

				if ( TEXTBOX(o)->Flags & TBF_MULTILINE )
					TextBoxWriteChar(TEXTBOX(o),'\n');
				else if ( TEXTBOX(o)->ValidMsg )
					WidgetSendSEvent(o, EV_MESSAGE, TEXTBOX(o)->ValidMsg, NULL);

			}
			else if ( ASCII(Ev) && TEXTBOX(o)->Flags & TBF_EDITABLE )
			{
				TextBoxWriteChar(TEXTBOX(o),ASCII(Ev));
				TextBoxRefreshFromTo(TEXTBOX(o),TEXTBOX(o)->SelPos-1,TEXTBOX(o)->SelPos);
			}

	 	}

		if ( TEXTBOX(o)->Flags & TBF_MULTILINE ){
		 	if ( TextBoxCalculateMax(TEXTBOX(o)) ) {
			 	CheckSelVisibility(TEXTBOX(o),true);
			 	WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
	      WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);
		 	} else
		 		CheckSelVisibility(TEXTBOX(o),true);
 		} else {
			TextBoxCalculateMax(TEXTBOX(o));
			CheckSelVisibility(TEXTBOX(o),false);


		}



		WidgetDraw(o, NULL);
		return true;
	}

	return ScrollWidgetEventHandler(o,Ev);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxFreeEx ( PWidget o ) {
	if ( TEXTBOX(o)->Text ) free(TEXTBOX(o)->Text);
	if ( TEXTBOX(o)->Tmr ) KillTimer(TEXTBOX(o)->Tmr);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxSetTextEx ( PTextbox o, l_text Txt ) {
	if ( o->Text ) free(o->Text);
  o->Text = TextDup(Txt);
  o->SelPos = o->SelPosEnd = o->DragPos = o->Text;
	if ( TextBoxCalculateMax(o) ) {
		if ( SLLWDG(o)->Vertical )  WidgetDraw(WIDGET(SLLWDG(o)->Vertical), NULL);
    if ( SLLWDG(o)->Horizontal ) WidgetDraw(WIDGET(SLLWDG(o)->Horizontal), NULL);
	}
	WidgetDraw(WIDGET(o), NULL);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxSetText ( PTextbox o, l_text Format, ... ) {
	l_char Buffer[4096];
  va_list arg;
  va_start(arg, Format);
  vsprintf(Buffer, Format, arg);
  TextBoxSetTextEx(o,Buffer);
  va_end(arg);
}
////////////////////////////////////////////////////////////////////////////////
void TextBoxTimer ( PTextbox o ) {
	if ( o->Flags & TBF_EDITABLE && ( WIDGET(o)->Flags & WFFocused || !(o->Flags & TBF_TIMERHIDECURSOR))) {
		o->Flags = o->Flags ^ TBF_TIMERHIDECURSOR;
		TextBoxRefreshFromTo(TEXTBOX(o),TEXTBOX(o)->SelPosEnd-1,TEXTBOX(o)->SelPosEnd);
	}
}
////////////////////////////////////////////////////////////////////////////////
PTextbox CreateTextbox(PApplication App, TRect r, l_ulong Flags)
{
	PTextbox p = malloc(sizeof(TTextbox));

	if (!p) return NULL;

	if ( !(Flags & TBF_MULTILINE) ) {
		if ( UseSkins ) {
			if ( r.b.x < r.a.y+text_height(default_font)+SkinTextbox->Top+SkinTextbox->Bottom )
				RectAssign(&r,r.a.x,r.a.y,r.b.x,r.a.y+text_height(default_font)+SkinTextbox->Top+SkinTextbox->Bottom);
		} else
			if ( r.b.x < r.a.y+text_height(default_font)+4 )
				RectAssign(&r,r.a.x,r.a.y,r.b.x,r.a.y+text_height(default_font)+4);
	}

	memset(p, 0, sizeof(TTextbox));

	p->Font = default_font;
	p->Flags = Flags|TBF_TIMERHIDECURSOR;
	p->Text = TextDup("");
	//IntialiseWidget(App, WIDGET(p), r, "Textline");
	p->SelPos = p->SelPosEnd = p->DragPos = p->Text;

	InitScrollWidget(SLLWDG(p), TextboxClass, App, r, (Flags & TBF_MULTILINE)?3:0);

	SLLWDG(p)->Delta.y = text_height(p->Font)+1;
	SLLWDG(p)->Delta.x = 20;
	SLLWDG(p)->Scrolled = &Scroll;

	WIDGET(p)->Flags |= WFFocusable|WFForceBuffer;
	WIDGET(p)->Draw = &TextBoxDraw;
	WIDGET(p)->EventHandler = &TextBoxEventHandler;
	WIDGET(p)->FreeEx = &TextBoxFreeEx;

	WIDGET(p)->DropAccept 		= &TextBoxDropAccept;
	WIDGET(p)->DragBeforeDrop 	= &TextBoxDragBeforeDrop;
	WIDGET(p)->DropData 		= &TextBoxDropData;
	WIDGET(p)->DragAfterDrop 	= &TextBoxDragAfterDrop;

	p->Tmr = NewTimer(App, 500, (void*)&TextBoxTimer, p );

	return p;
}
	PTextbox tb;
	
l_bool	TextboxSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {
	
	if ( !TextCompare(Name,"editable") ) {
		if ( *((l_bool*)Value) ) 
			TEXTBOX(o)->Flags |= TBF_EDITABLE;
		else
			TEXTBOX(o)->Flags &= ~TBF_EDITABLE;
		return true;
	}
	if ( !TextCompare(Name,"multiline") ) {
		if ( *((l_bool*)Value) ) 
			TEXTBOX(o)->Flags |= TBF_MULTILINE;
		else
			TEXTBOX(o)->Flags &= ~TBF_MULTILINE;
			
		ScrollBarChangeType(SLLWDG(o),(TEXTBOX(o)->Flags & TBF_MULTILINE)?3:0);
		
		return true;
	}	
	return false;
}
	
void	*TextboxGetRProp ( PWidget o, l_text Name ) {
	l_bool tmp;
	if ( !TextCompare(Name,"editable") ) {
		tmp = TEXTBOX(o)->Flags & TBF_EDITABLE;
		return DuplicateBool(&tmp);
	}
	if ( !TextCompare(Name,"multiline") ) {
		tmp = TEXTBOX(o)->Flags & TBF_MULTILINE;
		return DuplicateBool(&tmp);
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
PWidget  StdCreateTextbox ( PApplication App, TRect r )
{
	return WIDGET(CreateTextbox( App, r, 0));
}
////////////////////////////////////////////////////////////////////////////////
l_bool LibMain (  int argc, l_text *argv  )
{
	TextboxClass = CreateWidgetClass( ULONG_ID('T','x','t','B'), ScollWidgetClass, 0 );
	TextboxClass->StdCreate = &StdCreateTextbox;
	TextboxClass->SetRProp = &TextboxSetRProp;
	TextboxClass->GetRProp = &TextboxGetRProp;

	WClassRegisterProp(TextboxClass,PTYPE_BOOL,"editable");
	WClassRegisterProp(TextboxClass,PTYPE_BOOL,"multiline");
	
	SkinTextbox = GetSkinItem("Textbox");

	APPEXPORT(CreateTextbox);
	APPEXPORT(TextBoxSetText);
	APPEXPORT(TextBoxSetTextEx);
	/*
	TRect r;
	WidgetSize(&r,10,100,300,150),
	tb = CreateTextbox(&Me, r, TBF_EDITABLE|TBF_MULTILINE);
	InsertWidget(DeskTop, WIDGET(tb));
	WidgetDraw(WIDGET(tb), NULL);
	SLLWDG(tb)->Scrolled = &Scroll;
	TextBoxSetText(tb,"You see, my softwares works most of times...\nI think this version works fine, but there is still work.\nOf course i will do it. So do not try to make changes in TextBox please.\nAnyway have a nice day using OUR GREAT sofware O3(oZone)");
*/
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	//WidgetDispose(WIDGET(tb));
}
////////////////////////////////////////////////////////////////////////////////
