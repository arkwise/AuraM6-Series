#ifndef _SKIN_H_INCLUDED_
#define _SKIN_H_INCLUDED_

typedef struct TSkin	*PSkin;
typedef struct TSkin
{
	l_text		Name;
	l_short		Left, Top, Right, Bottom;
	p_bitmap	Skin;

	/**
	*	Font
	*/
	FONT*		Font;
	l_color		FontColor;

	PSkin		Next;
	PSkin		Prev;
} TSkin;

extern PSkin SkinItems;

PSkin GetSkinItem(l_text szName);
void DrawSkin(p_bitmap out, PSkin si, l_int x1, l_int y1, l_int x2, l_int y2);
void DrawTransSkin(p_bitmap out, PSkin si, l_int x1, l_int y1, l_int x2, l_int y2, l_int nb, l_int max);

void DrawSkinCustom (p_bitmap out, p_bitmap ssi, l_int x1, l_int y1, l_int x2, l_int y2);
void DrawTransSkinCustom (p_bitmap out, p_bitmap ssi, l_int x1, l_int y1, l_int x2, l_int y2, l_int nb, l_int max);

void  SkinSystemInstall();


l_color GetColor( TSColor c );

extern l_color *SysColors;

#define COL_3DTEXT 				SysColors[0]
#define COL_3DFACE 				SysColors[1]
#define COL_3DLIGHT				SysColors[2]
#define COL_3DDARK				SysColors[3]
#define COL_3DOVER				SysColors[4]

#define COL_BOXFACE				SysColors[5]
#define COL_BOXTEXT				SysColors[6]
#define COL_BOXLIGHT			SysColors[7]
#define COL_BOXDARK				SysColors[8]
#define COL_BOXSEL				SysColors[9]

#define COL_WTACT				SysColors[10]
#define COL_WTACTTEXT			SysColors[11]

#define COL_WTPAS				SysColors[12]
#define COL_WTPASTEXT			SysColors[13]

extern l_bool UseSkins;


#endif /* _SKIN_H_INCLUDED_ */
