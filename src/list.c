////////////////////////////////////////////////////////////////////////////////
//
//    List Manager - Core file
//
//  (c) Copyright 2003 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "types.h"
#include "list.h"
#include "text.h" 
#include <stdlib.h>
#include <string.h>

/** @defgroup list List system.
        Widely used in kernel and libraries, PBList and derivated are very usefull.
        PList is the most used system as it is the oldest and have interresting functionalities.
        PBList and PSList provide an inline module, so very fast, and lightweight structures but have little funtionalities.
        @n PSList is derivated form PBList : PBList functions can be used for PSList.
        @n PList is derivated from PSList : PBList and PSList functions can be used for PList (to get better performances).
        @n *BUT* do not allocate a PSList item in a PList, system will crash.
        @{
*/
/**     @file
        List core file
*/

////////////////////////////////////////////////////////////////////////////////
PListItem NewListItem ( char *Key, void *Data, void (*FreeData) (void*) ) {
        PListItem i = (PListItem)malloc(sizeof(TListItem));
        if ( !i )
          return NULL;
        memset(i,0,sizeof(TListItem));
        i->Data = Data;

        if ( Key )
                i->Key = (char*)strdup(Key);

        i->FreeData = FreeData;
        return i;
}
////////////////////////////////////////////////////////////////////////////////
PListItem NewListItemEx ( char *Key, void *Data, void (*FreeData) (void*), l_ulong DataType, l_ulong DataSize ) {
        PListItem i = (PListItem)malloc(sizeof(TListItem));
        if ( !i )
          return NULL;
        memset(i,0,sizeof(TListItem));
        i->Data = Data;

        if ( Key )
                i->Key = (char*)strdup(Key);

        i->FreeData = FreeData;
        i->DataType = DataType;
        return i;
}
////////////////////////////////////////////////////////////////////////////////
PListItem ListAddItemBefore ( PList o, PListItem i, PListItem b ) {
        if ( !o->Last )
        {
                i->Next = i;
                i->Prev = i;
                o->Last = i;
        }
        else
        {
                if ( b )
                {
                        i->Prev = b->Prev;
                        i->Next = b;
                        b->Prev->Next = i;
                        b->Prev = i;
                }
                else
                {
                        i->Next = o->Last->Next;
                        i->Prev = o->Last;
                        o->Last->Next->Prev = i;
                        o->Last->Next = i;
                        o->Last = i;
                }
        }
        return i;
}
////////////////////////////////////////////////////////////////////////////////
PListItem ListAddItem ( PList o, PListItem i ) {
        BListAddItem(BLIST(o),BLISTITEM(i));
        return i;
}
////////////////////////////////////////////////////////////////////////////////
PListItem ListAdd ( PList o, char *Key, void *Data, void (*FreeData) (void*) ) {
        PListItem i = NewListItem(Key,Data,FreeData);
        if ( !i )
        return NULL;
        return (PListItem)BListAddItem(BLIST(o),BLISTITEM(i));
}
////////////////////////////////////////////////////////////////////////////////
char ListRemoveItemEx ( PList o, PListItem i ) {
        BListRemoveItemEx(BLIST(o),BLISTITEM(i));
        return 1;
}
////////////////////////////////////////////////////////////////////////////////
char ListRemoveItem ( PList o, PListItem i ) {
        BListRemoveItemEx(BLIST(o),BLISTITEM(i));
        if ( i->Key ) free(i->Key);
        if ( i->Data && i->FreeData ) i->FreeData(i->Data);
        free(i);
        return 1;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the item that index is specified one.
        @param o List containing the item
        @param Index Index to found
        @return NULL on error, or searched data
*/
PListItem ListAtItem ( PList o, unsigned long Index ) {
        PListItem a, b;
  if ( !o->Last ) return NULL;
        a = b = o->Last->Next;
          Index--;
  if ( a ) do {
          if ( !Index ) return a;
          Index--;
          a = a->Next;
  } while ( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the data associated to item that index is specified one.
        @param o List containing the item
        @param Index Index to found
        @return NULL on error, or searched data
*/
void *ListAt ( PList o, unsigned long Index ) {
        PListItem a = ListAtItem(o,Index);
        if ( a ) return a->Data;
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the item that Key is specified one. Case sensitive.
        @param o List containing the item
        @param Key Item key to found
        @return NULL on error, or a pointer to item
*/
PListItem ListKeyItem ( PList o, char *Key ) {
        PListItem a, b;
  if ( !o->Last ) return NULL;
        a = b = o->Last->Next;
  if ( a ) do {
          if ( a->Key ) if ( !TextCompare(Key,a->Key) ) return a;
          a = a->Next;
  } while ( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the data associated to item that Key is specified one. Case sensitive.
        @param o List containing the item
        @param Key Item key to found
        @return NULL on error, or searched data
*/
void *ListKey ( PList o, char *Key ) {
        PListItem a = ListKeyItem(o,Key);
        if ( a ) return a->Data;
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the item that Key is specified one ignoring the case.
        @param o List containing the item
        @param Key Item key to found
        @return NULL on error, or a pointer to item
*/
PListItem ListKeyCaseItem ( PList o, char *Key ) {
        PListItem a, b;
  if ( !o->Last ) return NULL;
        a = b = o->Last->Next;
  if ( a ) do {
          if ( a->Key ) if ( !TextCaseCompare(Key,a->Key) ) return a;
          a = a->Next;
  } while ( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the data associated to item that Key is specified one ignoring the case.
        @param o List containing the item
        @param Key Item key to found
        @return NULL on error, or searched data
*/
void *ListKeyCase ( PList o, char *Key ) {
        PListItem a = ListKeyCaseItem(o,Key);
        if ( a ) return a->Data;
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Found the item that Data is specified one.
        @param o List containing the item
        @param Data Data to found
        @return NULL on error, or a pointer to item
        @note Similar to ((PListItem)SListFoundItem((PSList)o,Data))
*/
PListItem ListFoundItem ( PList o, void *Data ) {
        PListItem a, b;
  if ( !o->Last ) return NULL;
        a = b = o->Last->Next;
  if ( a ) do {
          if ( a->Data == Data ) return a;
          a = a->Next;
  } while ( a != b );
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Determines the index of item that Data is specified one.
        @param o List containing the item
        @param Data Data to found
        @return -1 on error, or item index
*/
unsigned long ListFoundIndex ( PList o, void *Data ) {
        PListItem a, b;
        l_ulong i = 1;
  if ( !o->Last ) return 0;
        a = b = o->Last->Next;
  if ( a ) do {
          if ( a->Data == Data ) return i;
          a = a->Next;
          i++;
  } while ( a != b );
        return -1;
}
////////////////////////////////////////////////////////////////////////////////
char ListOrderByData ( PList o, long (*comparator) ( void*,void*) ) {
        PListItem c, a, b;
        TListItem t;
  if ( !o && !o->Last && !o->Last->Next && !comparator ) return 0; //fix 27.01.2007
        c = a = o->Last->Next;
        do {
          b = a;
                do {
      if ( a->Data && b->Data && comparator( a->Data, b->Data ) > 0 ) {
             /* char * aKey = aKey;
              void * aData = a->Data;
              a->Data = b->Data;
              a->Key  = b->Key;
              b->Data = aData;
              b->Key  = aKey;*/

       memcpy((void*)(((long)&t)+8),(void*)(((long)a)+8),sizeof(TListItem)-8);
       memcpy((void*)(((long)a)+8),(void*)(((long)b)+8),sizeof(TListItem)-8);
       memcpy((void*)(((long)b)+8),(void*)(((long)&t)+8),sizeof(TListItem)-8);

      }
      b = b->Next;
    } while ( c != b );
    a = a->Next;
  } while ( c != a );
  return 1;
}
////////////////////////////////////////////////////////////////////////////////
char ListOrderByItem ( PList o, long (*comparator) ( PListItem,PListItem) ) {
        PListItem c, a, b;
        TListItem t;
  if ( !o->Last && !comparator ) return 0;
        c = a = o->Last->Next;
        do {
          b = a;
                do {
      if ( comparator( a, b ) > 0 ) {
              /*char * aKey = a->Key;
              void * aData = a->Data;
              a->Data = b->Data;
              a->Key  = b->Key;
              b->Data = aData;
              b->Key  = aKey;*/

                memcpy((void*)(((long)&t)+8),(void*)(((long)a)+8),sizeof(TListItem)-8);
                                memcpy((void*)(((long)a)+8),(void*)(((long)b)+8),sizeof(TListItem)-8);
              memcpy((void*)(((long)b)+8),(void*)(((long)&t)+8),sizeof(TListItem)-8);

      }
      b = b->Next;
    } while ( c != b );
    a = a->Next;
  } while ( c != a );
  return 1;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Creates a PList
        @note Never forget to free the returned list after use.
        @see FreeList
*/
PList NewList ( void ) {
        PList o = (PList)malloc(sizeof(TList));
        if ( !o )
          return NULL;
        memset(o,0,sizeof(TList));
        return o;
}
////////////////////////////////////////////////////////////////////////////////
/**
        Free a list and items data (using the FreeData procedure if specified, else doesn't free data)
        @param o List to free
*/
void FreeList ( PList o ) {
        PListItem a, b,n;
        if ( o->Last ) {
        a = b = o->Last;
        do {
                n = a->Prev;
                if ( a->Key ) free(a->Key);
                if ( a->Data && a->FreeData ) a->FreeData(a->Data);
                free(a);
                a = n;
        } while ( a != b );
  }
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
/**
        Free a List but do not free items data.
        @note Similar to FreeSList((PSList)o,NULL)
*/
void KickList ( PList o ) {
        PListItem a, b,n;
        if ( o->Last ) {
        a = b = o->Last;
        do {
                n = a->Prev;
                if ( a->Key ) free(a->Key);
                free(a);
                a = n;
        } while ( a != b );
  }
        free(o);
}
////////////////////////////////////////////////////////////////////////////////
