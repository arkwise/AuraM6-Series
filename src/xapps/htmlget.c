#include "kernel.h"
#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "tbox.h"
#include "iodlg.h"
#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

#ifdef __GNUC__
#define write _write
#define close _close
#endif

#define WINDOWSIZE  (16*1024)
#define BUFFERSIZE  2048
#define PROGRAM     "HTMLGET"
#define VERSION     "1.00"
#define AUTHOR      "Finn"
#define HTTPVER     "HTTP/1.0"
#define HTTPVER10   "HTTP/1.0"
#define HTTPVER11   "HTTP/1.1"
#define strn(s)     s, sizeof(s)-1
#define MSG_GET_IT        100001
#define MSG_SELECTFILE    0x00010002

typedef unsigned long  DWORD;

l_ulong AppVersion = ULONG_ID(0, 0, 0, 1);
char AppName[] = "HtmlGet";
l_uid nUID = "htmlget";
l_uid NeededLibs[] = {"label", "iodlg", "textbox", "button", "window", ""};

PWindow w = 0;
PButton BtnGetIt = 0;
PButton BtnSelectFile = 0;
PLabel LblFromHttp = 0;
PLabel LblTo = 0;
PLabel LblProgress = 0;
PTextbox TxtFromHttp = 0;
PTextbox TxtTo = 0;
l_text File = 0;
PFileInfo FFile = 0;
char *buffer;
tcp_Socket *sock;
char *outputfile = NULL;
FILE *of;
void (*prev_hook) (const char*, const char*);

int sock_getline(void *sock, char *buf, int len) {
    int i;
    char ch;

    for (i = 0, --len; i <= len && sock_read(sock, &ch, 1) > 0;) {
        if (ch == '\n')
            break;
        if (ch != '\r') {
            *buf++ = ch;
            ++i;
        }
    }
    *buf = '\0';
    return (i);
}

long header(const char *path) {
    int i, len, response;
    long contentlength;
    char *s;

    contentlength = LONG_MAX;
    if ((len = sock_getline(sock, buffer, BUFFERSIZE)) <= 0) {
        DebugMessage("EOF from server");
        MessageBox(&Me, "Error", "EOF from server", MBB_OK);
        return (-1L);
    }
    if (strncmp(s = buffer, strn(HTTPVER10)) &&
            strncmp(s, strn(HTTPVER11))) {
        DebugMessage("Not a HTTP/1.[01] server");
        MessageBox(&Me, "Error", "Not a HTTP/1.[01] server", MBB_OK);
        return (-1L);
    }

    s += sizeof (HTTPVER10) - 1;
    if ((i = strspn(s, " \t")) <= 0) {
        DebugMessage("Malformed HTTP/1.[01] line");
        MessageBox(&Me, "Error", "Malformed HTTP/1.[01] line", MBB_OK);
        return (-1L);
    }
    s += i;
    response = 500;
    sscanf(s, "%3d", &response);
    if (response == 401) {
        DebugMessage("%s: authorisation failed!", path);
        MessageBox(&Me, "Error", "Authorisation failed", MBB_OK);
        return (-1L);
    } else if (response != 200 && response != 301 &&
            response != 302 && response != 304) {
        DebugMessage("%s: %s", path, s);
        MessageBox(&Me, "Error", s, MBB_OK);
        contentlength = -1L;
    }
    while ((len = sock_getline(sock, buffer, BUFFERSIZE)) > 0) {
        if (!strnicmp(s = buffer, strn("Content-Length:"))) {
            s += sizeof ("Content-Length:") - 1;
            contentlength = atol(s);
        } else if (!strnicmp(buffer, strn("Location:"))) {
            if (response == 301 || response == 302) {
                DebugMessage("At %s", buffer);
            }
        } else if (strchr(" \t", buffer[0])) {
            DebugMessage("Warning: continuation line encountered");
            MessageBox(&Me, "Warning", "Continuation line encountered", MBB_OK);
        }
    }
    return (response == 304 ? 0L : contentlength);
}

