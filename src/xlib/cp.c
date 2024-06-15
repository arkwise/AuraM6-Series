////////////////////////////////////////////////////////////////////////////////
//
//	Control Panel Deamon
//
//	Copyright (c) 2003 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "menu.h"
#include "fms2.h"

#include "cp.h"

l_uid NeededLibs[] = { "fms2", "" };

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char    AppName[]	= "Control Panel directory deamon";
l_uid	nUID		= "ControlPanel";

PList CPPanels = NULL;
PFileDriver CPDriver;
PCodec 		CPCodec;
PSubCodec 	CPSubCodec;
PDataTypeEx CPDTE;
PDrive CPDrive = NULL;

#define BTNWIDTH 80
#define BTNHEIGHT 22
#define BTNSPACE 5
////////////////////////////////////////////////////////////////////////////////
PIcon CPGetIcons ( PFileInfo f ) {
	
	return DuplicateIcon(CPPANEL(f->Ex)->Icon);
}
////////////////////////////////////////////////////////////////////////////////
l_bool CPRun ( PFileInfo o, int argc, l_text *argv, PApplication Host, PErrorHandler h )
{
	if ( CPPANEL(o->Ex)->Run )
	{
		return CPPANEL(o->Ex)->Run();
	}
	else
	{
		//RunFile2(CPPANEL(o->Ex)->Command,CPPANEL(o->Ex)->Args);
		return DynLdRun(CPPANEL(o->Ex)->Command,CPPANEL(o->Ex)->Args) ? true : false;

	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
PList CPList ( PDrive d, l_text VDir, l_text Dir, l_uchar Attrib )
{
	PList l = NewList();

	if ( CPPanels->Last )
	{
		PListItem a =CPPanels->Last->Next, b=a;

		do
		{
			AddFileInfoToList(l,CPPANEL(a->Data)->Name,VDir,0,FT_VFILE,0,0,0,0,d,CPCodec,a->Data);

			a = a->Next;
		}
		while ( a != b );
	}

	return l;
}
////////////////////////////////////////////////////////////////////////////////
void FreeCPPanel ( PCPPanel o ) {
	if ( o->Name ) free(o->Name);
	if ( o->Command ) free(o->Command);
	if ( o->Args ) free(o->Args);
	if ( o->Icon ) FreeIcon(o->Icon);
	free(o);	
}
////////////////////////////////////////////////////////////////////////////////
PCPPanel CPAddPanel ( l_text Name, l_text Command, l_text Args, p_bitmap Icon16, p_bitmap Icon32, p_bitmap Icon48, l_bool (*Run)(void))
{
	PCPPanel o = malloc(sizeof(TCPPanel));
	if ( !o ) return NULL;
	memset(o, 0, sizeof(TCPPanel));

	o->Name = TextDup(Name);
	o->Command = TextDup(Command);
	
	if ( Icon16 || Icon32 || Icon48 ) {
		o->Icon = NewIcon2(Icon16,Icon32,Icon48);
	}
	o->Run = Run;
	o->Args = TextDup(Args);

	ListAdd(CPPanels, "", o, (void*)&FreeCPPanel);

	return o;
}
////////////////////////////////////////////////////////////////////////////////

l_bool LibMain ( int argc, l_text *argv )
{
	if ( !Me.Ressource ) {
		DebugError("CP.DL ressource is missing. Control Panel have not been loaded.");
		return false;
	}

	CPPanels = NewList();



	CPAddPanel("Keyboard", "xapps/setkeyb.app", NULL, NULL, NULL,0,0);
	CPAddPanel("Mouse", "xapps/setmouse.app", NULL, NULL, NULL,0,0);
	CPAddPanel("Display device", "xapps/setscr.app",NULL, ListKey(Me.Ressource,"SETSCR16"), ListKey(Me.Ressource,"SETSCR32"), 0,0);
	CPAddPanel("Wallpaper", "xapps/setwall.app", NULL, ListKey(Me.Ressource,"SETWALL16"), ListKey(Me.Ressource,"SETWALL32"),0,0);
	CPAddPanel("Theme manager", "xapps/settheme.app", NULL, 0, 0,0,0);

	CPDriver = malloc(sizeof(TFileDriver));
	memset(CPDriver, 0, sizeof(TFileDriver));
	CPDriver->List = &CPList;
	CPDrive = AddDrive("Control Panel", FT_FOLDER, NULL, NULL, CPDriver);

	
	// FMS2/DMS2 Compatiblity WIP
	CPCodec = AddCodec("CP Codec",0xFFFF0001,0,0,0,&CPGetIcons,"&cpld","vfile/controlpanel",0xFFFF0001,0 );
	AddNewSubCodec(CPCodec,0xFFFF0001,0,0,0,0,0,0,0,0,0);
	CPDTE = AddDataTypeEx(0xFFFF0001,"Control panel item",0,0,0,NULL,DTEF_STRICTINTERNAL);
	AddDTAction( CPDTE,"Open",NULL, &CPRun,0x0FFFFFFF );

	
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close (void)
{
	RemoveDrive(CPDrive);
	FreeList(CPPanels);
	free(CPDriver);
}
////////////////////////////////////////////////////////////////////////////////
