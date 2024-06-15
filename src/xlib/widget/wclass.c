////////////////////////////////////////////////////////////////////////////////
//
//  Widgets - Core File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
/**	
	@addtogroup widgetsys
	@{
*/
/**
	@file
	Widget Class file
*/
/**
	@defgroup wclass Widget Class
	@{
*/
////////////////////////////////////////////////////////////////////////////////
// Widget Class managment
////////////////////////////////////////////////////////////////////////////////
PList WidgetClasses = NULL; // Registred classes
PList PropTypes = NULL;
////////////////////////////////////////////////////////////////////////////////
void FreeWidgetClass ( PWidgetClass o ) {
	if ( o->Properties ) FreeList(o->Properties);
	free(o);	
}
////////////////////////////////////////////////////////////////////////////////
PWidgetClass CreateWidgetClass ( l_ulong Id, PWidgetClass Parent, l_ulong Flags ) {
	PWidgetClass o = malloc(sizeof(TWidgetClass));
	if ( !o ) return NULL;
	memset(o,0,sizeof(TWidgetClass));
	o->Id = Id;
	o->Parent = Parent;
	o->Flags = Flags;
	o->Properties = NewList();
	ListAdd(WidgetClasses,NULL,o,(void*)&FreeWidgetClass);
	DebugMessage("CreateWidgetClass %x",Id);
	return o;
} 
////////////////////////////////////////////////////////////////////////////////
PWidgetClass GetWidgetClass ( l_ulong Id ) {
	if ( WidgetClasses->Last ) {
		PListItem a,b;
		a = b = WidgetClasses->Last->Next;
		do {
			if ( WCLASS(a->Data)->Id == Id ) return WCLASS(a->Data);
			a = a->Next;
		} while ( a != b );
	}
	DebugError("WIDGET : Unable to found class %x",Id);
	return NULL;	
}
////////////////////////////////////////////////////////////////////////////////
/**
	Determines if class of widget is specified class of a sub-class
	@param o Widget to test
	@param Id 32bits identifier of class
*/
l_bool WidgetIsClassId ( PWidget o, l_ulong Id ) {
	PWidgetClass cl = o->Class;
	while ( cl ) {
		if ( cl->Id == Id ) return true;
		cl = cl->Parent;	
	}
	return false;	
}
////////////////////////////////////////////////////////////////////////////////
PPropType CreatePropType ( l_ulong Id, void *(*Duplicate)(void*),void (*Free)(void*),void (*Write) ( PFile, void *),void *(*Read) ( PFile )) {
	PPropType o = malloc(sizeof(TPropType));
	if ( !o ) return NULL;
	o->Id = Id;
	o->Duplicate = Duplicate;
	o->Free = Free;
	o->Write = Write;
	o->Read = Read;
	ListAdd(PropTypes,NULL,o,&free);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
PPropType GetPropType ( l_ulong Id ) {
	if ( PropTypes->Last ) {
		PListItem a,b;
		a = b = PropTypes->Last->Next;
		do {
			if ( PTYPE(a->Data)->Id == Id ) return PTYPE(a->Data);
			a = a->Next;
		} while ( a != b );
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void WriteInt ( PFile f, void *o ) {
	FileWrite(o,1,sizeof(l_int),f);
}
////////////////////////////////////////////////////////////////////////////////
void WriteBool ( PFile f, void *o ) {
	FileWrite(o,1,sizeof(l_bool),f);
}
////////////////////////////////////////////////////////////////////////////////
void *ReadInt ( PFile f ) {
	void * o =malloc(sizeof(l_int));	
	if( !o ) return NULL;
	FileRead(o,1,sizeof(l_int),f);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void *ReadBool ( PFile f ) {
	void * o =malloc(sizeof(l_bool));	
	if( !o ) return NULL;
	FileRead(o,1,sizeof(l_bool),f);
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void *DuplicateInt ( void *o ) {
	void *d = malloc(sizeof(int));	
	memcpy(d,o,sizeof(int));
	return d;
}
////////////////////////////////////////////////////////////////////////////////
void *DuplicateBool ( void *o ) {
	void *d = malloc(sizeof(l_bool));	
	memcpy(d,o,sizeof(l_bool));
	return d;
}
////////////////////////////////////////////////////////////////////////////////
void FreeWidgetProp ( PWidgetProp o ) {
	if ( o->FreeExtra ) o->FreeExtra(o->Extra);
	free(o);	
}
////////////////////////////////////////////////////////////////////////////////
void FreeWidgetPropFlag ( PWidgetPropFlag o ) {
	
	if ( o->Name ) free(o->Name);
	if ( o->Description ) free(o->Description);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void WClassRegisterPropEx ( PWidgetClass c, l_ulong tid, l_text name, void *Extra, void (*FreeExtra) ( void *Extra ) ) {
	PPropType t;
	if ( t = GetPropType(tid) ) {
		PWidgetProp Prop = malloc(sizeof(TWidgetProp));
		if ( Prop ) {
			Prop->Type = t;
			if ( tid == PTYPE_FLAGS ) {
				Prop->Extra = NewList();
				Prop->FreeExtra = (void*)&FreeList;
			} else {
				Prop->Extra = Extra;
				Prop->FreeExtra = FreeExtra;
			}
			ListAdd(c->Properties,name,Prop,(void*)&FreeWidgetProp);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void WClassRegisterProp ( PWidgetClass c, l_ulong tid, l_text name ) {
	WClassRegisterPropEx(c,tid,name,NULL,NULL);
}

////////////////////////////////////////////////////////////////////////////////
PWidgetProp WClassGetProp ( PWidgetClass c, l_text name ) {
	PWidgetProp a;
	if ( a = ListKey(c->Properties,name) ) return a;
	if ( c->Parent ) return WClassGetProp(c->Parent,name);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void WClassResgisterFlag ( PWidgetClass c, l_text Property, l_ulong Value, l_text Name, l_text Description ) {
	PWidgetProp p = WClassGetProp(c,Property);
	if ( p ) {
		if ( p->Type->Id == PTYPE_FLAGS ) {
			PWidgetPropFlag o = malloc(sizeof(TWidgetPropFlag));
			if ( !o ) return;
			o->Value = Value;
			o->Name = TextDup(Name);
			o->Description = TextDup(Description);
			ListAdd((PList)p->Extra,Name,o,(void*)&FreeWidgetPropFlag);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
	@}
	@}
*/
////////////////////////////////////////////////////////////////////////////////
void WClassInstall ( void ) { 
	
	PropTypes = NewList(); 
	WidgetClasses = NewList();

	CreatePropType(PTYPE_STRING,(void*)&TextDup,&free,(void*)&FileWriteBinString,(void*)FileReadBinString);
	CreatePropType(PTYPE_INT,&DuplicateInt,&free,&WriteInt,&ReadInt);
	CreatePropType(PTYPE_BOOL,&DuplicateBool,&free,&WriteBool,&ReadBool);
	CreatePropType(PTYPE_MESSAGE,&DuplicateInt,&free,&WriteInt,&ReadInt);
	CreatePropType(PTYPE_FLAGS,&DuplicateInt,&free,&WriteInt,&ReadInt);
	
}
////////////////////////////////////////////////////////////////////////////////
void WClassUnInstall ( void ) {
	FreeList(WidgetClasses);
	FreeList(PropTypes);
}
////////////////////////////////////////////////////////////////////////////////
