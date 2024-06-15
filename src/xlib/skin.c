/////////////////////////////////////////////////////////////////////////////////
//
//	Skin Core File 
//
//	Aura Milestone 4 
//
//	(c) Copyright 2013 Finn Technologies and Chase Finn. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "string.h"

l_ulong AppVersion      = ULONG_ID(1,5,2,0);
char    AppName[]       = "Skin System";
l_uid   nUID            = "skin";

l_text SKIN_DIR  = "./SYSTEM/SKINS/";


/**
 * The skin items tree
 */
PSkin   SkinItems = 0;

/**
 * Global exported variable determining the use of skins
 */
l_bool  UseSkins = true;

/**
 * Color support
 */
l_color _SysColors[25];
l_color *SysColors = _SysColors;

l_color GetColor( TSColor c ) {
        if ( c.idx == 0xFF )
                return makecol(c.r,c.g,c.b);
        else
                return SysColors[c.idx];
}

p_bitmap ssi =0;


PSkin GetSkinItem (l_text szName)
{
        PSkin p = SkinItems;

        while (p)
        {
                if (!stricmp (szName, p->Name)) return p;

                p = p->Next;
        }

        return 0;
}

void DrawSkin (p_bitmap out, PSkin si, l_int x1, l_int y1, l_int x2, l_int y2)
{
        if ( !si )
        {
                DebugError("No skin");
                return;
        }

        masked_blit(si->Skin, out, 0, 0, x1, y1, si->Left, si->Top);
        masked_blit(si->Skin, out, 0, si->Skin->h-si->Bottom, x1, y2-si->Bottom+1, si->Left, si->Bottom);
        masked_blit(si->Skin, out, si->Skin->w-si->Right, 0, x2-si->Right+1, y1, si->Right, si->Top);
        masked_blit(si->Skin, out, si->Skin->w-si->Right, si->Skin->h-si->Bottom, x2-si->Right+1, y2-si->Bottom+1, si->Right, si->Bottom);

        masked_stretch_blit(si->Skin, out, 0, si->Top, si->Left, si->Skin->h-si->Top-si->Bottom, x1, y1+si->Top, si->Left, (y2-y1+1)-si->Top-si->Bottom);
        masked_stretch_blit(si->Skin, out, si->Skin->w-si->Right, si->Top, si->Right, si->Skin->h-si->Top-si->Bottom, x2-si->Right+1, y1+si->Top, si->Right, (y2-y1+1)-si->Top-si->Bottom);

        masked_stretch_blit(si->Skin, out, si->Left, 0, si->Skin->w-si->Left-si->Right, si->Top, x1+si->Left, y1, (x2-x1+1)-si->Left-si->Right, si->Top);
        masked_stretch_blit(si->Skin, out, si->Left, si->Skin->h-si->Bottom, si->Skin->w-si->Left-si->Right, si->Bottom, x1+si->Left, y2-si->Bottom+1, (x2-x1+1)-si->Left-si->Right, si->Bottom);

        masked_stretch_blit(si->Skin, out, si->Left, si->Top, si->Skin->w-si->Left-si->Right, si->Skin->h-si->Top-si->Bottom, x1+si->Left, y1+si->Top, (x2-x1)-si->Left-si->Right+1, (y2-y1)-si->Top-si->Bottom+1);

}

