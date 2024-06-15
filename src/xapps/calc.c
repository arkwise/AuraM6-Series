////////////////////////////////////////////////////////////////////////////////
//
//	Calculator 0.01
//
//	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"

l_ulong AppVersion	= ULONG_ID(0,0,0,1);
char AppName[]		= "Calculator";
l_uid	nUID		= "app:regedit";
l_uid NeededLibs[]	= { "label","button","window", "" };

#define M_PI			3.14159265358979323846

/**
*	Button messages
*/
#define  MSG_0			100001
#define  MSG_1			100002
#define  MSG_2			100003
#define  MSG_3			100004
#define  MSG_4			100005
#define  MSG_5			100006
#define  MSG_6			100007
#define  MSG_7			100008
#define  MSG_8			100009
#define  MSG_9			100010
#define  MSG_MINUS		100011
#define  MSG_PLUS		100012
#define  MSG_DIVIDE		100013
#define  MSG_MULTIPLY	100014
#define  MSG_PERCENT	100015
#define  MSG_SQRT		100016
#define  MSG_EQUALS		100017
#define  MSG_C			100018
#define  MSG_PLUSMINUS	100019
#define  MSG_DOT		100020
#define  MSG_PI			100021
#define  MSG_SQR		100022

/**
*	Global variables
*/
PLabel Display			= 0;
l_bool DisplayValue		= false;
l_bool MainDot			= false;
l_double MainSum		= 0;
l_ulong LastOperation	= 0;
l_ulong MainOperation	= 0;

void  MakeOperation ( l_ulong o )
{
	l_text Text		= NULL;
	l_bool Zero		= (l_bool)(Display->Caption && !strcmp(Display->Caption, "0"));
	l_bool Error	= false;

	if ( DisplayValue || Zero )
	{
		if ( MainOperation == 0 ) MainSum = atof(Display->Caption);
		if ( MainOperation == MSG_PLUS ) MainSum += atof(Display->Caption);
		if ( MainOperation == MSG_MINUS ) MainSum -= atof(Display->Caption);
		if ( MainOperation == MSG_MULTIPLY ) MainSum *= atof(Display->Caption);
		if ( MainOperation == MSG_PERCENT ) MainSum = MainSum*(atof(Display->Caption)/100.0);
		if ( MainOperation == MSG_DIVIDE )
		{
			if ( !Zero ) MainSum /= atof(Display->Caption);
			else
			{
				Error = true;
				Text = (char*)strdup("ERROR - Dividing by ZERO");
			}
		}

		if ( !Text )
		{
			Text = TextArgs("%G", MainSum);
			//Text = ignore_zeros(text);
		}

		LabelSetText(Display, Text);
		LastOperation = o;
		DisplayValue = false;
		MainDot = false;
	}

	if ( o != MSG_EQUALS && !Error ) MainOperation = o;
	else MainOperation = 0;

	free(Text);
}

