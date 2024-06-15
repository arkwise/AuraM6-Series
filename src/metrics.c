/*
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			rect.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Metric functions (rect and point calculations). Version 2
*
*	CONTRIBUTORS:
*					Lukas Lipka
*         Etelain Julien
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/

/** @defgroup metrics Metrics
	Rect, point and rect list calculations.
	@{
*/
/**	@file
	Metric functions 
	Rect, point and rect list calculations.
*/

#include "kernel.h"
#include <stdio.h>
#include <stdarg.h>

/*
	Point functions
*/

/**
	Assign a point with the specified x and y coordinates.
	@param p Pointer to the point to assign
	@param x Value of x
	@param y Value of y
*/
void PointAssign( TPoint *p, l_int x, l_int y )
{
	p->x = x;
 	p->y = y;
}

/**
	Calculates wherever a point is located in the specified rectangle.
	@return true if the rect contains the point, otherwise false
*/
l_bool PointInRect ( TPoint p, TRect r )
{
	return (p.x >= r.a.x && p.y >= r.a.y && p.x <= r.b.x && p.y <= r.b.y);
}

/*
	Rect functions
*/

/**
	Assigns the specified coordinates to a rectangle structure.
	Passed values are not checked.
	@param r Pointer to the rect to assign
	@param ax Minimal value of x
	@param ay Minimal value of y
	@param bx Maximal value of x
	@param by Maximal value of y
*/
void RectAssign( TRect *r, l_int ax, l_int ay, l_int bx, l_int by )
{
	r->a.x = ax;
	r->a.y = ay;
	r->b.x = bx;
	r->b.y = by;
}


/**
	Moves a rectangle using a point to define x and y move.
	@param d Rect where result will be assigned
	@param r Rect to move
	@param p Point defining the move
	@see RectUnMove
*/
void RectMove ( TRect *d, TRect r, TPoint p)
{
	*d = r;

	d->a.x += p.x;
	d->a.y += p.y;
	d->b.x += p.x;
	d->b.y += p.y;
}

/**
	Moves a rectangle back from a prevoius call of RectMove. It corresponds to a negative move.
	@param d Rect where result will be assigned
	@param r Rect to move
	@param p Point defining the move
	@see RectMove
*/
void RectUnMove ( TRect *d, TRect r, TPoint p)
{
	*d = r;

	d->a.x -= p.x;
	d->a.y -= p.y;
	d->b.x -= p.x;
	d->b.y -= p.y;
}

/*
	Size functions for rect
*/

/**
	Calculates the position of the x coordinate of the rectangle
	@return x varation between r.b and r.a
*/
l_int GetXDelta(TRect r)
{
	return r.b.x-r.a.x;
}

/**
	Calculates the position of the y coordinate of the rectangle
	@return y varation between r.b and r.a
*/
l_int GetYDelta(TRect r)
{
	return r.b.y-r.a.y;
}

/**
	Calculates the width of the rectangle
	@return width of rectangle
*/
l_int GetWidth(TRect r)
{
	return r.b.x-r.a.x+1;
}

/**
	Calculates the height of the rectangle
	@return height of rectangle
*/
l_int GetHeight(TRect r)
{
	return r.b.y-r.a.y+1;
}

/**
	Dertermines if rects @a r and @a d has a common part
	@return 'true' if both rects has a common part, else returns false.
*/
l_bool  RectOverlay ( TRect r, TRect d )
{
	//if ( r.a.x > d.b.x || r.a.y > d.b.y || r.b.x < d.a.x || r.b.y < d.a.y )
	//	return false;

	return _RectOverlay(r, d);
}

/**
	Calculate the common part of @a r and @a d ans assign it into @a i
	@param i Rect to assign the result in
	@param r First rect
	@param d Second rect
	@warning @a r and @a d MUST have a common part (use RectOverlay to be sure)
*/
void  RectIntersept ( TRect *i , TRect r, TRect d )
{
	i->b.x = max(r.a.x, min(r.b.x, d.b.x));
	i->b.y = max(r.a.y, min(r.b.y, d.b.y));
	i->a.x = min(r.b.x, max(r.a.x, d.a.x));
	i->a.y = min(r.b.y, max(r.a.y, d.a.y));
}

////////////////////////////////////////////////////////////////////////////////
// Rect List system, mostly used by widget system


l_bool RectOverlayList ( PList List, TRect r ) {
	PListItem a, b;
	TRect t;
	if ( List->Last ) {
		a = b = List->Last;
		do {
			t = *(PRect)a->Data;
			if ( _RectOverlay(r,t) ) return true;
			a = a->Next;
		} while ( a != b );
	}
	return false;
}



INLINE void _AddRectInList ( PList List, int ax, int ay, int bx, int by )
{
	if ( ax <= bx && ay <= by )
	{
		PRect r = (PRect)malloc(sizeof(TRect));
		r->a.x = ax; r->a.y = ay;
		r->b.x = bx; r->b.y = by;
		ListAdd(List,NULL,r,free);
	}
}

PList NewListFromRect ( TRect t )
{
	PList List = NewList();
	
	_AddRectInList(List,t.a.x,t.a.y,t.b.x,t.b.y);

	return List;
}

PList NewListFromRectList ( PList l )
{
	PListItem a, b;
	TRect t;
	PList List = NewList();
	if ( l->Last ) {
		a = b = l->Last;
		do {
			t = *(PRect)a->Data;
			_AddRectInList(List,t.a.x,t.a.y,t.b.x,t.b.y);
			a = a->Next;
		}
		while ( a != b );
	}
	return List;
}

