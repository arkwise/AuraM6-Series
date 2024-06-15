////////////////////////////////////////////////////////////////////////////////
//
//    List Manager - Header file
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LIST_H_INCLUDED_
#define _LIST_H_INCLUDED_

/**
	@addtogroup list
	@{
*/

/**	@file
	List Header file.
	Declarations and inlines functions.
*/

typedef struct TBListItem *PBListItem;
/**
	Basic List Item
*/
typedef struct TBListItem {
	PBListItem Next;
	PBListItem Prev;
} TBListItem;

#define BLISTITEM(o) ((PBListItem)(o))

/**
	Simple List Item 
*/
typedef struct TSListItem {
	struct TBListItem o;
	void *Data;
} TSListItem,*PSListItem;

#define SLISTITEM(o) ((PSListItem)(o))

/**
	Basic and Simple List (identical) 
*/
typedef struct TBList {
	PBListItem Last;
} TBList, *PBList, *PSList;

#define BLIST(o) ((PBList)(o))

void BListRemoveItemEx ( PBList o, PBListItem i );
PSListItem SListAdd ( PBList o, void *Data );
PBListItem BListAddItem ( PBList o, PBListItem i );
void FreeSList ( PBList o, void (*FreeData) (void*)  );
PBList NewBList ( void );
PSListItem SListFoundItem ( PBList o, void *Data );


_INLINE PSListItem SListFoundItem ( PBList o, void *Data ) {
	PBListItem a, b;
	if ( !o->Last ) return NULL;
	a = b = o->Last->Next;
	if ( a ) do {
		if ( SLISTITEM(a)->Data == Data ) return SLISTITEM(a);
		a = a->Next;
	} while ( a != b );
	return NULL;
}

_INLINE PBList NewBList ( void ) {
	PBList o = (PBList)malloc(sizeof(TBList));
	o->Last = NULL;
	return o;
}

_INLINE void FreeSList ( PBList o, void (*FreeData) (void*)  ) {
	PBListItem a,b,n;
	if ( o->Last ) {
		a = b = o->Last;
		do {
			n = a->Prev;
			if ( FreeData && SLISTITEM(a)->Data ) FreeData(SLISTITEM(a)->Data);
			free(a);
			a = n;
		} while ( a != b );
	}
	free(o);
}

_INLINE PBListItem BListAddItem ( PBList o, PBListItem i ) {
	if ( o->Last ) {
		i->Next = o->Last->Next;
		i->Prev = o->Last;
		o->Last->Next->Prev = i;
		o->Last->Next = i;
		o->Last = i;
	} else {
		i->Next = i;
		i->Prev = i;
		o->Last = i;
	}
	return i;
}

_INLINE PSListItem SListAdd ( PBList o, void *Data ) {
	PSListItem i = (PSListItem)malloc(sizeof(TSListItem));
	i->Data = Data;
	BListAddItem(o,BLISTITEM(i));
	return i;
}

_INLINE void BListRemoveItemEx ( PBList o, PBListItem i ) {
  if ( i->Next == i ) {
	  o->Last = NULL;
  } else {
	  i->Next->Prev = i->Prev;
	  i->Prev->Next = i->Next;
	  if ( o->Last == i ) o->Last = i->Prev;
  }
}

typedef struct TListItem *PListItem;
typedef struct TListItem
{
	PListItem Next;
	PListItem Prev;
	void *Data;
	
	l_ulong DataType;
	char *Key;
	void (*FreeData) (void*);
} TListItem;

typedef struct TList *PList;
typedef struct TList
{
	PListItem Last;
	PListItem Current;
} TList;

PListItem  NewListItemEx   ( char *Key, void *Data, void (*FreeData) (void*), l_ulong DataType, l_ulong DataSize );
PListItem  NewListItem     ( char *Key, void *Data, void (*FreeData) (void*) );
PListItem  ListAddItem     ( PList o, PListItem i );
PListItem  ListAdd         ( PList o, char *Key, void *Data, void (*FreeData) (void*) );
char       ListRemoveItem  ( PList o, PListItem i );
PListItem  ListAtItem      ( PList o, unsigned long Index );
void      *ListAt          ( PList o, unsigned long Index );
PListItem  ListKeyItem     ( PList o, char *Key );
void      *ListKey         ( PList o, char *Key );
PListItem  ListKeyCaseItem     ( PList o, char *Key );
void      *ListKeyCase         ( PList o, char *Key );
PListItem  ListFoundItem   ( PList o, void *Data );
PList      NewList         ( void );
unsigned long ListFoundIndex ( PList o, void *Data );
PListItem ListAddItemBefore ( PList o, PListItem i, PListItem b );
char ListRemoveItemEx ( PList o, PListItem i );

// /!\ This 2 functions dot no order Items in List, but Data /!\ ->-> To be improved ...
char       ListOrderByData ( PList o, long (*comparator) ( void*,void*) );
char       ListOrderByItem ( PList o, long (*comparator) ( PListItem,PListItem) );

// free list, all items and data. Ideal for most uses.
void FreeList ( PList o );

// Same as freelist but do not free data. (skip DataFree)
void KickList ( PList o );

#endif /* _LIST_H_INCLUDED_ */
