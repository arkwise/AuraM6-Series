////////////////////////////////////////////////////////////////////////////////
//
//      My IP 0.03
//
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "tcp.h"
#include "debug.h"

l_ulong AppVersion = ULONG_ID(0, 0, 0, 1);
char AppName[] = "MyIP";
l_uid nUID = "app:regedit";
l_uid NeededLibs[] = { "label", "button", "window", "" };

/**
 * Button messages
 */
#define MSG_REFRESH 100001

/**
 * Global variables
 */
PLabel LblLocalIPCaption = 0;
PLabel LblInternetIPCaption = 0;
PLabel LblLocalIPAdr = 0;
PLabel LblInternetIPAdr = 0;

int GetIP(char* LocalIP, char* InternetIP) {
    char buffer[513];
    char* strIP = NULL;
    int status;
    longword host;
    tcp_Socket tsock;
    int len;
    
    // Initialize IP addresses with placeholder values
    strcpy(InternetIP, "???.???.???.???");
    dbug_init();
    sock_init();
    
    DebugMessage("Resolving local IP address...");
    char* local_ip_str = inet_ntoa(gethostid());
    if (!local_ip_str) {
        DebugMessage("Error: Unable to resolve local IP address");
        strcpy(LocalIP, "Error");
        return 1;
    }
    strcpy(LocalIP, local_ip_str);
    
    DebugMessage("Local IP: %s", LocalIP);

    DebugMessage("Resolving host 'checkip.dyndns.org'...");
    if (!(host = resolve("checkip.dyndns.org"))) {
        DebugMessage("Error: Could not resolve host 'checkip.dyndns.org'");
        return 2;
    }

    DebugMessage("Attempting to open 'checkip.dyndns.org' on port 80...");
    if (!tcp_open(&tsock, 0, host, 80, NULL)) {
        DebugMessage("Error: Unable to open TCP session to 'checkip.dyndns.org'");
        return 3;
    }

    sock_wait_established(&tsock, sock_delay, NULL, &status);
    if (status != 1) {
        DebugMessage("Error: TCP connection not established");
        return 4;
    }
    DebugMessage("Socket established");

    sock_puts(&tsock, "GET / HTTP/1.1\n");
    sock_puts(&tsock, "Host: checkip.dyndns.org\n");
    sock_puts(&tsock, "\n");
    sock_flush(&tsock);
    
    DebugMessage("Request sent, waiting for response...");

    sock_wait_input(&tsock, 30, NULL, &status);
    if (status != 1) {
        DebugMessage("Error: No response from server");
        return 5;
    }

    len = sock_dataready(&tsock);
    if (len > sizeof(buffer)) len = sizeof(buffer);
    sock_read(&tsock, buffer, len);
    buffer[len] = 0;

    const char* suffix = "</body>";
    strIP = strstr(buffer, suffix);
    if (strIP != NULL) {
        *strIP = 0;
        const char* prefix = "Current IP Address: ";
        strIP = strstr(buffer, prefix);
        if (strIP != NULL) {
            strcpy(InternetIP, strIP + strlen(prefix));
        }
    }

    sock_wait_closed(&tsock, sock_delay, NULL, &status);
    if (status != 1) {
        DebugMessage("Error: Connection not closed properly");
        return 6;
    }
    // sock_close(&tsock);
sock_err:
    return 0;
}

void Refresh() {
    char LocalIP[20];
    char InternetIP[20];
    int result;

    strcpy(LocalIP, "");
    strcpy(InternetIP, "");

    result = GetIP(LocalIP, InternetIP);
    switch (result) {
        case 0:
            DebugMessage("IP addresses retrieved successfully");
            break;
        case 1:
            MessageBox(&Me, "Error", "Unable to resolve local IP address", MBB_OK);
            return;
        case 2:
            MessageBox(&Me, "Error", "Could not resolve host 'checkip.dyndns.org'", MBB_OK);
            return;
        case 3:
            MessageBox(&Me, "Error", "Unable to open TCP session to 'checkip.dyndns.org'", MBB_OK);
            return;
        case 4:
            MessageBox(&Me, "Error", "TCP connection not established", MBB_OK);
            return;
        case 5:
            MessageBox(&Me, "Error", "No response from server", MBB_OK);
            return;
        case 6:
            MessageBox(&Me, "Error", "Connection not closed properly", MBB_OK);
            return;
        default:
            MessageBox(&Me, "Error", "Unknown error occurred", MBB_OK);
            return;
    }
    DebugMessage("Local IP: %s", LocalIP);
    DebugMessage("Internet IP: %s", InternetIP);

    LabelSetText(LblLocalIPAdr, LocalIP);
    LabelSetText(LblInternetIPAdr, InternetIP);
}

l_bool AppEventHandler(PWidget o, PEvent Event) {
    if (Event->Type == EV_MESSAGE) {
        switch (Event->Message) {
            case WM_CLOSE:
                WidgetDispose(WIDGET(o));
                CloseApp(&Me);
                return true;
            case WM_ABOUT:
                MessageBox(&Me, "About My IP", "My IP \nAura My IP\n\nCopyright (c) 2012 Finn Technologies. All rights reserved.", MBB_OK);
                return true;
            case MSG_REFRESH:
                Refresh();
                return true;
        }
    }
    return false;
}

l_int Main(int argc, l_text* argv) {
    PWindow w = 0;
    PButton b = 0;
    TRect r;

    RectAssign(&r, 0, 0, 285, 90);
    w = CreateWindow(&Me, r, "My IP", WF_CAPTION | WF_FRAME | WF_MINIMIZE | WF_CENTERED);
    WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));
    WidgetPreCenter(WIDGET(w));

    WidgetSize(&r, 10, 5, 130, 20);
    LblLocalIPCaption = CreateLabel(&Me, r, "Local IP");
    WIDGET(LblLocalIPCaption)->BackgroundColor = makecol(250, 250, 250);
    InsertWidget(WIDGET(w), WIDGET(LblLocalIPCaption));

    WidgetSize(&r, 145, 5, 130, 20);
    LblLocalIPAdr = CreateLabel(&Me, r, "xxx.xxx.xxx.xxx");
    WIDGET(LblLocalIPAdr)->BackgroundColor = makecol(250, 250, 250);
    InsertWidget(WIDGET(w), WIDGET(LblLocalIPAdr));

    WidgetSize(&r, 10, 30, 130, 20);
    LblInternetIPCaption = CreateLabel(&Me, r, "Internet IP");
    WIDGET(LblInternetIPCaption)->BackgroundColor = makecol(250, 250, 250);
    InsertWidget(WIDGET(w), WIDGET(LblInternetIPCaption));

    WidgetSize(&r, 145, 30, 130, 20);
    LblInternetIPAdr = CreateLabel(&Me, r, "xxx.xxx.xxx.xxx");
    WIDGET(LblInternetIPAdr)->BackgroundColor = makecol(250, 250, 250);
    InsertWidget(WIDGET(w), WIDGET(LblInternetIPAdr));

    WidgetSize(&r, 190, 60, 85, 30);
    b = CreateButton(&Me, r, "Refresh", MSG_REFRESH);
    InsertWidget(WIDGET(w), WIDGET(b));

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void) {

}