int htget(const char *host, int port, const char *path) {
    char line[1024];
    DWORD hostaddr;
    int status = 0;
    int connected = 0;
    int completed = 0;
    int i;
    long length, contentlength = 0L;
    const char *name;
    char *buf = buffer;
    char url[512];
    PFile f = NULL;

    DebugMessage("htget:");

    outputfile = TxtTo->Text;
    name = host;

    if ((hostaddr = lookup_host(name, NULL)) == 0) {
        DebugMessage("Error: Unable to resolve host");
        MessageBox(&Me, "Error", "Unable to resolve host", MBB_OK);
        return (1);
    }

    if (!tcp_open(sock, 0, hostaddr, port, NULL)) {
        sprintf(line, "Cannot connect to `%s'", name);
        DebugMessage(line);
        MessageBox(&Me, "Error", line, MBB_OK);
        return (1);
    }

    sock_setbuf(sock, malloc(WINDOWSIZE), WINDOWSIZE);

    sock_wait_established(sock, sock_delay, NULL, &status);
    if (status != 1) {
        DebugMessage("Error: TCP connection not established");
        MessageBox(&Me, "Error", "TCP connection not established", MBB_OK);
        return (1);
    }
    connected = 1;
    completed = 1;
    sock_tick(sock, &status);
    strcpy(url, path);
    buf += sprintf(buf, "%s %s " HTTPVER "\r\n"
            "User-Agent: " PROGRAM "-DOS/" VERSION "\r\n",
            "GET", url);
    buf += sprintf(buf, "\r\n");
    sock_fastwrite(sock, buffer, buf - buffer);

    if ((contentlength = header(path)) >= 0L) {
        DebugMessage("contentlength: %lu", contentlength);
        if (outputfile) {
            f = FileOpen(outputfile, "wb");

            if (f == NULL) {
                MessageBox(&Me, "Error", "Unable to open output file", MBB_OK);
                goto close_up;
            }
        }

        length = 0L;
        while ((i = sock_read(sock, buffer, BUFFERSIZE)) > 0) {
            FileWrite(buffer, 1, i, f);
            length += i;
            sprintf(line, "Got %lu bytes", length);
            LabelSetText(LblProgress, line);
            WidgetDrawAll(WIDGET(LblProgress));
        }
        FileClose(f);
        if (contentlength != LONG_MAX && length != contentlength) {
            sprintf(line, "Warning, actual length = %ld, content length = %ld",
                    length, contentlength);
            DebugMessage(line);
            MessageBox(&Me, "Warning", line, MBB_OK);
        }
    }

close_up:
    sock_close(sock);
    sock_wait_closed(sock, sock_delay, NULL, &status);

sock_err:
    DebugMessage("sock_err:");
    if (status == -1) {
        sprintf(line, "`%s' %s", name, sockerr(sock));
        DebugMessage(line);
        MessageBox(&Me, "Error", line, MBB_OK);
    }
    if (!connected) {
        DebugMessage("Could not get connected");
        MessageBox(&Me, "Error", "Could not get connected", MBB_OK);
    }
    return (!completed || contentlength < 0L);
}

void cnf_hook(const char *name, const char *value) {
    if (prev_hook)
        (*prev_hook) (name, value);
}

void set_cnf_hook(void) {
    prev_hook = usr_init;
    usr_init = cnf_hook;
}

