#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "canvas.h"
#include "slider.h"
#include "iodlg.h"



l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "Photo Orientation";
l_uid	nUID		= "app:imgor";
l_uid NeededLibs[]	= { "widget", "button","label","window","" };

PCanvas	c 	= 0;
PWindow w = 0;

PImage SmllBuf = NULL;
PImage SmllImg = NULL;
PImage Original = NULL;
PSlider Sl;

TRect Zone = { 0, 0, 255, 255 };
TPoint Origin;

l_text File = 0;

float ax=0,ay=0,az=0,zoom=0,foc=128;
float ScaleReal = 1;

#define MSG_AXP		0x20101
#define MSG_AXM		0x20102
#define MSG_AYP		0x20103
#define MSG_AYM		0x20104
#define MSG_AZP		0x20105
#define MSG_AZM		0x20106
#define MSG_RESET	0x20107
#define MSG_DOWORK	0x20108

#define ProjectPoint( v, p,  cx,  cy,  f ) \
	p.x = cx + ((v.x-cx)*f/(f+v.z)); \
	p.y = cy + ((v.y-cy)*f/(f+v.z))
	
void Wire(PImage Dst, V3D_f *v1, V3D_f *v2, l_int f )
{
	TPoint p1,p2;	
	l_int cx = (Dst->w)/2, cy = (Dst->h)/2;
	ProjectPoint((*v1),p1,cx,cy,f);
	ProjectPoint((*v2),p2,cx,cy,f);
	line(Dst, p1.x, p1.y, p2.x, p2.y, COL_3DDARK);
}

inline void ChunkPixel ( PImage Dst, void *cnt, l_int x, l_int y, l_int r, l_int g, l_int b ) {
	if ( (x >= 0) && (y >= 0) && (x < Dst->w) && (y < Dst->h) ) {
		unsigned char *cur = (char*)((l_long)cnt + (x+(y*Dst->w))); 
		if ( *cur ) {
			l_int c = getpixel(Dst,x,y);
			r = (r+((*cur)*getr(c)))/((*cur)+1);
			g = (g+((*cur)*getg(c)))/((*cur)+1);
			b = (b+((*cur)*getb(c)))/((*cur)+1);
		}
		(*cur)++;
		putpixel(Dst,x,y,makecol(r,g,b));	
	}
} 

inline void ChunkPixelSP ( PImage Dst, void *cnt, l_int x, l_int y, l_int r, l_int g, l_int b ) {
	if ( (x >= 0) && (y >= 0) && (x < Dst->w) && (y < Dst->h) ) {
		unsigned char *cur = (char*)((l_long)cnt + (x+(y*Dst->w))); 
		if ( !(*cur) ) 
			putpixel(Dst,x,y,makecol(r,g,b));	
	}
} 


void ProjectSurface ( PImage Dst, PImage Img, V3D_f *v1,V3D_f *v2,V3D_f *v4,l_int f ) {
	l_int x, y;
	V3D_f w,w2,u,v; 
	TPoint p,p2,d;
	unsigned char *cnt, *cur;
	l_int cx = (Dst->w)/2, cy = (Dst->h)/2,r,g,b,c,i,j,nr,ng,nb;	

	u.x = (v2->x-v1->x)/Img->w;
	u.y = (v2->y-v1->y)/Img->w;
	u.z = (v2->z-v1->z)/Img->w;
	
	v.x = (v4->x-v1->x)/Img->h;
	v.y = (v4->y-v1->y)/Img->h;
	v.z = (v4->z-v1->z)/Img->h;
	
	cnt = malloc(Dst->w*Dst->h);
	memset(cnt,0,Dst->w*Dst->h);
	
	for(y=0;y<Img->h;y++) {
		
		w.x = v1->x+(v.x*y);
		w.y = v1->y+(v.y*y);
		w.z = v1->z+(v.z*y);

		for(x=0;x<Img->w;x++) {
			
			c = getpixel(Img,x,y);
			r = getr(c);
			g = getg(c);
			b = getb(c);
						
			w.x += u.x;
			w.y += u.y;
			w.z += u.z;

			ProjectPoint(w,p,cx,cy,f);
			ChunkPixel(Dst,cnt,p.x,p.y,r,g,b);
			
			w2.x = w.x+(u.x+v.x);
			w2.y = w.y+(u.y+v.y);
			w2.z = w.z+(u.z+v.z);
			ProjectPoint(w2,p2,cx,cy,f);
			if ( (p.x != p2.x) || (p.y != p2.y) ) {
				d.x = max(p.x,p2.x);
				d.y = max(p.y,p2.y);
				p.x = min(p.x,p2.x);
				p.y = min(p.y,p2.y);
				for (i=p.x;i<=d.x;i++)
					for (j=p.y;j<=d.y;j++) {
						nr = (r+getr(c))/2;
						ng = (g+getg(c))/2;
						nb = (b+getb(c))/2;
						ChunkPixelSP(Dst,cnt,i,j,r,g,b);
					}
			}
		}
	}
	
	free(cnt);
}

