////////////////////////////////////////////////////////////////////////////////
//
//	Orchid Web Browser with Watt TCP, Curl, and libxml2 HTML Parsing
//
//	Note: This code is a simplified example and may not provide a complete
//	web browsing experience.
//
//	Make sure to link with wattcp, curl, and libxml2 libraries, and add the
//	necessary include directories and compiler flags.
//
//	Note: This code assumes a single-threaded environment. If you want to
//	integrate with a multi-threaded environment, additional considerations
//	and modifications may be required.
//
////////////////////////////////////////////////////////////////////////////////


// the flags for compiling are : (to be added)



#include "kernel.h"
#include "widget.h"
#include "menu.h"
#include "button.h"
#include "window.h"
#include "tbox.h"
#include "iodlg.h"
#include "toolbar.h"
#include <stdio.h>
#include <curl/curl.h>
#include <wattcp.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

char AppName[]		= "Orchid";
l_uid	nUID = "app:Orchid";
l_uid NeededLibs[] = { "menu", "button", "iodlg", "window", "textbox", "toolbar", "" };


#define  MSG_FILENEW	0xF0020001
#define  MSG_FILEOPEN	0xF0020002
#define  MSG_FILESAVE	0xF0020003
#define  MSG_FILESAVEAS	0xF0020004
#define  MSG_GO	0xF0020005


PTextbox tb = 0;
PButton b = 0;
PTextbox addtb = 0;
PWindow w = 0;
PToolbar bar = 0;
PFileTypes Filter = 0;
l_text OpenedFile = 0;


l_bool AppEventHandler(PWidget o, PEvent Event)
{
	if (Event->Type == EV_MESSAGE)
	{
		switch (Event->Message)
		{
		case WM_CLOSE:
		{
			CloseApp(&Me);
			return true;
		}
		break;

		case MSG_FILENEW:
		{
			TextBoxSetTextEx(tb, "Orchid 0.4");
			WindowSetCaptionEx(w, "Orchid Browser");

			return true;
		}
		break;

		case MSG_GO:
		{
			l_text url = TextBoxGetText(addtb);
			if (url) {
				// Create a CURL handle
				CURL* curl = curl_easy_init();

				if (curl) {
					// Set the URL to fetch
					curl_easy_setopt(curl, CURLOPT_URL, url);

					// Set the callback function for the response data
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TextBoxSetTextEx);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, tb);

					// Perform the request
					curl_easy_perform(curl);

					// Cleanup
					curl_easy_cleanup(curl);
				}
			}

			// Parse the HTML content using libxml2
			l_text htmlContent = TextBoxGetText(tb);
			if (htmlContent) {
				// Parse the HTML content
				xmlDocPtr doc = htmlReadMemory(htmlContent, strlen(htmlContent), NULL, NULL, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
				if (doc) {
					xmlNodePtr root = xmlDocGetRootElement(doc);
					if (root) {
						// Traverse the HTML tree and process the nodes
						processHtmlNode(root);
					}

					// Free the parsed document
					xmlFreeDoc(doc);
				}
			}

			return true;
		}
		break;
		case MSG_FILEOPEN:
		{
			l_text tmp = OpenedFile;

			OpenedFile = IOBox("Open text file (UTF-8)", IOBOX_OPEN, NULL, Filter, true);

			if (OpenedFile)
			{
				l_text txt = LoadData2(OpenedFile, TYPE_TEXT);

				if (txt)
				{
					TextBoxSetTextEx(addtb, OpenedFile);
					TextBoxSetTextEx(tb, txt);
					free(txt);

				}

				free(tmp);
			}
			return true;
		}
		break;
		case MSG_FILESAVE:
		case MSG_FILESAVEAS:
		{
			l_text File;

			if (Event->Message == MSG_FILESAVEAS)
			{
				free(OpenedFile);
				OpenedFile = IOBox("Save text file (UTF-8)", IOBOX_SAVE, NULL, Filter, false);
			}

			if (OpenedFile)
			{
				l_text txt = TextDup(tb->Text);

				if (SaveData2(OpenedFile, txt, TYPE_TEXT))
				{
					WindowSetCaption(w, "Type Writer");
				}

				free(txt);
			}
			return true;
		}
		break;

		case WM_ABOUT:
		{
			MessageBox(&Me, "About Orchid", "Orchid 0.4\nThe Aura Web Flower\n\nCopyright (c) 2023 Doscore. All rights reserved.", MBB_OK);
			return true;
		}
		break;
		}
	}

	return false;
}