PList NewListFromRectListIntersept ( PList l, TRect r )
{
	PListItem a, b;
	TRect t;
	PList List = NewList();
	if ( l->Last ) {
		a = b = l->Last;
		do {
			t = *(PRect)a->Data;
			if ( _RectOverlay (t,r) ) {
				_AddRectInList(List,min(r.b.x, max(r.a.x, t.a.x)),min(r.b.y, max(r.a.y, t.a.y)),
				max(r.a.x, min(r.b.x, t.b.x)),max(r.a.y, min(r.b.y, t.b.y)));
			}
			a = a->Next;
		}
		while ( a != b );
	}
	return List;
}
/*
#define AddBminusA(aa,bb,List,func) \
if ( aa.a.x < bb.a.x ) \
	func(List,aa.a.x,aa.a.y,bb.a.x-1,aa.b.y); \
if ( aa.a.y < bb.a.y ) \
	func(List,max(bb.a.x,aa.a.x),aa.a.y,aa.b.x,bb.a.y-1); \
if ( aa.b.x > bb.b.x ) \
	func(List,bb.b.x+1,max(aa.a.y,bb.a.y),aa.b.x,min(bb.b.y,aa.b.y)); \
if ( aa.b.y > bb.b.y ) \
	func(List,max(aa.a.x,bb.a.x),bb.b.y+1,aa.b.x,aa.b.y);
*/
	
#define AddBminusAHPref(aa,bb,List,func) \
if ( aa.a.x < bb.a.x ) \
	func(List,aa.a.x,max(aa.a.y,bb.a.y),bb.a.x-1,min(bb.b.y,aa.b.y)); \
if ( aa.a.y < bb.a.y ) \
	func(List,aa.a.x,aa.a.y,aa.b.x,bb.a.y-1); \
if ( aa.b.x > bb.b.x ) \
	func(List,bb.b.x+1,max(aa.a.y,bb.a.y),aa.b.x,min(bb.b.y,aa.b.y)); \
if ( aa.b.y > bb.b.y ) \
	func(List,aa.a.x,bb.b.y+1,aa.b.x,aa.b.y);
	
#define AddBminusAVPref(aa,bb,List,func) \
if ( aa.a.x < bb.a.x ) \
	func(List,aa.a.x,aa.a.y,bb.a.x-1,aa.b.y); \
if ( aa.a.y < bb.a.y ) \
	func(List,max(aa.a.x,bb.a.x),aa.a.y,min(aa.b.x,bb.b.x),bb.a.y-1); \
if ( aa.b.x > bb.b.x ) \
	func(List,bb.b.x+1,aa.a.y,aa.b.x,aa.b.y); \
if ( aa.b.y > bb.b.y ) \
	func(List,max(aa.a.x,bb.a.x),bb.b.y+1,min(aa.b.x,bb.b.x),aa.b.y);

INLINE void RemoveZoneInListItemEx ( PList List, PListItem A, TRect b )
{
	TRect a = *(PRect)A->Data;

	if ( _RectOverlay (a,b) )
	{
		AddBminusAHPref(a,b,List,_AddRectInList);
		ListRemoveItem(List,A);
	}
}

void RemoveZoneInList ( PList l, TRect r )
{
	PListItem a, b, n;
	int c = 0;
	TRect t;
	
	if ( l->Last )
	{
		a = b = l->Last;

		do
		{
			c++;
			a = a->Next;
		}
		while ( a != b );
		
		do
		{
			t = *(PRect)a->Data;
			if ( _RectOverlay (t,r) ) {
				n = a->Next;
				AddBminusAHPref(t,r,l,_AddRectInList);
				ListRemoveItem(l,a);
				a = n;
			} else
				a = a->Next;
			c--;
		}
		while ( c );
	}
}

void RemoveZoneInListV ( PList l, TRect r )
{
	PListItem a, b, n;
	int c = 0;
	TRect t;
	
	if ( l->Last )
	{
		a = b = l->Last;

		do
		{
			c++;
			a = a->Next;
		}
		while ( a != b );
		
		do
		{
			t = *(PRect)a->Data;
			if ( _RectOverlay (t,r) ) {
				n = a->Next;
				AddBminusAVPref(t,r,l,_AddRectInList);
				ListRemoveItem(l,a);
				a = n;
			} else
				a = a->Next;
			c--;
		}
		while ( c );
	}
}

void AddZoneInListEx ( PList List, int ax, int ay, int bx, int by ) {
	TRect r;
	r.a.x = ax; 
	r.a.y = ay;
	r.b.x = bx; 
	r.b.y = by;
	AddZoneInList(List,r);
}

void AddZoneInList ( PList l, TRect r )
{
	
	PListItem a, b;
	int c = 0;
	TRect t;
	
	if ( l->Last )
	{
		a = b = l->Last;

		do
		{
			c++;
			a = a->Next;
		}
		while ( a != b );
		
		do
		{
			t = *(PRect)a->Data;
			if ( _RectOverlay (t,r) ) {
				AddBminusAHPref(r,t,l,AddZoneInListEx);
				return;
			}
			a = a->Next;
			c--;
		}
		while ( c );
	}
	
	_AddRectInList(l,r.a.x,r.a.y,r.b.x,r.b.y);
}
/**
@}
*/
