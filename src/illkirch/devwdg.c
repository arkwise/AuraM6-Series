////////////////////////////////////////////////////////////////////////////////
//
// Codename: Illkirch B
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
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
l_bool DevWidgetSetPropertyEx ( PDevWidget o, l_text name, void* value );
////////////////////////////////////////////////////////////////////////////////
void DevWidgetDraw ( PWidget o, p_bitmap buffer, PRect w ) {
        TRect r;
        rectfill(buffer, w->a.x, w->a.y, w->b.x, w->b.y, COL_3DFACE );


        if ( DEVWIDGET(o)->Class ) {
                if ( DEVWIDGET(o)->Class->Draw )
                        DEVWIDGET(o)->Class->Draw(DEVWIDGET(o),buffer,w);
        }

        if ( o->Flags & WFFocused ) {
                RectAssign(&r,o->Absolute.b.x-10,o->Absolute.b.y-10,o->Absolute.b.x,o->Absolute.b.y);
                DrawDotRect(buffer,o->Absolute,makecol(0,0,0));
                DrawDotRect(buffer,r,makecol(0,0,0));
        }

        if ( (DEVWIDGET(o)->Class->Flags & DCF_CONTENER ) ) {
                l_int x = o->ChildArea.a.x, y;
                while ( x < o->ChildArea.b.x ) {
                        y = o->ChildArea.a.y;
                        while ( y < o->ChildArea.b.y ) {
                                putpixel(buffer,o->Absolute.a.x+x,o->Absolute.a.y+y, makecol(0,0,0));
                                y += 5;
                        }
                        x+= 5;
                }
        }
}
////////////////////////////////////////////////////////////////////////////////
l_bool DevWidgetEventHandler ( PWidget o, PEvent Ev ) {

        if ( Ev->Type == EV_MESSAGE ){
                if ( Ev->Message == WEvNotifyFocused ){
                        WidgetDraw(o, NULL);
                        SetPropDevWgd(DEVWIDGET(o));
                        return true;
                } else if ( Ev->Message == WEvNotifyLostFocus ){
                        WidgetDraw(o, NULL);
                        return true;
                }

        }

        if ( Ev->Type == EV_MOUSE ) {


                if ( Ev->Message == WEvMouseLDown ) {
                        if ( AddClass ) {
                                if ( DEVWIDGET(o)->Class ) {
                                        if ( !(DEVWIDGET(o)->Class->Flags & DCF_CONTENER ) ) return true;
                                }
                                DEVWIDGET(o)->Flags |= DWF_ADDING;
                        } else if ( (Mouse->State.p.x > o->Absolute.b.x-10) && (Mouse->State.p.y > o->Absolute.b.y-10) )
                                DEVWIDGET(o)->Flags |= DWF_RESIZING;
                        else {
                                if (DEVWIDGET(o)->Flags & DWF_UNMOVEABLE ) return true;
                                DEVWIDGET(o)->Flags |= DWF_MOVING;
                        }

                        DEVWIDGET(o)->w = Mouse->State.p;

                        MasterWidget = o;
                        return true;
                } else if ( (Ev->Message == WEvMouseMove) && (MasterWidget == o )) {

                        TRect tmp;

                        if ( DEVWIDGET(o)->Flags & DWF_RESIZING ) {
                                RectAssign(&tmp,o->Absolute.a.x,o->Absolute.a.y,
                                o->Absolute.b.x+(((Mouse->State.p.x-DEVWIDGET(o)->w.x)/5)*5),
                                o->Absolute.b.y+(((Mouse->State.p.y-DEVWIDGET(o)->w.y)/5)*5));
                        } else if ( DEVWIDGET(o)->Flags & DWF_MOVING ) {
                                RectAssign(&tmp,
                                o->Absolute.a.x+(((Mouse->State.p.x-DEVWIDGET(o)->w.x)/5)*5),
                                o->Absolute.a.y+(((Mouse->State.p.y-DEVWIDGET(o)->w.y)/5)*5),
                                o->Absolute.b.x+(((Mouse->State.p.x-DEVWIDGET(o)->w.x)/5)*5),
                                o->Absolute.b.y+(((Mouse->State.p.y-DEVWIDGET(o)->w.y)/5)*5));

                        } else {
                                TPoint d;
                                d.x = o->Absolute.a.x+o->ChildArea.a.x;
                                d.y = o->Absolute.a.y+o->ChildArea.a.y;
                                RectAssign(&tmp,
                                d.x+(((DEVWIDGET(o)->w.x-d.x)/5)*5),d.y+(((DEVWIDGET(o)->w.y-d.y)/5)*5),
                                d.x+(((Mouse->State.p.x-d.x)/5)*5),d.y+(((Mouse->State.p.y-d.y)/5)*5));

                        }

                        WidgetSetOSDRect(tmp);

                        return true;
                } else if ( (Ev->Message == WEvMouseLUp) && (MasterWidget == o ) ) {

                        if ( DEVWIDGET(o)->Flags & DWF_RESIZING ) {
                                WidgetResize(o,(((Mouse->State.p.x-DEVWIDGET(o)->w.x)/5)*5),
                                (((Mouse->State.p.y-DEVWIDGET(o)->w.y)/5)*5));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.b.x",&(o->Relative.b.x));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.b.y",&(o->Relative.b.y));
                                PropViewRefresh(DEVWIDGET(o));
                        } else if ( DEVWIDGET(o)->Flags & DWF_MOVING ) {
                                WidgetMove(o,(((Mouse->State.p.x-DEVWIDGET(o)->w.x)/5)*5),(((Mouse->State.p.y-DEVWIDGET(o)->w.y)/5)*5));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.a.x",&(o->Relative.a.x));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.a.y",&(o->Relative.a.y));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.b.x",&(o->Relative.b.x));
                                DevWidgetSetPropertyEx(DEVWIDGET(o),"r.b.y",&(o->Relative.b.y));
                                PropViewRefresh(DEVWIDGET(o));
                        } else if ( AddClass ) {
                                PDevWidget n; TRect tmp,r; TPoint d;
                                d.x = o->Absolute.a.x+o->ChildArea.a.x;
                                d.y = o->Absolute.a.y+o->ChildArea.a.y;
                                RectAssign(&tmp,(((DEVWIDGET(o)->w.x-d.x)/5)*5),(((DEVWIDGET(o)->w.y-d.y)/5)*5),
                                (((Mouse->State.p.x-d.x)/5)*5),(((Mouse->State.p.y-d.y)/5)*5));
                                r.a.x = min(tmp.a.x,tmp.b.x);
                                r.a.y = min(tmp.a.y,tmp.b.y);
                                r.b.x = max(tmp.a.x,tmp.b.x);
                                r.b.y = max(tmp.a.y,tmp.b.y);
                                n = CreateDevWidget(r,AddClass);
                                WidgetInsert(o,WIDGET(n));
                                WidgetSetFocus(WIDGET(n));
                                //SetPropDevWgd(DEVWIDGET(o));
                                AddClass = NULL;
                        }

                        WidgetUnOSD();

                        DEVWIDGET(o)->Flags &= ~(DWF_RESIZING|DWF_MOVING|DWF_ADDING);
                        MasterWidget = NULL;
                        return true;
                }
        }

        if ( Ev->Type == EV_KEYBOARD ) {
                if ( SCANCODE(Ev,KEY_DEL) ) {
                        if ( DEVWIDGET(o)->Flags & DWF_UNMOVEABLE ) return true;
                        WidgetDispose(o);
                        SetPropDevWgd(NULL);
                        return true;
                }
        }
        return false;
}
////////////////////////////////////////////////////////////////////////////////
void DevWidgetFreeEx ( PWidget o )
{
        DebugMessage("DevWidgetFreeEx(%x->Properties = %x)",o,DEVWIDGET(o)->Properties);
        FreeList(DEVWIDGET(o)->Properties);
        DebugMessage("OK");
}
////////////////////////////////////////////////////////////////////////////////
l_bool DevWidgetSetPropertyEx ( PDevWidget o, l_text name, void* value ) {

        PListItem i;
        PDevProp p = NULL;
        PDevPType t;

        if ( o->Class ) {
                p = DEVPROP(ListKey(o->Class->Properties,name));
        }

        if ( !p ) p = DEVPROP(ListKey(DefProperties,name));

        if ( !p ) {
                DebugError("Illkirch: Unknown property ''%s'' (into DevWidget %x Class %x)",name,o,o->Class);
                return false;
        }

        t = p->Type;
        if ( !t ) {
                DebugError("Illkirch: Property ''%s'' as no type (into DevWidget %x Class %x)",name,o,o->Class);
                return false;
        }

        value = t->t->Duplicate(value);
        i = ListKeyItem(o->Properties,name);
        if ( !i ) {
                DebugMessage("Add property : %s into %x",name,o);
                ListAdd(o->Properties,name,value,t?t->t->Free:NULL);
        } else {
                if ( i->FreeData ) i->FreeData(i->Data);
                i->Data = value;
        }

        return true;
}
////////////////////////////////////////////////////////////////////////////////
void DevWidgetSetProperty ( PDevWidget o, l_text name, void* value ) {
        l_text OldName = NULL;

        if ( !TextCompare(name,"name" ) && (o == RootDW) ) OldName = TextDup((l_text)ListKey(RootDW->Properties,"name"));

        if ( !DevWidgetSetPropertyEx(o,name,value) ) return;

        if ( o->Class ) {
                if ( ListKey(o->Class->Properties,name) ) {
                        if ( o->Class->SetProperty ) o->Class->SetProperty(o,name,value);
                }
        }


        TRect r = WIDGET(o)->Relative;

        if ( OldName ) {
                PListItem i = ListKeyItem(ActualRes,OldName);
                if ( i ) {
                        free(i->Key);
                        i->Key = TextDup((l_text)value);
                }
                if ( Tree->Items->Last ) {
                        PTreeItem a = Tree->Items->Last->Next;
                        PTreeItem b = a;
                        do {
                                if ( !TextCompare(a->Caption,OldName) ) {
                                        free(a->Caption);
                                        a->Caption = TextDup((l_text)value);
                                        WidgetDraw(WIDGET(Tree), NULL);

                                }
                                a = a->Next;
                        } while ( a != b );
                }
                free(OldName);
        } else if ( !TextCompare(name,"r.a.x" ) ) {
                r.a.x = *((l_int*)value);
                WidgetSetMetrics(WIDGET(o),r.a.x,r.a.y,r.b.x,r.b.y);
        } else if ( !TextCompare(name,"r.a.y" ) ) {
                r.a.y = *((l_int*)value);
                WidgetSetMetrics(WIDGET(o),r.a.x,r.a.y,r.b.x,r.b.y);
        } else if ( !TextCompare(name,"r.b.x" ) ) {
                r.b.x = *((l_int*)value);
                WidgetSetMetrics(WIDGET(o),r.a.x,r.a.y,r.b.x,r.b.y);
        } else if ( !TextCompare(name,"r.b.y" ) ) {
                r.b.y = *((l_int*)value);
                WidgetSetMetrics(WIDGET(o),r.a.x,r.a.y,r.b.x,r.b.y);
        }


}
////////////////////////////////////////////////////////////////////////////////

