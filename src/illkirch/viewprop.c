////////////////////////////////////////////////////////////////////////////////
//
// Codename: Illkirch
//
// (c) Copyright 2004 Point Mad. All rights reserved.
//
//      Contributors : Julien Etelain (Point Mad)
//
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "illkirch.h"
#include "window.h"
#include "menu.h"
#include "button.h"
#include "label.h"
#include "grfx.h"
#include "tbox.h"
#include "listview.h"
#include <string.h>
PDevWidget SelDevWdg = NULL;

////////////////////////////////////////////////////////////////////////////////
void ViewPropCalculateMaxEx ( PList l, l_int *y ) {
        PListItem a,b;
        if ( l->Last ) {
                a = b = l->Last->Next;
                do {
                        *y += 18;
                        a = a->Next;
                } while( a != b );
        }
}
////////////////////////////////////////////////////////////////////////////////
void ViewPropCalculateMax ( PWidget o ) {
        l_int y = 0;

        if ( SelDevWdg ) {
                ViewPropCalculateMaxEx(DefProperties,&y);
                ViewPropCalculateMaxEx(SelDevWdg->Class->Properties,&y);
        }

        if ( y > (o->Absolute.b.y-o->Absolute.a.y) )
                ScrollBarSetMaxY(SLLWDG(o),y-(o->Absolute.b.y-o->Absolute.a.y));
        else
                ScrollBarSetMaxY(SLLWDG(o),0);

}
////////////////////////////////////////////////////////////////////////////////
void ViewPropDrawEx ( PWidget o, p_bitmap buffer, PRect w, PList l, l_int *y, PDevWidget s ) {
        PListItem a,b;
        TRect r;
        if ( l->Last ) {
                a = b = l->Last->Next;
                do {
                        DrawNiceText(buffer,default_font,o->Absolute.a.x+1,*y,75,DEVPROP(a->Data)->Name,COL_BOXTEXT);
                        RectAssign(&r,o->Absolute.a.x+75,*y,o->Absolute.b.x-1,(*y)+18);
                        if ( DEVPROP(a->Data)->Type->DrawProp ) {
                                void *v = ListKey(s->Properties,DEVPROP(a->Data)->Name);
                                if ( !v )
                                        v = DEVPROP(a->Data)->DefValue;
                                if ( v )
                                        DEVPROP(a->Data)->Type->DrawProp(buffer,r,v);
                        }
                        vline(buffer,o->Absolute.a.x+74,*y,(*y)+18,COL_BOXDARK);
                        hline(buffer,o->Absolute.a.x,(*y)+18,o->Absolute.b.x,COL_BOXDARK);
                        *y += 18;
                        a = a->Next;
                } while( a != b );
        }

}
////////////////////////////////////////////////////////////////////////////////
void ViewPropDraw ( PWidget o, p_bitmap buffer, PRect w ) {
        l_int y = o->Absolute.a.y-SLLWDG(o)->Pos.y;

        rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, COL_BOXFACE );

        if ( SelDevWdg ) {
                ViewPropDrawEx(o,buffer,w,DefProperties,&y,SelDevWdg);
                if ( SelDevWdg->Class )
                ViewPropDrawEx(o,buffer,w,SelDevWdg->Class->Properties,&y,SelDevWdg);
        }

}
////////////////////////////////////////////////////////////////////////////////
PDevProp GetPropUnderMouseEx ( PWidget o, PList l, l_int *y, PRect r ) {
        PListItem a,b;
        if ( l->Last ) {
                a = b = l->Last->Next;
                do {
                        RectAssign(r,o->Absolute.a.x+75,*y,o->Absolute.b.x-1,(*y)+18);
                        if ( CURSOR_IN_RECT(*r) ) return (PDevProp)a->Data;
                        *y += 18;
                        a = a->Next;
                } while( a != b );
        }
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
PDevProp GetPropUnderMouse ( PWidget o, PRect r ) {
        l_int y = o->Absolute.a.y-SLLWDG(o)->Pos.y;
        PDevProp ret = NULL;
        if ( SelDevWdg ) {
                ret = GetPropUnderMouseEx(o,DefProperties,&y,r);
                if ( SelDevWdg->Class && !ret )
                ret = GetPropUnderMouseEx(o,SelDevWdg->Class->Properties,&y,r);
        }
        return ret;
}
////////////////////////////////////////////////////////////////////////////////
l_bool ViewPropEventHandler ( PWidget o, PEvent Ev ) {
        if (Ev->Type == EV_MOUSE) {
                if (Ev->Message == WEvMouseLUp) { //WEvMouseLDClk
                        TRect r;
                        PDevProp p = GetPropUnderMouse(o,&r);
                        if ( p ) {
                                if ( p->Type->SetTool)
                                        p->Type->SetTool(p,SelDevWdg,(PViewProp)o,r,Mouse->State.p.x);
                                WidgetDraw(o,NULL);
                        }
                        return true;
                }
        }
        if ( Ev->Type == EV_MESSAGE ){
                if ( Ev->Message == MSG_NOTIFY_AUTO_RESIZE ) {
                        ViewPropCalculateMax(o);
                        return true;
                }
        }
        return false;
}
////////////////////////////////////////////////////////////////////////////////
void SetPropDevWgd ( PDevWidget o ) {
        SelDevWdg = o;
        ViewPropCalculateMax(WIDGET(ViewProp));
        ScrollBarRedraw(SLLWDG(ViewProp));
        WidgetDraw(WIDGET(ViewProp),NULL);

}
////////////////////////////////////////////////////////////////////////////////
void PropViewRefresh ( PDevWidget o ) {
        SelDevWdg = o;
        ViewPropCalculateMax(WIDGET(ViewProp));
        ScrollBarRedraw(SLLWDG(ViewProp));
        WidgetDraw(WIDGET(ViewProp),NULL);
}
////////////////////////////////////////////////////////////////////////////////
l_text PropViewSetToolHelperString ( PViewProp o, TRect r, l_text value ) {
        PTextbox tb;
        l_text ret = NULL;
        tb = CreateTextbox(&Me,r,TBF_EDITABLE);
        InsertWidget(DeskTop, WIDGET(tb));
        TextBoxSetTextEx(tb,value?value:"");
        WidgetDraw(WIDGET(tb), NULL);
        tb->ValidMsg = MSG_OK;
        WidgetSetFocusEx(WIDGET(tb));
        if ( WidgetExecute(WIDGET(tb)) != MSG_CANCEL ) {
                if ( !value )
                        ret = TextDup(tb->Text);
                else if ( TextCaseCompare(tb->Text,value))
                        ret = TextDup(tb->Text);
        }
        WidgetDispose(WIDGET(tb));
        return ret;
}
////////////////////////////////////////////////////////////////////////////////
void _ComboboxSelectItem ( PList Items, PListviewItem p ) {
        PListItem a = Items->Last, b = a;
        if ( !a ) return;
        do {
                if (LISTVIEWITEM(a->Data)->Flags & LVI_SELECTED )
                {
                        LISTVIEWITEM(a->Data)->Flags &= ~LVI_SELECTED;
                }
                a = a->Prev;
        } while ( a != b );

        if ( p ) {
                p->Flags |= LVI_SELECTED;
        }
}
////////////////////////////////////////////////////////////////////////////////
l_int PropViewSetToolHelperCombobox ( PViewProp o, TRect r, l_int index, PList Items ) {

        PListview l;
        l_ulong msg;
        l_int idx = 0;

        RectAssign(&r,r.a.x,r.b.y+1,r.b.x,r.b.y+min(100,ListviewEstimateHeight(Items)));

        l = CreateListview(&Me,r,NULL,LVS_LIST,0);

        FreeList(l->Items);

        l->Items = Items;
        if ( index )
                l->TopItem = ListAt(Items,index);
        if ( l->TopItem )
                _ComboboxSelectItem(Items,l->TopItem);

        l->OnValMsg = MSG_OK;

        InsertWidget(DeskTop,WIDGET(l));

        WidgetDraw(WIDGET(l), NULL);
        WidgetDraw(WIDGET(SLLWDG(l)->Vertical), NULL);

        ListviewSelectItem(l, GetSelectedItem(l), true, false);

        WidgetSetFocusEx(WIDGET(l));

        if ( (msg = WidgetExecute(WIDGET(l))) == MSG_OK )
                idx = ListFoundIndex(Items,GetSelectedItem(l));

        l->Items = NewList();

        WidgetDispose(WIDGET(SLLWDG(l)->Vertical));
        SLLWDG(l)->Vertical = NULL;
        WidgetDispose(WIDGET(l));

        return idx;
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

PViewProp CreateViewProp ( TRect r) {

        PViewProp o = malloc(sizeof(TViewProp));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TViewProp));

        InitScrollWidget(SLLWDG(o), ScollWidgetClass, &Me, r, 1 );

        SLLWDG(o)->Delta.y = 20;
        SLLWDG(o)->Scrolled = &Scroll;

        WIDGET(o)->Draw = &ViewPropDraw;
        WIDGET(o)->EventHandler = &ViewPropEventHandler;
        WIDGET(o)->Flags |= WFFocusable;

        return o;
}
////////////////////////////////////////////////////////////////////////////////
l_ulong PropViewSetToolHelperFlags ( PViewProp o, TRect r, l_ulong Flags, PList Items ) {

        PListview l;
        l_ulong msg;

        RectAssign(&r,r.a.x,r.b.y+1,r.b.x,r.b.y+200);

        l = CreateListview(&Me,r,"Name",LVS_LIST,LVF_CHECKBOXES);

        if ( Items->Last ) {
                PListItem a = Items->Last->Next;
                PListItem b = a;
                PListviewItem i;
                do {
                        i = ListviewAddItem ( l, a->Key, NULL );
                        if ( Flags & ((PWidgetPropFlag)a->Data)->Value ) i->Flags |= LVI_CHECKED;
                        a = a->Next;
                } while ( a != b );
        }

        InsertWidget(DeskTop,WIDGET(l));

        WidgetDraw(WIDGET(l), NULL);
        WidgetDraw(WIDGET(SLLWDG(l)->Vertical), NULL);

        WidgetSetFocusEx(WIDGET(l));

        msg = WidgetExecute(WIDGET(l));

        Flags = 0;
        if ( l->Items->Last ) {
                PListItem a = l->Items->Last->Next;
                PListItem b = a;
                PListviewItem i;
                do {
                        i = a->Data;
                        if ( i->Flags & LVI_CHECKED ) {
                                PWidgetPropFlag f = ListKey(Items,i->Caption);
                                if ( f ) Flags |= f->Value;
                        }
                        a = a->Next;
                } while ( a != b );
        }

        WidgetDispose(WIDGET(SLLWDG(l)->Vertical));
        SLLWDG(l)->Vertical = NULL;
        WidgetDispose(WIDGET(l));

        return Flags;
}