void CalculateSmall ( void ) {
	int i = 0;
	MATRIX_f Rotation = { 0 };
	MATRIX_f Trans2 = { 0 };
	MATRIX_f Trans1 = { 0 };
	MATRIX_f Final = { 0 };
	
	V3D_f v[4] = 
	{{   0,   0,   0 },
	 {   0,   0,   0 },
	 {   0,   0,   0 },
	 {   0,   0,   0 }};

	v[0].y = v[1].y = (SmllBuf->h-SmllImg->h)/2;  
	v[2].y = v[3].y = v[0].y + SmllImg->h;
	
	v[0].x = v[3].x = (SmllBuf->w-SmllImg->w)/2; 
	v[1].x = v[2].x = v[0].x + SmllImg->w; 

	get_rotation_matrix_f(&Rotation, ax, ay, az);
	get_translation_matrix_f(&Trans1, -128, -128, 0);
	get_translation_matrix_f(&Trans2, 128, 128, 0);
	
	matrix_mul_f(&Trans1, &Rotation, &Final);
	matrix_mul_f(&Final, &Trans2, &Final);

	zoom = 0;
	
	for (i =0; i<4; i++) {
		apply_matrix_f(&Final, v[i].x, v[i].y, v[i].z, &v[i].x, &v[i].y, &v[i].z);
		if ( (-v[i].z) > zoom ) zoom = -v[i].z;
	}
	
	for (i =0; i<4; i++) {
		v[i].z += zoom;
	}
	
	clear_to_color(SmllBuf,COL_3DDARK);

	ProjectSurface ( SmllBuf, SmllImg, &v[0], &v[1], &v[3], foc );
	
	if ( bitmap_color_depth(SmllBuf) != 8 ) {
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		set_trans_blender(255, 255, 255, 128);
		if ( Zone.a.y > 0 ) 
			rectfill(SmllBuf,0,0,255,Zone.a.y-1,makecol(255,255,255));
		if ( Zone.a.x > 0 ) 
			rectfill(SmllBuf,0,Zone.a.y,Zone.a.x-1,Zone.b.y,makecol(255,255,255));
		if ( Zone.b.y < 255 ) 
			rectfill(SmllBuf,0,Zone.b.y+1,255,255,makecol(255,255,255));
		if ( Zone.b.x < 255 ) 
			rectfill(SmllBuf,Zone.b.x+1,Zone.a.y,255,Zone.b.y,makecol(255,255,255));
		solid_mode();
	}
	rect(SmllBuf,Zone.a.x,Zone.a.y,Zone.b.x,Zone.b.y,makecol(255,0,0));


}

void ImgOrrDraw (PWidget o, p_bitmap buffer, PRect w)
{
	
	draw_sprite(buffer,SmllBuf,o->Absolute.a.x,o->Absolute.a.y);
	
	rect(buffer,w->a.x,w->a.y,w->b.x,w->b.y,COL_3DDARK);

}
l_bool  ZoneEventHandler ( PWidget o, PEvent Ev )
{
	if (Ev->Type == EV_MOUSE)
	{



		if (Ev->Message == WEvMouseLDown)
		{
			Origin.x = Mouse->State.p.x-o->Absolute.a.x;
			Origin.y = Mouse->State.p.y-o->Absolute.a.y;
			
			Zone.a.x = Origin.x;
			Zone.a.y = Origin.y;
			
			Zone.b.x = Origin.x;
			Zone.b.y = Origin.y;
			
			CalculateSmall();
			WidgetDraw(o, NULL);

			return true;
		}

		if ( Ev->Message == WEvMouseMove && (Mouse->State.b & BUTTON_LEFT) )
		{
			
			Zone.a.x = min(Origin.x,Mouse->State.p.x-o->Absolute.a.x);
			Zone.a.y = min(Origin.y,Mouse->State.p.y-o->Absolute.a.y);
			
			Zone.b.x = max(Origin.x,Mouse->State.p.x-o->Absolute.a.x);
			Zone.b.y = max(Origin.y,Mouse->State.p.y-o->Absolute.a.y);

			CalculateSmall();
			WidgetDraw(o, NULL);
			return true;
		}		
		
		if (Ev->Message == WEvMouseLUp )
		{
			
			Zone.a.x = min(Origin.x,Mouse->State.p.x-o->Absolute.a.x);
			Zone.a.y = min(Origin.y,Mouse->State.p.y-o->Absolute.a.y);
			
			Zone.b.x = max(Origin.x,Mouse->State.p.x-o->Absolute.a.x);
			Zone.b.y = max(Origin.y,Mouse->State.p.y-o->Absolute.a.y);

			CalculateSmall();
			WidgetDraw(o, NULL);
			return true;
		}
	}

	return false;
}