void DrawSkinCustom (p_bitmap out, p_bitmap ssi,  l_int x1, l_int y1, l_int x2, l_int y2)
{
        if ( !ssi )
        {
                DebugError("Custom skin file was called, but was unable to be drawn");
                return;
        }

        masked_blit(ssi, out, 0, 0, x1, y1, 0, 0);
        masked_blit(ssi, out, 0, ssi->h-0, x1, y2-0+1, 0, 0);
        masked_blit(ssi, out, ssi->w-0, 0, x2-0+1, y1, 0, 0);
        masked_blit(ssi, out, ssi->w-0, ssi->h-0, x2-0+1, y2-0+1, 0, 0);

        masked_stretch_blit(ssi, out, 0, 0, 0, ssi->h-0-0, x1, y1+0, 0, (y2-y1+1)-0-0);
        masked_stretch_blit(ssi, out, ssi->w-0, 0, 0, ssi->h-0-0, x2-0+1, y1+0, 0, (y2-y1+1)-0-0);

        masked_stretch_blit(ssi, out, 0, 0, ssi->w-0-0, 0, x1+0, y1, (x2-x1+1)-0-0, 0);
        masked_stretch_blit(ssi, out, 0, ssi->h-0, ssi->w-0-0, 0, x1+0, y2-0+1, (x2-x1+1)-0-0, 0);

        masked_stretch_blit(ssi, out, 0, 0, ssi->w-0-0, ssi->h-0-0, x1+0, y1+0, (x2-x1)-0-0+1, (y2-y1)-0-0+1);      
		
}

void DrawTransSkin (p_bitmap out, PSkin si, l_int x1, l_int y1, l_int x2, l_int y2, l_int nb, l_int max)
{
        if ( !si )
        {
                DebugError("No skin");
                return;
        }
		
	BITMAP *sprite_buffer = create_bitmap(x2-x1+1,y2-y1);
	clear_to_color(sprite_buffer,makecol(255,0,255));
	masked_stretch_blit(si->Skin, sprite_buffer, 0,  0, 10, si->Skin->h, 0, 0, 10, y2-y1);
	masked_stretch_blit(si->Skin, sprite_buffer, 10, 0, si->Skin->w-20, si->Skin->h, 10, 0, x2-x1-19, y2-y1);
	masked_stretch_blit(si->Skin, sprite_buffer, si->Skin->w-10, 0, 10, si->Skin->h, x2-x1-9, 0, 10, y2-y1);

	
    	set_trans_blender(0, 0, 0, nb*255/max);

    	draw_trans_sprite(out, sprite_buffer, x1, y1);

    	solid_mode();

        destroy_bitmap (sprite_buffer);

}
p_bitmap  ResImage = 0;

void DrawTransSkinCustom (p_bitmap out, p_bitmap ssi, l_int x1, l_int y1, l_int x2, l_int y2, l_int nb, l_int max)
{
        if ( !ssi )
        {
                DebugError("Custom skin file was called, but was unable to be drawn");
                return;
        }
		
	BITMAP *sprite_buffer = create_bitmap(x2-x1+1,y2-y1);
	clear_to_color(sprite_buffer,makecol(255,0,255));
	masked_stretch_blit(ssi, sprite_buffer, 0,  0, 10, ssi->h, 0, 0, 10, y2-y1);
	masked_stretch_blit(ssi, sprite_buffer, 10, 0, ssi->w-20, ssi->h, 10, 0, x2-x1-19, y2-y1);
	masked_stretch_blit(ssi, sprite_buffer, ssi->w-10, 0, 10, ssi->h, x2-x1-9, 0, 10, y2-y1);

	
    	set_trans_blender(0, 0, 0, nb*255/max);

    	draw_trans_sprite(out, sprite_buffer, x1, y1);

    	solid_mode();

        destroy_bitmap (sprite_buffer);

}



