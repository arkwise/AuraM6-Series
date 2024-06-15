#include "kernel.h"
#include "widget.h"
#include "checkbox.h"
#include "string.h"

l_ulong AppVersion = ULONG_ID(0,0,0,1);
l_char  AppName[] = "Checkbox Widget";
l_uid   nUID = "checkbox";
l_uid NeededLibs[] = { "widget", "skin", "" };

PSkin CheckboxCleared = 0;
PSkin CheckboxChecked = 0;

PWidgetClass CheckboxClass;

void CheckboxDraw ( PWidget o, p_bitmap buffer, PRect w )
{
        if ( UseSkins ) {

                if (!CHECKBOX(o)->Checked)
                        DrawSkin(buffer, CheckboxCleared, o->Absolute.a.x, o->Absolute.a.y , o->Absolute.b.x, o->Absolute.b.y);
                else
                        DrawSkin(buffer, CheckboxChecked, o->Absolute.a.x, o->Absolute.a.y , o->Absolute.b.x, o->Absolute.b.y);

                textout_ex(buffer, default_font, CHECKBOX(o)->Caption, o->Absolute.a.x + CheckboxChecked->Left + 1, o->Absolute.a.y + (o->Absolute.b.y-o->Absolute.a.y)/2 - text_height(CheckboxCleared->Font)/2, CheckboxCleared->FontColor,-1);

        } else {

                rectfill(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DFACE);

                rect(buffer,o->Absolute.a.x+1,(o->Absolute.b.y+o->Absolute.a.y)/2-5,o->Absolute.a.x+11,(o->Absolute.b.y+o->Absolute.a.y)/2+5,COL_3DTEXT);

                if ( CHECKBOX(o)->Checked )     rectfill(buffer,o->Absolute.a.x+3,(o->Absolute.a.y+o->Absolute.b.y)/2-3,o->Absolute.a.x+9,(o->Absolute.a.y+o->Absolute.b.y)/2+3,COL_3DTEXT);

                textout_ex(buffer, default_font, CHECKBOX(o)->Caption, o->Absolute.a.x + 15,(o->Absolute.b.y+o->Absolute.a.y)/2 - text_height(default_font)/2, COL_3DTEXT,-1);

        }
}

l_bool CheckboxEventHandler ( PWidget o, PEvent Ev )
{
        if (Ev->Type == EV_MOUSE)
        {
                if (Ev->Message == WEvMouseLDown)
                {
                        CHECKBOX(o)->Checked = !CHECKBOX(o)->Checked;
                        WidgetDraw(o, NULL);
                        return true;
                }

                /*if (Ev->Message == WEvMouseLUp)
                {
                        CHECKBOX(o)->Checked = !CHECKBOX(o)->Checked;
                        WidgetDraw(o, NULL);
                        return true;
                }*/

        }

        return false;
}

PCheckbox CreateCheckbox(PApplication App, TRect r, l_text Caption)
{
        PCheckbox b = malloc(sizeof(TCheckbox));

        if (!b) return NULL;

        memset(b, 0, sizeof(TCheckbox));

        b->Checked = false;
        b->Caption = TextDup(Caption);

        /**
        *       Set the width and height to checkbox skin item
        *               width and height.
        */
        if ( UseSkins ) {
                r.b.x = r.a.x + (/**/ CheckboxCleared->Left + CheckboxCleared->Right + 1 + text_length(CheckboxCleared->Font, b->Caption) > CheckboxCleared->Skin->w ? CheckboxCleared->Left + CheckboxCleared->Right + 1 + text_length(CheckboxCleared->Font, b->Caption) : CheckboxCleared->Skin->w/**/);
                r.b.y = r.a.y + (CheckboxCleared->Skin->h > text_height(CheckboxCleared->Font) ? CheckboxCleared->Skin->h : text_height(CheckboxCleared->Font));
        }

        IntialiseWidget(App, CheckboxClass, WIDGET(b), r );

        WIDGET(b)->Draw = &CheckboxDraw;
        WIDGET(b)->EventHandler = &CheckboxEventHandler;

        return b;
}

l_bool  CheckBoxSetRProp ( PWidget o, l_text Name, void *Value, l_bool Redraw ) {

        if ( !TextCompare(Name,"caption") ) {
                if ( CHECKBOX(o)->Caption ) free(CHECKBOX(o)->Caption);
                CHECKBOX(o)->Caption = TextDup((l_text)Value);
                if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
                return true;
        }
        if ( !TextCompare(Name,"checked") ) {
                CHECKBOX(o)->Checked = *((l_bool*)Value);
                if ( Redraw ) WidgetDraw(WIDGET(o), NULL);
                return true;
        }

        return false;
}

void    *CheckBoxGetRProp ( PWidget o, l_text Name ) {
        if ( !TextCompare(Name,"caption") ) return TextDup(CHECKBOX(o)->Caption);
        if ( !TextCompare(Name,"checked") ) return DuplicateBool(&(CHECKBOX(o)->Checked));
        return NULL;

}

PWidget  StdCreateCheckbox ( PApplication App, TRect r )
{
        return WIDGET(CreateCheckbox( App, r, ""));
}

l_bool LibMain ( int argc, l_text *argv )
{
        CheckboxClass = CreateWidgetClass( ULONG_ID('C','h','k','B'), StdWidgetClass, 0 );
        CheckboxClass->StdCreate = &StdCreateCheckbox;
        CheckboxClass->SetRProp = &CheckBoxSetRProp;
        CheckboxClass->GetRProp = &CheckBoxGetRProp;

        WClassRegisterProp(CheckboxClass,PTYPE_STRING,"caption");
        WClassRegisterProp(CheckboxClass,PTYPE_BOOL,"checked");


        CheckboxCleared = GetSkinItem("CheckboxCleared");
        CheckboxChecked = GetSkinItem("CheckboxChecked");

        APPEXPORT(CreateCheckbox);
//        SYSEXPORT(CreateCheckbox);

        /*TRect r;
        RectAssign(&r, 50,50,150,70);
        PCheckbox p = CreateCheckbox(&Me, r, "Bujaka");
        InsertWidget(WIDGET(DeskTop), WIDGET(p));
        WidgetDraw(WIDGET(p), NULL);*/

        return true;
}

void Close (void)
{

}