void GenerateEditSubMenu(PMenuItem o, void* Args) {
	o->SubMenu = NewMenu(
		NewMenuItem("Copy", NULL, MSG_COPY, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) ? 0 : MI_DISABLE, NULL,
			NewMenuItem("Cut", NULL, MSG_CUT, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL,
				NewMenuItem("Paste", NULL, MSG_PASTE, ((TEXTBOX(tb)->Flags & TBF_EDITABLE) && ClipboardGet(TYPE_TEXT, NULL)) ? 0 : MI_DISABLE, NULL,
					NewMenuItemSeparator(
						NewMenuItem("Delete", NULL, MSG_DELETE, (TEXTBOX(tb)->SelPos != TEXTBOX(tb)->SelPosEnd) && (TEXTBOX(tb)->Flags & TBF_EDITABLE) ? 0 : MI_DISABLE, NULL,
							NULL))))));
}

void processHtmlNode(xmlNodePtr node) {
	xmlNodePtr cur = node;

	while (cur) {
		if (cur->type == XML_ELEMENT_NODE) {
			// Process the HTML element node
			// You can add your code here to handle different HTML elements
			// For example, you can extract hyperlinks and navigate to other pages

			// Check if the element is an <a> tag (hyperlink)
			if (xmlStrcmp(cur->name, (const xmlChar*)"a") == 0) {
				// Get the href attribute of the <a> tag
				const xmlChar* href = xmlGetProp(cur, (const xmlChar*)"href");

				if (href) {
					// Navigate to the URL specified in the href attribute
					printf("Navigating to URL: %s\n", (const char*)href);
					// You can perform the necessary action to navigate to the URL
					// For example, you can update the text box with the new HTML content
				}
			}
		}

		// Recursively process child nodes
		processHtmlNode(cur->children);

		cur = cur->next;
	}
}


l_int Main(int argc, l_text* argv)
{

	TRect r;
	PToolbarItem ti;

	RectAssign(&r, 0, 0, 540, 380);
	w = CreateWindow(&Me, r, "Orchid Browser", WF_NORMAL | WF_MAXIMIZE | WF_CENTERED | WF_RESIZE);
	WIDGET(w)->AppEvHdl = &AppEventHandler;
	InsertWidget(WIDGET(DeskTop), WIDGET(w));


	Filter = NewFileTypes("HTML files", "htm,html",
		NewFileTypes("Text files", "txt",
			NewFileTypes("All files", NULL,
				NULL)));

	PMenu Menu = NewMenu(
		NewMenuItem("File", NULL, NULL, NULL,
			NewMenu(
				NewMenuItem("New", NULL, MSG_FILENEW, NULL, NULL,
					NewMenuItem("Open", NULL, MSG_FILEOPEN, NULL, NULL,
						NewMenuItem("Save", NULL, MSG_FILESAVE, NULL, NULL,
							NewMenuItem("Save as...", NULL, MSG_FILESAVEAS, NULL, NULL,
								NewMenuItem("Exit", NULL, WM_CLOSE, NULL, NULL, NULL)))))
			),
		NewMenuItemEx("Edit", NULL, 0, 0, NULL, &GenerateEditSubMenu, NULL,
			NewMenuItem("View", NULL, NULL, NULL, NULL,
				NewMenuItem("Help", NULL, NULL, NULL,
					NewMenu(
						NewMenuItem("About", NULL, WM_ABOUT, NULL, NULL, NULL)),
					NULL))))
	);

	RectAssign(&r, 0, 0, 540, 20);
	PMenuView o = NewMenuView(&Me, r, Menu, MenuViewStyleHorizontal, 0);
	InsertWidget(WIDGET(w), WIDGET(o));

	RectAssign(&r, 510, 25, 540, 40);
	b = CreateButton(&Me, r, "Go", MSG_GO);
	InsertWidget(WIDGET(w), WIDGET(b));

	RectAssign(&r, 0, 25, 505, 40);
	addtb = CreateTextbox(&Me, r, TBF_EDITABLE);
	WIDGET(addtb)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT;
	InsertWidget(WIDGET(w), WIDGET(addtb));

	RectAssign(&r, 10, 45, 530, 370);
	tb = CreateTextbox(&Me, r, TBF_EDITABLE | TBF_MULTILINE);
	WIDGET(tb)->Flags |= WF_AUTORESIZE | WF_MAGNETRIGHT | WF_MAGNETBOTTOM;
	InsertWidget(WIDGET(w), WIDGET(tb));


	if (argc > 1)
	{
		l_text txt = LoadData2(argv[1], TYPE_TEXT);
		OpenedFile = TextDup(argv[1]);

		if (txt)
		{
			TextBoxSetTextEx(addtb, argv[1]);
			TextBoxSetTextEx(tb, txt);
			free(txt);
		}
	}

	WidgetDrawAll(WIDGET(w));
	return true;
}

void Close(void)
{
	if (OpenedFile) free(OpenedFile);

	WidgetDispose(WIDGET(w));
	FreeFileTypes(Filter);
	xmlCleanupParser();
}