l_bool AppEventHandler ( PWidget o, PEvent Event )
{
	if ( Event->Type == EV_MESSAGE )
	{
		switch ( Event->Message )
		{
			case WM_CLOSE:
			{
				WidgetDispose(WIDGET(o));
				CloseApp(&Me);
				return true;
			}
			break;

			case WM_ABOUT:
			{
				MessageBox(&Me, "About Calculator", "Calculator 1.0\nCalculator for oZone\n\nCopyright (c) 2003 Lukas Lipka. All rights reserved.", MBB_OK);
				return true;
			}
			break;

			case MSG_1:
			{
				l_text Text = !DisplayValue ? (char*)strdup("1") : TextArgs("%s%c", Display->Caption, '1');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_2:
			{
				l_text Text = !DisplayValue ? (char*)strdup("2") : TextArgs("%s%c", Display->Caption, '2');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_3:
			{
				l_text Text = !DisplayValue ? (char*)strdup("3") : TextArgs("%s%c", Display->Caption, '3');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_4:
			{
				l_text Text = !DisplayValue ? (char*)strdup("4") : TextArgs("%s%c", Display->Caption, '4');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_5:
			{
				l_text Text = !DisplayValue ? (char*)strdup("5") : TextArgs("%s%c", Display->Caption, '5');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_6:
			{
				l_text Text = !DisplayValue ? (char*)strdup("6") : TextArgs("%s%c", Display->Caption, '6');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_7:
			{
				l_text Text = !DisplayValue ? (char*)strdup("7") : TextArgs("%s%c", Display->Caption, '7');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_8:
			{
				l_text Text = !DisplayValue ? (char*)strdup("8") : TextArgs("%s%c", Display->Caption, '8');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_9:
			{
				l_text Text = !DisplayValue ? (char*)strdup("9") : TextArgs("%s%c", Display->Caption, '9');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_0:
			{
				l_text Text = !DisplayValue ? (char*)strdup("0") : TextArgs("%s%c", Display->Caption, '0');
				LabelSetText(Display, Text);
				free(Text);
				DisplayValue = true;
			}
			break;

			case MSG_C:
			{
				LabelSetText(Display, "0");
				DisplayValue = false;
				MainSum = 0;
				LastOperation = 0;
				MainDot = false;
			}
			break;

			case MSG_PLUS:
			{
				MakeOperation(MSG_PLUS);
			}
			break;

			case MSG_MINUS:
			{
				MakeOperation(MSG_MINUS);
			}
			break;

			case MSG_MULTIPLY:
			{
				MakeOperation(MSG_MULTIPLY);
			}
			break;

			case MSG_DIVIDE:
			{
				MakeOperation(MSG_DIVIDE);
			}
			break;

			case MSG_EQUALS:
			{
				MakeOperation(MSG_EQUALS);
			}
			break;

			case MSG_PERCENT:
			{
				MakeOperation(MSG_PERCENT);
			}
			break;

			case MSG_PI:
			{
				l_text Text = TextArgs("%f", M_PI);
				LabelSetText(Display, Text);
				free(Text);

				MainDot = false;
				DisplayValue = true;
			}
			break;

			case MSG_DOT:
			{
				if (!MainDot)
				{
					l_text Text = !DisplayValue ? (char*)strdup("0.") : TextArgs("%s%c", Display->Caption, '.');
					LabelSetText(Display, Text);
					free(Text);

					DisplayValue = true;
					MainDot = true;
				}
			}

			case MSG_SQRT:
			{

			}
			break;

			case MSG_SQR:
			{

			}
			break;
		}
	}

	return false;
}

l_int Main ( int argc, l_text *argv )
{
	PWindow w	= 0;
	PButton b	= 0;

	TRect r;

	RectAssign(&r,0, 0, 285, 190);
	w = CreateWindow(&Me, r, "Calculator", WF_CAPTION|WF_FRAME|WF_MINIMIZE|WF_CENTERED);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));
	WidgetPreCenter(WIDGET(w));

	WidgetSize(&r,10, 5, 265, 20);
	Display = CreateLabel(&Me, r, "0");
	WIDGET(Display)->BackgroundColor = makecol(250,250,250);
	InsertWidget(WIDGET(w), WIDGET(Display));

	WidgetSize(&r,10, 45, 40, 30);
	b = CreateButton(&Me, r, "7", MSG_7);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,55, 45, 40, 30);
	b = CreateButton(&Me, r, "8", MSG_8);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,100, 45, 40, 30);
	b = CreateButton(&Me, r, "9", MSG_9);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,10, 80, 40, 30);
	b = CreateButton(&Me, r, "4", MSG_4);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,55, 80, 40, 30);
	b = CreateButton(&Me, r, "5", MSG_5);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,100, 80, 40, 30);
	b = CreateButton(&Me, r, "6", MSG_6);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,10, 115, 40, 30);
	b = CreateButton(&Me, r, "1", MSG_1);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,55, 115, 40, 30);
	b = CreateButton(&Me, r, "2", MSG_2);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,100, 115, 40, 30);
	b = CreateButton(&Me, r, "3", MSG_3);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,10, 150, 40, 30);
	b = CreateButton(&Me, r, "0", MSG_0);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,55, 150, 40, 30);
	b = CreateButton(&Me, r, "+/-", MSG_PLUSMINUS);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,100, 150, 40, 30);
	b = CreateButton(&Me, r, ".", MSG_DOT);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,145, 45, 40, 30);
	b = CreateButton(&Me, r, "+", MSG_PLUS);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,145, 80, 40, 30);
	b = CreateButton(&Me, r, "-", MSG_MINUS);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,145, 115, 40, 30);
	b = CreateButton(&Me, r, "*", MSG_MULTIPLY);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,145, 150, 40, 30);
	b = CreateButton(&Me, r, "/", MSG_DIVIDE);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,190, 45, 85, 30);
	b = CreateButton(&Me, r, "C", MSG_C);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,190, 80, 40, 30);
	b = CreateButton(&Me, r, "SQRT", MSG_SQRT);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,190, 115, 40, 30);
	b = CreateButton(&Me, r, "PI", MSG_PI);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,235, 80, 40, 30);
	b = CreateButton(&Me, r, "SQR", MSG_SQR);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,235, 115, 40, 30);
	b = CreateButton(&Me, r, "%", MSG_PERCENT);
	InsertWidget(WIDGET(w), WIDGET(b));

	WidgetSize(&r,190, 150, 85, 30);
	b = CreateButton(&Me, r, "=", MSG_EQUALS);
	InsertWidget(WIDGET(w), WIDGET(b));
	
	WidgetDrawAll(WIDGET(w));
	return true;
}

void Close (void)
{

}
