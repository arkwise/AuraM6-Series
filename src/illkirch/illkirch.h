////////////////////////////////////////////////////////////////////////////////
//
// Codename: Illkirch B
//
// (c) Copyright 2004 Point Mad. All rights reserved.
//
//	Contributors : Julien Etelain (Point Mad)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILLKIRCH_H_INCLUDED_
#define _ILLKIRCH_H_INCLUDED_

#include"widget.h"
#include "treeview.h"

typedef struct TDevWidget *PDevWidget;
typedef struct TDevProp *PDevProp;
typedef struct TViewProp *PViewProp;

typedef struct TDevPType {
	
	l_ulong Type;
	l_ulong Flags;
	
	PPropType t;
	
	void (*SetTool) ( PDevProp t, PDevWidget w, PViewProp o, TRect r , l_ulong xpos ); 
	void (*DrawProp) ( p_bitmap buffer, TRect r,  void *value );
	
	
} TDevPType, *PDevPType;

#define DPTYPE(o) ((PDevPType)(o))


typedef struct TDevProp {
	
	l_text Name;
	void *DefValue;
	PDevPType Type;
	
	PWidgetProp Real;
	
} TDevProp;

#define DEVPROP(o) ((PDevProp)(o))



typedef struct TDevClass {
	l_text	Name;
	l_ulong Id;
	l_uid   Lib;
	l_ulong Flags;
	p_bitmap Icon16;
	PList Properties;
	void (*Init) ( PDevWidget o ); 
	void (*Draw) ( PDevWidget o,  p_bitmap buffer, PRect w ); 
	void (*SetProperty) ( PDevWidget o,  l_text name, void* value ); 
	
	PWidget Host;
	PWidgetClass RClass;
	
	l_ulong Nb;
	
	TPoint Delta;
	TRect  ChildArea;
	
} TDevClass, *PDevClass;

#define DEVCLASS(o) ((PDevClass)(o))

#define DCF_CONTENER 0x01

typedef struct TDevWidget {
	struct TWidget o;
	PList Properties;
	PDevClass Class;
	l_ulong Flags;
	TPoint 	w;
} TDevWidget;

#define DEVWIDGET(o) ((PDevWidget)(o))

#define DWF_RESIZING 	0x01
#define DWF_MOVING 		0x02
#define DWF_ADDING 		0x04
#define DWF_UNMOVEABLE 	0x08

PDevWidget CreateDevWidget ( TRect r, PDevClass Class );


typedef struct TViewProp {
	
	struct TScrollWidget o;
	
	
	PDevWidget s;
	

} TViewProp;

#define DEVWIDGET(o) ((PDevWidget)(o))


PDevClass AddDevClass ( l_text Name, p_bitmap Icon16, l_ulong Id, l_uid Lib, l_ulong Flags,void (*Draw) (PDevWidget,p_bitmap,PRect),void (*Init) ( PDevWidget o ),void (*SetProperty) ( PDevWidget o,  l_text name, void* value ) );
PDevProp AddDevProp ( PList l, l_text Name, void *DefValue, PDevPType Type, PWidgetProp Real );
PDevPType NewDevPType( l_ulong Type,l_ulong Flags,void (*SetTool)(PDevProp t,PDevWidget w,PViewProp o,TRect r, l_ulong xpos),void (*DrawProp) ( p_bitmap buffer, TRect r,  void *value ) );												

extern PList Classes;
extern PDevClass AddClass;
extern PDevClass WindowClass;
extern PList DefProperties;
extern PList DPTypes;
extern PDevPType PT_String;
extern PDevPType PT_Int;
extern PDevPType PT_Bool;
extern PViewProp ViewProp;

l_int PropViewSetToolHelperCombobox ( PViewProp o, TRect r, l_int index, PList Items );
l_text PropViewSetToolHelperString ( PViewProp o, TRect r, l_text value );
PViewProp CreateViewProp ( TRect r);
PDevPType GetDPType ( l_ulong Id );

PDevClass AddDevClassFrom ( l_text Name, p_bitmap Icon16, l_ulong Id );


void InitDevClass ( void );

PiWidget   ConvertToiWidget  ( PDevWidget o );
PDevWidget ImportFromiWidget ( PiWidget iw );

PDevClass GetDevClass(l_ulong Id) ;

extern PDevWidget RootDW;
extern PTreeView Tree;
extern PList ActualRes;

void AboutIllkirch ( void );

void ViewPropCalculateMax ( PWidget o );

PMenu MenuEditor( PMenu Menu );

typedef struct TMsgInfo {
	
	l_ulong Value;
	l_text	Name;
	l_text	Description;
	
	
	
} TMsgInfo, *PMsgInfo;

#define MSGINFO(o) ((PMsgInfo)(o))


void MsgMan( void );
extern PList MsgColl;
l_ulong MsgSelect ( l_ulong Message );

PBinData MsgInfoGenerateStore ( void );
void MsgInfoInitFromStore ( PBinData o );
void ResetMsgInfo ( void );
l_text GetMessageName(l_ulong messageValue);
#endif