int GetURL(char *URL) {
    char line[1024];
    char *host, *path, *s;
    int port = 80;
    buffer = malloc(BUFFERSIZE);
    sock = malloc(sizeof (*sock));
    if (!buffer || !sock) {
        DebugMessage("No memory");
        MessageBox(&Me, "Error", "No memory", MBB_OK);
        return (-1);
    }

    if (!strnicmp(URL, strn("http://")))
        URL += sizeof ("http://") - 1;

    if ((path = strchr(URL, '/')) == NULL) {
        host = URL;
        path = "/";
    } else {
        if ((host = calloc(path - URL + 1, 1)) == NULL) {
            DebugMessage("Out of memory");
            MessageBox(&Me, "Error", "Out of memory", MBB_OK);
            return (1);
        }
        strncat(host, URL, path - URL);
    }
    if ((s = strchr(host, ':')) != NULL) {
        *s++ = '\0';
        port = atoi(s);
    }

    tzset();
    set_cnf_hook();
    sock_init();

    sprintf(line, "Host: %s\nPort: %d\nPath: %s", host, port, path);
    DebugMessage(line);
    int status = htget(host, port, path);
    free(buffer);
    free(sock);
    return (status);
}

void GetIt() {
    int status;
    char line[1024];
    DebugMessage("HTTP_GET -> GetIt() -> Start Download");
    DebugMessage("GetURL:");
    DebugMessage(TxtFromHttp->Text);
    status = GetURL(TxtFromHttp->Text);
    if (status) MessageBox(&Me, "Info Message", "Process finished with errors", MBB_OK);
    else MessageBox(&Me, "Info Message", "Process finished successfully", MBB_OK);
}

l_bool AppEventHandler(PWidget o, PEvent Event) {
    if (Event->Type == EV_MESSAGE) {
        switch (Event->Message) {
            case WM_CLOSE:
            {
                WidgetDispose(WIDGET(o));
                CloseApp(&Me);
                return true;
            }
            case WM_ABOUT:
            {
                MessageBox(&Me, "About HTML Get", "HTML Get \nAura HTTP Get\n\nCopyright (c) 2013 Finn Technologies. All rights reserved.", MBB_OK);
                return true;
            }
            case MSG_SELECTFILE:
            {
                File = IOBox("Save As", IOBOX_SAVE, NULL, NULL, false);
                if (File) {
                    TextBoxSetText(TxtTo, "%s", File);
                    free(File);
                }
                return true;
            }
            case MSG_GET_IT:
            {
                GetIt();
                return true;
            }
        }
    }
    return false;
}

l_int Main(int argc, l_text *argv) {
    TRect r;
    RectAssign(&r, 0, 0, 285, 90);
    w = CreateWindow(&Me, r, "HTML Get", WF_CAPTION | WF_FRAME | WF_MINIMIZE | WF_CENTERED);
    WIDGET(w)->AppEvHdl = &AppEventHandler;
    InsertWidget(WIDGET(DeskTop), WIDGET(w));
    WidgetPreCenter(WIDGET(w));

    WidgetSize(&r, 10, 5, 60, 20);
    LblFromHttp = CreateLabel(&Me, r, "From http://");
    InsertWidget(WIDGET(w), WIDGET(LblFromHttp));

    WidgetSize(&r, 75, 5, 205, 20);
    TxtFromHttp = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(TxtFromHttp));

    WidgetSize(&r, 10, 30, 15, 20);
    LblTo = CreateLabel(&Me, r, "To: ");
    InsertWidget(WIDGET(w), WIDGET(LblTo));

    WidgetSize(&r, 30, 30, 225, 20);
    TxtTo = CreateTextbox(&Me, r, TBF_EDITABLE);
    InsertWidget(WIDGET(w), WIDGET(TxtTo));

    WidgetSize(&r, 260, 30, 20, 20);
    BtnSelectFile = CreateButton(&Me, r, "...", MSG_SELECTFILE);
    InsertWidget(WIDGET(w), WIDGET(BtnSelectFile));

    WidgetSize(&r, 190, 60, 85, 30);
    BtnGetIt = CreateButton(&Me, r, "Get It", MSG_GET_IT);
    InsertWidget(WIDGET(w), WIDGET(BtnGetIt));

    WidgetSize(&r, 10, 60, 160, 20);
    LblProgress = CreateLabel(&Me, r, "");
    InsertWidget(WIDGET(w), WIDGET(LblProgress));

    WidgetDrawAll(WIDGET(w));
    return true;
}

void Close(void) {
}
