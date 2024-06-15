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
	iWidget file
*/
/**
	@defgroup iwidget Ireal Widget
	System to store widgets design in ressources files.
	@{
*/
////////////////////////////////////////////////////////////////////////////////
// iWidget system
////////////////////////////////////////////////////////////////////////////////
PiWidget NewiWidget(PWidgetClass C)
{
	PiWidget o = malloc(sizeof(TiWidget));
	memset(o, 0, sizeof(TiWidget));
	o->Class = C;
	o->Properties = NewList();
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void InsertiWidgetBefore(PiWidget o, PiWidget i, PiWidget b)
{
	PiWidget t = b;
	if (!o->Last)
	{
		i->Next = i;
		i->Prev = i;
		o->Last = i;
	}
	else
	{
		if (b)
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
	i->Parent = o;
}
////////////////////////////////////////////////////////////////////////////////
/**
	Generate widgets from iwidgets
	@param App Application which want to allocate the widget( use &Me in most cases)
	@param iw iWidget source data
	@return Pointer to the widget corresponding to iw. Use iWidgetGetCompiled to get pointers to sub-widgets.
	@see iWidgetGetCompiled
*/
PWidget ComplieiWidget(PApplication App, PiWidget iw)
{
	PWidgetClass wcl = iw->Class;
	PWidget o = iw->Class->StdCreate(App, iw->r);
	o->Name = TextDup(iw->Name);

	while (wcl)
	{

		if (wcl->Properties->Last && wcl->SetRProp)
		{
			PListItem a, b;
			void *v;
			a = b = wcl->Properties->Last->Next;
			do
			{
				if (TextCompare(a->Key, "name") &&
						TextCompare(a->Key, "r.a.x") &&
						TextCompare(a->Key, "r.a.y") &&
						TextCompare(a->Key, "r.b.x") &&
						TextCompare(a->Key, "r.b.y"))
					if (v = ListKey(iw->Properties, a->Key))
					{
						wcl->SetRProp(o, a->Key, v, false);
					}
				a = a->Next;
			} while (a != b);
		}
		wcl = wcl->Parent;
	}
	if (iw->Last)
	{
		PiWidget a, b;
		PWidget n;
		a = b = iw->Last->Next;
		do
		{
			n = ComplieiWidget(App, a);
			WidgetInsert(o, n);
			a = a->Next;
		} while (a != b);
	}
	return o;
}
////////////////////////////////////////////////////////////////////////////////
void FreeiWidget(PiWidget o)
{
	if (o->Last)
	{
		PiWidget a, b, n;
		a = b = o->Last->Next;
		do
		{
			n = a->Next;
			FreeiWidget(a);
			a = n;
		} while (a != b);
	}
	if (o->Properties)
		FreeList(o->Properties);
	if (o->Name)
		free(o->Name);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
void WritePropertiesList(PFile f, PList vl, PWidgetClass wcl)
{
	l_ulong TopPos, BckPos;
	PWidgetProp pt;
	PListItem a, b;
	l_ulong nb = 0;

	FileGetPos(f, &TopPos);
	FileWrite(&nb, 1, sizeof(l_ulong), f);

	if (vl->Last)
	{
		a = b = vl->Last->Next;
		do
		{
			pt = WClassGetProp(wcl, a->Key);
			if (pt)
			{
				FileWriteBinString(f, a->Key);
				FileWrite(&pt->Type->Id, 1, sizeof(l_ulong), f);
				pt->Type->Write(f, a->Data);
				nb++;
			}
			a = a->Next;
		} while (a != b);
	}

	FileGetPos(f, &BckPos);
	FileSetPos(f, &TopPos);
	FileWrite(&nb, 1, sizeof(l_ulong), f);
	FileSetPos(f, &BckPos);
}
////////////////////////////////////////////////////////////////////////////////
l_bool iWidgetSave(PFile file, l_ptr Data, l_ulong *EndOffset, l_ulong *ExtraSave, PErrorHandler h)
{
	l_ulong Nb = 0;

	FileWrite(&IWIDGET(Data)->Class->Id, 1, sizeof(l_ulong), file);

	FileWrite(&IWIDGET(Data)->r.a.x, 1, sizeof(l_int), file);
	FileWrite(&IWIDGET(Data)->r.a.y, 1, sizeof(l_int), file);
	FileWrite(&IWIDGET(Data)->r.b.x, 1, sizeof(l_int), file);
	FileWrite(&IWIDGET(Data)->r.b.y, 1, sizeof(l_int), file);

	FileWriteBinString(file, IWIDGET(Data)->Name);

	WritePropertiesList(file, IWIDGET(Data)->Properties, IWIDGET(Data)->Class);

	Nb = 0;

	if (IWIDGET(Data)->Last)
	{
		PiWidget a, b;
		a = b = IWIDGET(Data)->Last->Next;
		do
		{
			Nb++;
			a = a->Next;
		} while (a != b);
		FileWrite(&Nb, 1, sizeof(l_ulong), file);
		do
		{
			iWidgetSave(file, a, NULL, ExtraSave, h);
			a = a->Next;
		} while (a != b);
	}
	else
		FileWrite(&Nb, 1, sizeof(l_ulong), file);

	if (EndOffset)
		FileGetPos(file, EndOffset);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void LoadPropertiesList(PFile f, PList vl, PWidgetClass wcl)
{
	l_ulong TopPos, BckPos;
	l_ulong nb, type;
	l_text name;
	PListItem c;
	PWidgetProp pt;
	void *v;
	FileGetPos(f, &TopPos);

	FileRead(&nb, 1, sizeof(l_ulong), f);
	while (nb)
	{
		name = FileReadBinString(f);
		pt = WClassGetProp(wcl, name);
		if (!pt)
			return;
		FileRead(&type, 1, sizeof(l_ulong), f);
		if (pt->Type->Id == type)
		{
			v = pt->Type->Read(f);
			ListAdd(vl, name, v, pt->Type->Free);
		}
		else
		{
			DebugMessage("iWidget : Error loading properties !");
			return;
		}
		free(name);
		nb--;
	}
	FileGetPos(f, &BckPos);
}
////////////////////////////////////////////////////////////////////////////////
l_bool iWidgetLoad(PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *ExtraLoad, PErrorHandler h)
{
	l_ulong Id, Nb;
	*Data = NewiWidget(NULL);

	FileRead(&Id, 1, sizeof(l_ulong), file);

	IWIDGET(*Data)->Class = GetWidgetClass(Id);

	if (!IWIDGET(*Data)->Class)
	{
		FreeiWidget(IWIDGET(*Data));
		return false;
	}

	FileRead(&IWIDGET(*Data)->r.a.x, 1, sizeof(l_int), file);
	FileRead(&IWIDGET(*Data)->r.a.y, 1, sizeof(l_int), file);
	FileRead(&IWIDGET(*Data)->r.b.x, 1, sizeof(l_int), file);
	FileRead(&IWIDGET(*Data)->r.b.y, 1, sizeof(l_int), file);

	IWIDGET(*Data)->Name = FileReadBinString(file);

	LoadPropertiesList(file, IWIDGET(*Data)->Properties, IWIDGET(*Data)->Class);

	FileRead(&Nb, 1, sizeof(l_ulong), file);

	if (Nb)
	{
		PiWidget n;
		while (Nb)
		{
			if (!iWidgetLoad(file, (void *)&n, NULL, ExtraLoad, h))
				return false;
			InsertiWidgetBefore(IWIDGET(*Data), n, IWIDGET(*Data)->Last ? IWIDGET(*Data)->Last->Next : NULL);
			Nb--;
		}
	}
	if (EndOffset)
		FileGetPos(file, EndOffset);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool WidgetSetRProp(PWidget o, l_text Name, void *Value, l_bool Redraw)
{
	if (!TextCompare(Name, "wflags"))
	{
		o->Flags = (o->Flags & ~WF_SCRIPTFLAGS) | (*((l_ulong *)Value));
		if (Redraw)
			WidgetDraw(WIDGET(o), NULL);
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void *WidgetGetRProp(PWidget o, l_text Name)
{
	if (!TextCompare(Name, "wflags"))
		return DuplicateInt(&o->Flags);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
	@}
	@}
*/
////////////////////////////////////////////////////////////////////////////////
PCodec OIW;
PWidgetClass StdWidgetClass;
////////////////////////////////////////////////////////////////////////////////
void iWidgetInstall(void)
{

	// iWidget DMS interface

	RegisterDataType(DTYPE_IWIDGET, (void *)&FreeiWidget, NULL);

	OIW = AddCodec("oZone irreal widget (iWidget)", ULONG_ID('i', 'W', 'd', 'g'), 0, 0, 0, NULL, "oiw", "application/ozone-iwidget", DTYPE_IWIDGET, 0);
	AddSubCodec(OIW, GenericBinary);
	AddNewSubCodec(OIW, DTYPE_IWIDGET, 0, 0, 0, &iWidgetSave, &iWidgetLoad, 0, 0, 0, 0);

	// Standart Widget support for iWidget

	StdWidgetClass = CreateWidgetClass(0, NULL, 0);
	StdWidgetClass->StdCreate = &CreateWidget;
	StdWidgetClass->SetRProp = &WidgetSetRProp;
	StdWidgetClass->GetRProp = &WidgetGetRProp;

	WClassRegisterProp(StdWidgetClass, PTYPE_STRING, "name");
	WClassRegisterProp(StdWidgetClass, PTYPE_INT, "r.a.x");
	WClassRegisterProp(StdWidgetClass, PTYPE_INT, "r.a.y");
	WClassRegisterProp(StdWidgetClass, PTYPE_INT, "r.b.x");
	WClassRegisterProp(StdWidgetClass, PTYPE_INT, "r.b.y");
	WClassRegisterProp(StdWidgetClass, PTYPE_FLAGS, "wflags");

	WClassResgisterFlag(StdWidgetClass, "wflags", WF_AUTORESIZE, "WF_AUTORESIZE", NULL);
	WClassResgisterFlag(StdWidgetClass, "wflags", WF_FIXEDWIDTH, "WF_FIXEDWIDTH", NULL);
	WClassResgisterFlag(StdWidgetClass, "wflags", WF_FIXEDHEIGHT, "WF_FIXEDHEIGHT", NULL);
	WClassResgisterFlag(StdWidgetClass, "wflags", WF_MAGNETRIGHT, "WF_MAGNETRIGHT", NULL);
	WClassResgisterFlag(StdWidgetClass, "wflags", WF_MAGNETBOTTOM, "WF_MAGNETBOTTOM", NULL);
}
////////////////////////////////////////////////////////////////////////////////
void iWidgetUnInstall(void)
{
}
////////////////////////////////////////////////////////////////////////////////
