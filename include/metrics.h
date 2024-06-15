////////////////////////////////////////////////////////////////////////////////
//
//  Metrics - Header File
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _METRICS_H_INCLUDED_
#define _METRICS_H_INCLUDED_
/**
	@addtogroup metrics
	@{
*/

/**	@file
	Metrics
	Declarations and structures for metrics.
*/


/**
	 2 dimension point
*/
typedef struct TPoint
{
	l_int x; ///< x position (horizontal)
	l_int y; ///< y position (vertical)

} TPoint, *PPoint;

/**
	 3 dimension point
*/
typedef struct TSPoint
{
	l_int x; ///< x position
	l_int y; ///< y position
	l_int z; ///< z position
} TSPoint, *PSPoint;

/**
	2 dimension rectangle
*/
typedef struct TRect
{
	TPoint a; ///< Minimal point (Minimal value of x and y)
  	TPoint b; ///< Maximum point (Maximum value of x and y)

} TRect, *PRect;

/**
	2 dimension rectange size
*/
typedef struct TSize
{
	l_int	w; ///< Width
	l_int	h; ///< Height

} TSize, *PSize;

l_bool PointInRect	( TPoint p, TRect r );
l_bool  RectOverlay ( TRect r, TRect d );

void RectIntersept( TRect *i , TRect r, TRect d );
void RectMove ( TRect *d, TRect r, TPoint p);
void RectUnMove ( TRect *d, TRect r, TPoint p);
void RectAssign( TRect *r,l_int ax, l_int ay, l_int bx, l_int by );
void PointAssign( TPoint *p, l_int x, l_int y );

l_int GetXDelta		( TRect r );
l_int GetYDelta		( TRect r );
l_int GetWidth		( TRect r );
l_int GetHeight		( TRect r );
TSize MakeSize(l_int w, l_int h);


PList NewListFromRect ( TRect r );
PList NewListFromRectList ( PList l );
PList NewListFromRectListIntersept ( PList l, TRect r );
void AddRectInList ( PList List, l_int ax, l_int ay, l_int bx, l_int by );
void RemoveZoneInListItem ( PList List, PListItem A, TRect b );

void RemoveZoneInList ( PList l, TRect r );
void RemoveZoneInListV ( PList l, TRect r );

void AddZoneInList ( PList l, TRect r );
void AddZoneInListEx ( PList List, int ax, int ay, int bx, int by );
l_bool RectOverlayList ( PList List, TRect r );

#define SizeAssign(s,w,h) PointAssign((TPoint*)s,w,h)

/**
	Inline version of PointInRect (faster, use only if you need high performances)
	@see PointInRect
*/
#define _PointInRect(p,r) (p.x >= r.a.x && p.y >= r.a.y && p.x <= r.b.x && p.y <= r.b.y)
/**
	Inline version of RectOverlay (faster, use only if you need high performances)
	@see RectOverlay
*/
#define _RectOverlay(r,d) !( (r).a.x > (d).b.x || (r).a.y > (d).b.y || (r).b.x < (d).a.x || (r).b.y < (d).a.y )
/**
	Inline version of RectIntersept (faster, use only if you need high performances)
	@see RectIntersept
*/
#define _RectIntersept(i,r,d) (i)->b.x = max((r).a.x, min((r).b.x, (d).b.x)); (i)->b.y = max((r).a.y, min((r).b.y, (d).b.y)); (i)->a.x = min((r).b.x, max((r).a.x, (d).a.x)); (i)->a.y = min((r).b.y, max((r).a.y, (d).a.y));

/**
@}
*/

#endif /* _METRICS_H_INCLUDED_ */