l_bool AppEventHandler ( PWidget o, PEvent Ev )
{
	if ( Ev->Type == EV_MESSAGE )
	{
		if ( Ev->Message == WM_CLOSE )
		{
			CloseApp(&Me);

			return true;
		}

		if ( Ev->Message == MSG_AXP ) {
			ax+=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}
		if ( Ev->Message == MSG_AXM ) {
			ax-=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}

		if ( Ev->Message == MSG_AYP ) {
			ay+=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}
		if ( Ev->Message == MSG_AYM ) {
			ay-=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}		
		
		if ( Ev->Message == MSG_AZP ) {
			az+=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}
		if ( Ev->Message == MSG_AZM ) {
			az-=1;
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}	
		if ( Ev->Message == MSG_RESET ) {
			Zone.a.y = (SmllBuf->h-SmllImg->h)/2;  
			Zone.b.y = Zone.a.y + SmllImg->h;
			Zone.a.x = (SmllBuf->w-SmllImg->w)/2; 
			Zone.b.x = Zone.a.x + SmllImg->w; 
			CalculateSmall();
			WidgetDrawAll(WIDGET(c));
			return true;
		}
		if ( Ev->Message == MSG_DOWORK ) {
			PProgressPop p;
			PImage Buffer = create_bitmap(ScaleReal*256,ScaleReal*256);
			PImage Out;
			TRect RealZone;
			int i = 0;
			MATRIX_f Rotation = { 0 };
			MATRIX_f Trans2 = { 0 };
			MATRIX_f Trans1 = { 0 };
			MATRIX_f Final = { 0 };
			
			
			
			V3D_f v[4] = 
			{{   0,   0,   0 },
			 {   0,   0,   0 },
			 {   0,   0,   0 },
			 {   0,   0,   0 }};
			 
			p = ProgressPopUp(&Me,1,"Photo orientation","Please wait.",0,"");
			
			ProgressPopSetInfo(p,"Calculating new image...");
			
		
			RealZone.a.x = Zone.a.x*ScaleReal;
			RealZone.a.y = Zone.a.y*ScaleReal;
			RealZone.b.x = (Zone.b.x+1)*ScaleReal;
			RealZone.b.y = (Zone.b.y+1)*ScaleReal;
			 
			v[0].y = v[1].y = (Buffer->h-Original->h)/2;  
			v[2].y = v[3].y = v[0].y + Original->h;
			
			v[0].x = v[3].x = (Buffer->w-Original->w)/2; 
			v[1].x = v[2].x = v[0].x + Original->w; 
			
			get_rotation_matrix_f(&Rotation, ax, ay, az);
			get_translation_matrix_f(&Trans1, -ScaleReal*128, -ScaleReal*128, 0);
			get_translation_matrix_f(&Trans2, ScaleReal*128, ScaleReal*128, 0);
			
			matrix_mul_f(&Trans1, &Rotation, &Final);
			matrix_mul_f(&Final, &Trans2, &Final);
		
			zoom = 0;
			
			for (i =0; i<4; i++) {
				apply_matrix_f(&Final, v[i].x, v[i].y, v[i].z, &v[i].x, &v[i].y, &v[i].z);
				if ( (-v[i].z) > zoom ) zoom = -v[i].z;
			}
			
			for (i =0; i<4; i++) {
				v[i].z += zoom;
			}
			
			clear_to_color(Buffer, COL_3DFACE);
			
			ProjectSurface ( Buffer, Original, &v[0], &v[1], &v[3], ScaleReal*foc );
			
			Out = create_bitmap(RealZone.b.x-RealZone.a.x,RealZone.b.y-RealZone.a.y);
			blit(Buffer,Out,RealZone.a.x,RealZone.a.y,0,0,Out->w,Out->h);
			
			destroy_bitmap(Buffer);
			
			ProgressPopSetInfo(p,"Saving image...");
			
			SaveData2( File, Out, DTYPE_IMAGE );
			
			destroy_bitmap(Out);
			
			ProgressPopKick(p);
			
			CloseApp(&Me);
			
			return true;
		}

		
	}

	return false;
}
void SlNotify ( PSlider o, l_uchar Ctx, l_ulong Value ) {
	
	foc = 16+Value;
	CalculateSmall();
	WidgetDrawAll(WIDGET(c));
	
}