/*l_bool ReloadSkin (l_text File)
{
        PSkin p = SkinItems;
        l_text szFile = malloc(strlen(SKIN_DIR) + strlen(File) + 1);
        l_int Items = 0;

        strcpy(szFile, SKIN_DIR);
        strcat(szFile, File);

        DebugMessage(szFile);

        do
        {
                if (INISectionExists (szFile, p->Name))
                {
                        l_text szPath = malloc(1024);

                        strcpy(szPath, SKIN_DIR);
                        strcat(szPath, INIGetNew(szFile, p->Name, "bmp", NULL));
                        if (p->Skin) destroy_bitmap (p->Skin);
                        p->Skin = LoadImage(szPath);

                        p->Left = INIGetInteger(szFile, p->Name, "left", 3);
                        p->Right = INIGetInteger(szFile, p->Name, "right", 3);
                        p->Top = INIGetInteger(szFile, p->Name, "top", 3);
                        p->Bottom = INIGetInteger(szFile, p->Name, "bottom", 3);

                        Items++;

                        free (szPath);
                }
                else DebugMessage ("Section %s doesnt exist", p->Name);

                p = p->Next;
        }
        while (p);

        DebugMessage("Skin system reloaded a total of '%i' skin items", Items);

        WidgetDrawAll(DeskTop);

        return 0;
} */

l_bool LoadColors (l_text szFile)
{
        _SysColors[0]  = INIGetColor(szFile, "colors", "3dtext", _SysColors[0]);
        _SysColors[1]  = INIGetColor(szFile, "colors", "3dface", _SysColors[1]);
        _SysColors[2]  = INIGetColor(szFile, "colors", "3dlight", _SysColors[2]);
        _SysColors[3]  = INIGetColor(szFile, "colors", "3ddark", _SysColors[3]);
        _SysColors[4]  = INIGetColor(szFile, "colors", "3dover", _SysColors[4]);

        /*_SysColors[5]  = INIGetColor(szFile, "colors", "boxface", makecol(0,0,0));
        _SysColors[6]  = INIGetColor(szFile, "colors", "boxtext", makecol(0,0,0));
        _SysColors[7]  = INIGetColor(szFile, "colors", "boxlight", makecol(0,0,0));
        _SysColors[8]  = INIGetColor(szFile, "colors", "boxdark", makecol(0,0,0));
        _SysColors[9]  = INIGetColor(szFile, "colors", "boxsel", makecol(0,0,0));

        _SysColors[10] = INIGetColor(szFile, "colors", "wtact", makecol(0,0,0));
        _SysColors[11] = INIGetColor(szFile, "colors", "wtacttext", makecol(0,0,0));

        _SysColors[12] = INIGetColor(szFile, "colors", "wtpas", makecol(0,0,0));
        _SysColors[13] = INIGetColor(szFile, "colors", "wtpas", makecol(0,0,0));*/
}

l_bool LoadSkin (l_text File)
{
        FILE *f = 0;
        l_text szFile = malloc(strlen(SKIN_DIR) + strlen(File) + 1);
        l_int Items = 0;

        strcpy(szFile, SKIN_DIR);
        strcat(szFile, File);

        DebugMessage("Trying to load skin file - %s", szFile);

        f = fopen(szFile, "r");

        if (!f)
        {
                DebugWarning ("Skin - %s - not found! Using standard skin", szFile);
  free(szFile);
                return false;
        }

        LoadColors (szFile);

        while (!feof(f))
        {
                l_char c[256];
                fgets(c, 256, f);
                if (!c[0]) continue;
                if (c[strlen(c)-1] < 32) c[strlen(c)-1] = 0;
                if (c[0] == '[' && (c[strlen(c)-1] == ']' || c[strlen(c)-2] == ']') && stricmp(c+1, "Information]"))
                {
                        l_text szPath           = malloc(1024);
                        l_text szTemp           = malloc(1024);
                        l_bool Platform         = false;
                        PSkin p                         = (PSkin)malloc(sizeof(TSkin));
                        memset(p, 0, sizeof(TSkin));

                        /**
                         * Fuck this dirty hack :GRR:
                         */
                        if (c[strlen(c)-2] == ']') Platform = true;

                        p->Name = (char*)strdup(c+1);

                        if (Platform) p->Name[strlen(c+1)-2] = '\0';
                        else p->Name[strlen(c+1)-1] = '\0';

                /**
                        *       Load image  (bug fixed 19.05.2006)
                        */

                        strcpy(szPath, SKIN_DIR);
DebugMessage("Skin - Load Image - Path: %s , File: %s", szPath, INIGetNew(szFile, p->Name, "bmp", NULL));
                        strcpy(szTemp, INIGetNew(szFile, p->Name, "bmp", NULL));
                if (strlen(szTemp) > 1)
   {
   strcat(szPath, szTemp);
                        p->Skin = LoadImage(szPath);

                        /**
                        *       Load image offsets
                        */
                        p->Left = INIGetInteger(szFile, p->Name, "left", 3);
                        p->Right = INIGetInteger(szFile, p->Name, "right", 3);
                        p->Top = INIGetInteger(szFile, p->Name, "top", 3);
                        p->Bottom = INIGetInteger(szFile, p->Name, "bottom", 3);

                        /**
                        *       Load skin items font and color
                        */
                        strcpy(szPath, SKIN_DIR);
                        strcat(szPath, INIGetNew(szFile, p->Name, "font", ""));
                        p->Font = FontLoad(szPath);
                        p->FontColor = INIGetColor(szFile, p->Name, "color", makecol(0,0,0));
   }
                        Items++;

                        p->Next = NULL;

                        free(szPath);
                        free(szTemp);

                        if (!SkinItems) SkinItems = p;
                        else
                        {
                                PSkin w = SkinItems;

                                if (w->Next)
                                {
                                        do w = w->Next;
                                        while (w->Next);
                                }
                                w->Next = p;
                        }
                }
        } 

        DebugMessage("Skin system loaded a total of '%i' skin items", Items);

        fclose(f);
        free(szFile);

        return true;
}

