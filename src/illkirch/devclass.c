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
#include "scrllbar.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
TScrollWidget ScllHost;
////////////////////////////////////////////////////////////////////////////////
void InitDevClass ( void ) {
        TRect r = { 0, 0, 100, 100 };
        InitScrollWidget(&ScllHost,ScollWidgetClass,&Me,r,3);
}
////////////////////////////////////////////////////////////////////////////////
void FlushProperties ( PDevWidget o ) {
        PDevClass cl = o->Class;
        PWidgetClass wcl = cl->RClass;

        memcpy(cl->Host,o,(l_ulong)&(cl->Host->BackgroundColor) - (l_ulong)cl->Host);

        cl->Host->Flags |= WFSelected;

        while ( wcl && (wcl != StdWidgetClass) ) {

                if ( wcl->Properties->Last && wcl->SetRProp ) {
                        PListItem a,b;
                        a = b = wcl->Properties->Last->Next;
                        do {
                                void *v;
                                if ( v = ListKey(o->Properties,a->Key) ) {
                                        wcl->SetRProp( cl->Host,a->Key,v,false);
                                }
                                a = a->Next;
                        } while ( a != b );
                }
                wcl = wcl->Parent;
        }

        // Force position.
        memcpy(cl->Host,o,(l_ulong)&(cl->Host->ChildDrawAbs) - (l_ulong)cl->Host);


}

////////////////////////////////////////////////////////////////////////////////
void DevExtScllClassDraw ( PDevWidget o,  p_bitmap buffer, PRect w ) {

        TRect r, a, rb;

        PDevClass cl = o->Class;

        FlushProperties(o);

        r = WIDGET(o)->Absolute;

        if ( SLLWDG(cl->Host)->Vertical ) {
                RectAssign(&rb,r.b.x-(100-WIDGET(&ScllHost)->Relative.b.x)+1, r.a.y, r.b.x, r.b.y);

                if ( SLLWDG(cl->Host)->Horizontal ) rb.b.y -= (100-WIDGET(&ScllHost)->Relative.b.y)-1;

                if ( RectOverlay(rb,*w) ) {
                        RectIntersept(&a,rb,*w);
                        WIDGET(ScllHost.Vertical)->Absolute = rb;
                        WIDGET(ScllHost.Vertical)->Relative = rb;
                        WIDGET(ScllHost.Vertical)->Draw(WIDGET(ScllHost.Vertical),buffer,&a);
                }
        }

        if ( SLLWDG(cl->Host)->Horizontal ) {
                RectAssign(&rb,r.a.x,r.b.y-(100-WIDGET(&ScllHost)->Relative.b.y)+1,r.b.x,r.b.y);

                if ( SLLWDG(cl->Host)->Vertical ) rb.b.x -= (100-WIDGET(&ScllHost)->Relative.b.x)-1;

                if ( RectOverlay(rb,*w) ) {
                        RectIntersept(&a,rb,*w);
                        WIDGET(ScllHost.Horizontal)->Absolute = rb;
                        WIDGET(ScllHost.Horizontal)->Relative = rb;
                        WIDGET(ScllHost.Horizontal)->Draw(WIDGET(ScllHost.Horizontal),buffer,&a);
                }
        }


        if ( SLLWDG(cl->Host)->Vertical ) {
                cl->Host->Relative.b.x -= (100-WIDGET(&ScllHost)->Relative.b.x);
                cl->Host->Absolute.b.x -= (100-WIDGET(&ScllHost)->Relative.b.x);
        }

        if ( SLLWDG(cl->Host)->Horizontal ) {
                cl->Host->Relative.b.y -= (100-WIDGET(&ScllHost)->Relative.b.y);
                cl->Host->Absolute.b.y -= (100-WIDGET(&ScllHost)->Relative.b.y);
        }

        if ( RectOverlay(cl->Host->Absolute,*w) ) {
                RectIntersept(&a,cl->Host->Absolute,*w);
                cl->Host->Draw(cl->Host,buffer,&a);
        }

}
////////////////////////////////////////////////////////////////////////////////
void DevExtClassDraw ( PDevWidget o,  p_bitmap buffer, PRect w ) {

        PDevClass cl = o->Class;

        FlushProperties(o);

        cl->Host->Draw(cl->Host,buffer,w);

}