l_int Main ( int argc, l_text *argv )
{
	TRect r;
	PButton b = NULL;
	PLabel l;
	l_int wi, hi;
	
	if ( argc > 1 )
		File = TextDup(argv[1]);
	else {
		
		PFileTypes Filter = 	NewFileTypes("Images","bmp,png",
					NewFileTypes("All files",NULL,
					NULL ));
					
		File = IOBox("Select file", IOBOX_OPEN, NULL, Filter, true);
		
		if ( !File ) return false;
		
		FreeFileTypes(Filter);
		
	}
	
	Original = LoadImage(File);
	
	SmllBuf = create_bitmap(256,256);
	
	if ( Original->w > Original->h ) {
		wi = 256;
		hi = 256*Original->h/Original->w;
		ScaleReal = Original->w/256.;
	} else {
		hi = 256;
		wi = 256*Original->w/Original->h;
		ScaleReal = Original->h/256;
	}
	
	SmllImg = create_bitmap(wi,hi); // 256x192
	
	clear_to_color(SmllImg, COL_3DFACE);
	stretch_sprite(SmllImg, Original, 0, 0, wi, hi);
	
	Zone.a.y = (SmllBuf->h-SmllImg->h)/2;  
	Zone.b.y = Zone.a.y + SmllImg->h;
	Zone.a.x = (SmllBuf->w-SmllImg->w)/2; 
	Zone.b.x = Zone.a.x + SmllImg->w; 

	CalculateSmall();
	
	
	RectAssign(&r,0, 0, 500, 322);
	w = CreateWindow( &Me, r, "Photo Orientation Correction", WF_FRAME|WF_CAPTION|WF_MINIMIZE|WF_CENTERED );
	InsertWidget(DeskTop, WIDGET(w));
	WIDGET(w)->AppEvHdl = &AppEventHandler;

	WidgetSize(&r,0, 128, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AXP);
	b->Icon = ListKey(Me.Ressource,"XP");
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetSize(&r,0, 150, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AXM);
	b->Icon = ListKey(Me.Ressource,"XM");
	InsertWidget(WIDGET(w), WIDGET(b));
	
	
	WidgetSize(&r,128, 0, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AYP);
	b->Icon = ListKey(Me.Ressource,"YP");
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetSize(&r,150, 0, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AYM);
	b->Icon = ListKey(Me.Ressource,"YM");
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,128, 255+22, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AZP);
	b->Icon = ListKey(Me.Ressource,"ZP");
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetSize(&r,150, 255+22, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_AZM);
	b->Icon = ListKey(Me.Ressource,"ZM");
	InsertWidget(WIDGET(w), WIDGET(b));
	
	
	WidgetSize(&r,21, 0, 20, 20);
	b = CreateButton(&Me, r,NULL,MSG_RESET);
	b->Icon = ListKey(Me.Ressource,"RESET");
	InsertWidget(WIDGET(w), WIDGET(b));


	WidgetSize(&r, 21, 21, 255, 255);
	c = CreateCanvas(&Me, r);
	WIDGET(c)->EventHandler = &ZoneEventHandler;
	WIDGET(c)->Draw = &ImgOrrDraw;
	WIDGET(c)->Flags |= WFForceBuffer;
	InsertWidget(WIDGET(w), WIDGET(c));

	WidgetSize(&r, 21, 300, 49, 20);
	l = CreateLabel(&Me,r,"Focal");
	InsertWidget(WIDGET(w), WIDGET(l));
	
	WidgetSize(&r, 71, 300, 205, 20);
	Sl = CreateSlider( &Me, r, 512 );
	Sl->Value = 128-16;
	InsertWidget(WIDGET(w), WIDGET(Sl));
	Sl->Notify = &SlNotify;
	
	
	WidgetSize(&r,405, 300, 90, 20);
	b = CreateButton(&Me, r,"Apply",MSG_DOWORK);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetSize(&r,310, 300, 90, 20);
	b = CreateButton(&Me, r,"Cancel",WM_CLOSE);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));

	return true;
}

void Close (void)
{
	WidgetDispose(WIDGET(w));
	destroy_bitmap(SmllImg);
	destroy_bitmap(SmllBuf);
	destroy_bitmap(Original);
	free(File);
}