void SkinSystemInstall (void)
{
        _SysColors[0]  = makecol(  0,  0,  0);
        _SysColors[1]  = makecol(246,246,246);
        _SysColors[2]  = makecol(255,255,255);
        _SysColors[3]  = makecol(128,128,128);
        _SysColors[4]  = makecol(255,255,255);

        _SysColors[5]  = makecol(255,255,255);
        _SysColors[6]  = makecol(  0,  0,  0);
        _SysColors[7]  = makecol(195,195,195);
        _SysColors[8]  = makecol(195,195,195);
        _SysColors[9]  = makecol( 64, 64,255);

        _SysColors[10] = makecol(  0,  0,255);
        _SysColors[11] = makecol(255,255,255);

        _SysColors[12] = makecol(192,192,255);
        _SysColors[13] = makecol(  0,  0,  0);

SKIN_DIR = KeyGetText("/USER/GUI/SKINPATH", "./SYSTEM/SKINS/");

if (strlen(KeyGetText("/USER/GUI/SKIN", "")) == 0)
   {
   UseSkins = false;
   DebugMessage("User doesn't want to use skins. No skin is loaded.");
   return;
   }

        UseSkins = LoadSkin (KeyGetText("/USER/GUI/SKIN", "ozone.ini"));
}

l_bool LibMain ( int argc, l_text *argv )
{
        SkinSystemInstall();

        APPEXPORT(GetSkinItem);
        APPEXPORT(DrawSkin);
       APPEXPORT(DrawSkinCustom);
        APPEXPORT(DrawTransSkin);
        APPEXPORT(DrawTransSkinCustom);
        APPEXPORT(SysColors);
        APPEXPORT(_SysColors);
        APPEXPORT(UseSkins);
//      APPEXPORT(ReloadSkin);

        APPEXPORT(GetColor);

        return true;
}

void Close (void)
{
        PSkin p = SkinItems;
        PSkin tmp;
        l_int Items = 0;

        SkinItems = NULL;

        while ( p )
        {
                tmp = p->Next;

                if ( p->Name ) free(p->Name);
                if ( p->Skin ) destroy_bitmap(p->Skin);

                free(p);

                p = tmp;
                Items++;
        }

        DebugMessage("Skin system destroyed a total of '%i' skin items", Items);
}