PDevWidget CreateDevWidget ( TRect r, PDevClass Class ) {
        l_text name;
        PDevWidget o = malloc(sizeof(TDevWidget));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TDevWidget));

        IntialiseWidget(&Me, StdWidgetClass, WIDGET(o), r );

        o->Class = Class;
        WIDGET(o)->Draw = &DevWidgetDraw;
        WIDGET(o)->EventHandler = &DevWidgetEventHandler;
        WIDGET(o)->FreeEx = &DevWidgetFreeEx;
        o->Properties = NewList();
        WIDGET(o)->Flags |= WFFocusable;

        Class->Nb++;
        name = TextArgs("%s%d",Class->Name,Class->Nb);
        DevWidgetSetPropertyEx(o,"name",name);
        free(name);

        DevWidgetSetPropertyEx(o,"r.a.x",&(r.a.x));
        DevWidgetSetPropertyEx(o,"r.a.y",&(r.a.y));
        DevWidgetSetPropertyEx(o,"r.b.x",&(r.b.x));
        DevWidgetSetPropertyEx(o,"r.b.y",&(r.b.y));

        if ( Class ) if ( Class->Init ) Class->Init(o);

        return o;
}
////////////////////////////////////////////////////////////////////////////////

PiWidget ConvertToiWidget ( PDevWidget o ) {

        PiWidget iw = NewiWidget(o->Class->RClass);

        iw->Name = TextDup(ListKey(o->Properties,"name"));
        iw->r = WIDGET(o)->Relative;

        iw->r.b.x -= o->Class->Delta.x;
        iw->r.b.y -= o->Class->Delta.y;

        if ( o->Class->Properties->Last ) {
                PListItem a, b;
                void *v;
                a = b = o->Class->Properties->Last->Next;
                do {
                        v = ListKey(o->Properties,DEVPROP(a->Data)->Name);
                        if ( v ) {
                                v = DEVPROP(a->Data)->Type->t->Duplicate(v);
                                ListAdd(iw->Properties,DEVPROP(a->Data)->Name,v,DEVPROP(a->Data)->Type->t->Free);
                        }
                        a = a->Next;
                } while ( a != b );
        }

        if ( WIDGET(o)->Last ) {
                PWidget a,b;
                PiWidget n;
                a = b = WIDGET(o)->Last->Next;
                do {
                        n = ConvertToiWidget(DEVWIDGET(a));
                        InsertiWidgetBefore(iw,n,iw->Last?iw->Last->Next:NULL);
                        a = a->Next;
                } while ( a != b );
        }

        return iw;
}
////////////////////////////////////////////////////////////////////////////////
PDevWidget ImportFromiWidget ( PiWidget iw ) {

        PDevClass dvcl = GetDevClass(iw->Class->Id);

        if ( dvcl ) {
                TRect r = iw->r;
                PDevWidget o;

                r.b.x += dvcl->Delta.x;
                r.b.y += dvcl->Delta.y;

                o = CreateDevWidget(r,dvcl);

                if ( iw->Name ) {
                        if ( TextLen(iw->Name) > 0 )
                                DevWidgetSetPropertyEx(o,"name",iw->Name);
                }


                if ( dvcl->Properties->Last ) {
                        PListItem a, b;
                        void *v;
                        a = b = dvcl->Properties->Last->Next;
                        do {
                                v = ListKey(iw->Properties,DEVPROP(a->Data)->Name);
                                if ( v ) {

                                        DevWidgetSetPropertyEx(o,DEVPROP(a->Data)->Name,v);

                                        //v = DEVPROP(a->Data)->Type->t->Duplicate(v);
                                        //ListAdd(o->Properties,DEVPROP(a->Data)->Name,v,DEVPROP(a->Data)->Type->t->Free);
                                }
                                a = a->Next;
                        } while ( a != b );
                }

                if ( iw->Last ) {
                        PiWidget a,b;
                        PDevWidget n;
                        a = b = iw->Last->Next;
                        do {
                                n = ImportFromiWidget(a);
                                InsertWidget(WIDGET(o),WIDGET(n));
                                a = a->Next;
                        } while ( a != b );
                }
                return o;

        } else
                DebugError("ILLKIRCH : Unable to found class %x",iw->Class->Id);

        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