////////////////////////////////////////////////////////////////////////////////
void DevExtClassInit ( PDevWidget o ) {

        if ( o->Class->RClass->Flags & WCF_CONTAINER ) {
                RectAssign(&WIDGET(o)->ChildArea,
                        o->Class->ChildArea.a.x,
                        o->Class->ChildArea.a.y,
                        (WIDGET(o)->Relative.b.x-WIDGET(o)->Relative.a.x)-o->Class->ChildArea.b.x,
                        (WIDGET(o)->Relative.b.y-WIDGET(o)->Relative.a.y)-o->Class->ChildArea.b.y);
        }


}
////////////////////////////////////////////////////////////////////////////////
void DevExtClassSetProperty ( PDevWidget o,  l_text name, void* value ) {

        WidgetDraw(WIDGET(o),NULL);
}

////////////////////////////////////////////////////////////////////////////////
l_bool IsClassId ( PWidgetClass cl, l_ulong Id ) {
        while ( cl ) {
                if ( cl->Id == Id ) return true;
                cl = cl->Parent;
        }
        return false;
}
////////////////////////////////////////////////////////////////////////////////
PDevClass GetDevClass(l_ulong Id) {

        PListItem a,b;
        PDevClass o;
        if ( !Classes->Last ) return NULL;
        a = b = Classes->Last->Next;
        do {

                o = (PDevClass)a->Data;
                if ( o->Id == Id ) return o;

                a = a->Next;
        } while ( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void FreeDevClass ( PDevClass o ) {
        if ( o->Name ) free(o->Name);
        if ( o->Properties ) FreeList(o->Properties);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
PDevClass AddDevClass ( l_text Name, p_bitmap Icon16, l_ulong Id, l_uid Lib, l_ulong Flags,
                                                                                                void (*Draw) (PDevWidget,p_bitmap,PRect),void (*Init) ( PDevWidget o ),void (*SetProperty) ( PDevWidget o,  l_text name, void* value ) ) {
        PDevClass o = malloc(sizeof(TDevClass));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TDevClass));

        o->Name = TextDup(Name);
        o->Icon16 = Icon16;
        o->Id = Id;
        memcpy(&o->Lib,Lib,12);

        o->Flags = Flags;
        o->Draw = Draw;


        o->Init = Init;
        o->SetProperty = SetProperty;

        o->Properties = NewList();
        ListAdd(Classes,NULL,o,(void*)&FreeDevClass);

        return o;
}
////////////////////////////////////////////////////////////////////////////////
PDevClass AddDevClassFrom ( l_text Name, p_bitmap Icon16, l_ulong Id ) {
        PDevClass o = malloc(sizeof(TDevClass));
        PWidgetClass cl = GetWidgetClass(Id);
        TRect r = { 0, 0, 100, 100 };

        if ( !o ) return NULL;
        memset(o,0,sizeof(TDevClass));

        DebugMessage("New dev class '%s'",Name);

        o->Name = TextDup(Name);
        o->Icon16 = Icon16;
        o->Id = Id;

        o->Flags = 0;
        if ( cl->Flags & WCF_CONTAINER ) o->Flags |= DCF_CONTENER;

        if ( IsClassId(cl,ULONG_ID('S','l','W','g') ) )
                o->Draw = &DevExtScllClassDraw;
        else
                o->Draw = &DevExtClassDraw;

        o->Init = &DevExtClassInit;
        o->SetProperty = &DevExtClassSetProperty;


        o->Host = cl->StdCreate(&Me,r);

        if ( !IsClassId(cl,ULONG_ID('S','l','W','g') ) ) {
                o->Delta.x = o->Host->Relative.b.x-100;
                o->Delta.y = o->Host->Relative.b.y-100;

        }
        if ( o->Flags & DCF_CONTENER ) {

                RectAssign(&o->ChildArea,
                o->Host->ChildArea.a.x,
                o->Host->ChildArea.a.y,
                o->Host->Relative.b.x-o->Host->ChildArea.b.x,
                o->Host->Relative.b.y-o->Host->ChildArea.b.y);

        }

        o->RClass = cl;

        o->Properties = NewList();

        ListAdd(Classes,NULL,o,(void*)&FreeDevClass);

        while ( cl /*&& (cl != StdWidgetClass)*/ ) {

                if ( cl->Properties->Last ) {
                        PListItem a,b;
                        PDevPType dpt;
                        void *v;
                        a = b = cl->Properties->Last->Next;
                        do {

                                if ( TextCompare(a->Key,"name") &&
                             TextCompare(a->Key,"r.a.x") &&
                             TextCompare(a->Key,"r.a.y") &&
                             TextCompare(a->Key,"r.b.x") &&
                             TextCompare(a->Key,"r.b.y") )

                                AddDevProp(o->Properties,a->Key,cl->GetRProp(o->Host,a->Key),GetDPType(WDGPROP(a->Data)->Type->Id),WDGPROP(a->Data));

                                a = a->Next;

                        } while ( a != b );
                }
                cl = cl->Parent;
        }


        return o;
}
////////////////////////////////////////////////////////////////////////////////
void FreeDevProp ( PDevProp o ) {
        if ( o->Name ) free(o->Name);
        if ( o->DefValue ) o->Type->t->Free(o->DefValue);
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
PDevProp AddDevProp ( PList l, l_text Name, void *DefValue, PDevPType Type, PWidgetProp Real ) {

        PDevProp o = malloc(sizeof(TDevProp));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TDevProp));

        DebugMessage("  New property '%s'",Name);


        o->Name = TextDup(Name);

        if ( DefValue ) {
                o->DefValue = DefValue;

                if ( Type->Type == PTYPE_BOOL )
                        DebugMessage("    Default value : %s",*((l_bool*)o->DefValue)?"true":"false");
                if ( Type->Type == PTYPE_INT )
                        DebugMessage("    Default value : %i",*((l_int*)o->DefValue));
                if ( Type->Type == PTYPE_MESSAGE )
                        DebugMessage("    Default value : %x",*((l_ulong*)o->DefValue));
                if ( Type->Type == PTYPE_STRING )
                        DebugMessage("    Default value : %s",o->DefValue);

        }

        o->Type = Type;

        o->Real = Real;

        ListAdd(l,Name,o,(void*)&FreeDevProp);

        return o;
}
////////////////////////////////////////////////////////////////////////////////
PDevPType NewDevPType( l_ulong Type,l_ulong Flags,void (*SetTool)(PDevProp t,PDevWidget w,PViewProp o,TRect r, l_ulong xpos),void (*DrawProp) ( p_bitmap buffer, TRect r,  void *value ) ) {
        PDevPType o = malloc(sizeof(TDevPType));
        if ( !o ) return NULL;
        memset(o,0,sizeof(TDevPType));
        o->t            = GetPropType(Type);
        o->Type                 = Type;
        o->Flags                = Flags;
        o->SetTool              = SetTool;
        o->DrawProp     = DrawProp;
        ListAdd(DPTypes,NULL,o,&free);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
PDevPType GetDPType ( l_ulong Id ) {

        PListItem a,b;

        if ( !DPTypes->Last ) return NULL;

        a = b = DPTypes->Last->Next;

        do {

                if ( DPTYPE(a->Data)->Type == Id ) return DPTYPE(a->Data);

                a = a->Next;
        } while ( a != b );

        DebugMessage("DP %x type is unknown",Id);

        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
