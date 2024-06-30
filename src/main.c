/**
*       Project:        Aura
*       
*       Release:        Milestone 6
*
*       all original code was derived from lukas lipka & point mad also updates in the mid 2000's by florian xavier, chase finn and chelson aitcheson of doscore.
* 
*       the aim of aura m6 is to complete the TCP app transition and finalize the multithreading code + the sdk so people can finally develop their own apps based on 
*       the samples included in the source code.
*/

#define KERNEL_FILE

#include "allegro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kernel.h"
#include "dynld.h"
#include "internal.h"
#include "widget.h"
#include "menu.h"

// Debugging message macro
//#define TSS(x) DebugMessage("Type: %s\tSize: %i %i", #x, sizeof(x), sizeof(x) % 8);

void DMS2Init(void);
void DMS2DeInit(void);

jmp_buf MainPoint;

/**
*       NAME: GSExit
*       DESCRIPTION: Setting this variable to true ends the main program loop
*               But DO NOT set it directly to true, use the MSG_QUIT event !
*/
extern l_bool GSExit;

/**
*       NAME: KernelMode
*       DESCRIPTION: Defines the kernel mode
*/
l_ulong KernelMode = 0;

#ifdef _OZONE__DJGPP_
long Platform = PL_DOS;
#endif
#ifdef _OZONE__WIN32_
long Platform = PL_WIN;
#endif
#ifdef _OZONE__LINUX_
long Platform = PL_LNX;
#endif

/**
*       NAME: Kernel information
*       DESCRIPTION: Information about the kernel itself (version, build)
*/
_PUBLIC char* KernelName                = "Aura Kernel";
_PUBLIC char* KernelCopyright   = "doscore 2007-2024";
_PUBLIC int   KernelVersion             = 4080;
_PUBLIC char* KernelTextVersion = "Milestone 6 2b";

/**
*       NAME: System initialization
*/
_PRIVATE void _SystemInit(void)
{
    DebugMessage("Aura System initialization");

    DebugMessage("Initializing DynLd...");
    InitDynLd();
    RegisterExports();
    DebugMessage("DynLd initialized");

    DebugMessage("Initialize VFS...");
    GSSystemVFSInstall();
    DebugMessage("VFS Initialized");

    DebugMessage("Initializing registry...");
    NRegistryInit();
    DebugMessage("Registry initialized");

    DebugMessage("Initializing text manipulation functions...");
    DebugMessage("  ASCII/UTF-8...");
    InitText();
    DebugMessage("  UTF-16...");
    InitUText();
    DebugMessage("Text manipulation functions initialized");

    DebugMessage("Initialize Events System...");
    InitializeEventsSystem();
    DebugMessage("Events System Initialized");

    DebugMessage("Install Allegro...");
    allegro_init();
    install_keyboard();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    DebugMessage("Allegro installed");

    DebugMessage("Initialize DMS v2 (Data Management System)...");
    DMS2Init();
    DebugMessage("DMS initialized");

    DebugMessage("Initialize basic external drivers...");
    RegistryLoadLibraries("/SYSTEM/BASICDRIVERS");
    DebugMessage("External basic drivers initialized successfully");

    DebugMessage("Install screen...");
    GSSystemScreenInit();
    DebugMessage("Screen initialized successfully");

    DebugMessage("Initialize Timer...");
    InitTimer();
    DebugMessage("Timer initialized");

    DebugMessage("Add system fonts...");
    default_font = FontLoad(KeyGetText("/SYSTEM/FONTS", "SYSTEM/FONTS/HELV13.FNT"));
    if (!default_font) default_font = font;
    DebugMessage("All system fonts successfully added");

    DebugMessage("Install keyboard driver...");
    GSSystemKeyboardInstall();
    DebugMessage("Keyboard driver installed successfully.");

    DebugMessage("Install mouse driver...");
    GSSystemMouseInstall();
    DebugMessage("Mouse driver installed successfully.");

    DebugMessage("Initializing Socket...");
    sock_init(); // Disabled for now
    DebugMessage("Socket Initialized...");

    DebugMessage("Initialize drivers");
    RegistryLoadLibraries("/SYSTEM/DRIVERS");
    DebugMessage("Drivers initialized successfully");

    DebugMessage("Load libraries...");
    RegistryLoadLibraries("/SYSTEM/AUTOLIB");
    DebugMessage("Libraries loaded");

    {
        void (*func)(void);
        func = (void*)ResolveSymbol(NULL, "DisposeStartupLogo");
        if (func) func();
    }
    MouseShow();

    DebugMessage("Load start-up entries...");
    RegistryRunEntries("/SYSTEM/STARTUP");
    DebugMessage("Desktop initialized");

    DebugMessage("System Startup Complete\n");
}

/**
*       NAME: System Event Handler
*       DESCRIPTION: System events get handled here.
*/
_PRIVATE void _SystemEventHandler(PEvent Event)
{
    if (Event->Type & EV_KEYBOARD)
    {
        if (ALT(Event, KEY_X))
        {
            EventAssign(Event, EV_MESSAGE, MSG_QUIT, NULL, KState, Mouse->State);
            HandleEvent(Event);
            CLEAR_EVENT(Event);
        }

        if (ALT(Event, KEY_F))
        {
            DynLdRun("xapps/nav.app", NULL);
            CLEAR_EVENT(Event);
        }

        if (ALT(Event, KEY_R))
        {
            DynLdRun("xapps/regedit.app", NULL);
            CLEAR_EVENT(Event);
        }

        if (CTRL(Event, KEY_F8))
        {
            p_bitmap shot = create_bitmap(GSScreenWidth, GSScreenHeight);
            blit(screen, shot, 0, 0, 0, 0, GSScreenWidth, GSScreenHeight);
            SaveData2("shot.bmp", shot, TYPE_IMAGE);
            destroy_bitmap(shot);
            CLEAR_EVENT(Event);
        }
        if (CTRL(Event, KEY_F12))
        {
            p_bitmap shot = create_bitmap(GSScreenWidth, GSScreenHeight);

            MouseHide();

            if (shot)
            {
                p_bitmap trans = create_bitmap(GSScreenWidth, GSScreenHeight), o = screen;

                blit(screen, shot, 0, 0, 0, 0, GSScreenWidth, GSScreenHeight);
                set_clip_rect(screen, 0, 0, GSScreenWidth, GSScreenHeight);

                if (trans)
                {
                    set_trans_blender(0, 0, 0, 0);
                    draw_lit_sprite(trans, shot, 0, 0, 192);
                    o = trans;
                }

                rectfill(o, GSScreenWidth / 2 - 57, GSScreenHeight / 2 - 50,
                    GSScreenWidth / 2 - 7, GSScreenHeight / 2 + 50, makecol(255, 255, 255));

                rectfill(o, GSScreenWidth / 2 + 7, GSScreenHeight / 2 - 50,
                    GSScreenWidth / 2 + 57, GSScreenHeight / 2 + 50, makecol(255, 255, 255));

                textout_centre_ex(o, default_font, "paused", GSScreenWidth / 2 + 5, GSScreenHeight / 2 + 52, makecol(255, 255, 255), -1);
                textout_centre_ex(o, default_font, "press any key", GSScreenWidth / 2 + 5, GSScreenHeight / 2 + 53 + text_height(default_font), makecol(255, 255, 255), -1);

                if (trans)
                {
                    blit(trans, screen, 0, 0, 0, 0, GSScreenWidth, GSScreenHeight);
                    destroy_bitmap(trans);
                }
            }

            while (!keypressed());
            readkey();

            if (shot)
            {
                blit(shot, screen, 0, 0, 0, 0, GSScreenWidth, GSScreenHeight);
                destroy_bitmap(shot);
            }

            MouseShow();

            CLEAR_EVENT(Event);
        }

    }
}

int main(int argc, char* argv[])
{
    /**
     * Select the kernel mode
     */
    if (argc > 1) if (argv[1])
    {
        if (!strcmp(argv[1], "-safe"))
        {
            KernelMode = KERNEL_SAFE;
        }
        else if (!strcmp(argv[1], "-resetup"))
        {
            KernelMode = KERNEL_RESETUP;
        }
    }

    GSSystemDebugInstall("/syslogd.txt");
    DebugMessage("Command: %s", argv[0]);

    /**
     * Install multitasking
     */
    //ThreadInit();

    InstallEventHandler(_SystemEventHandler);

    _SystemInit();

    setjmp(MainPoint); // Secure return point

    while (!GSExit)
    {
        SysPoll();

#ifdef _OZONE__DJGPP_
        __dpmi_yield();
#endif
    }

    //DebugMessage("Uninitialising System Threads");
    //KillThread(EventsTheard);
    //KillThread(TasksTheard);

    //GSSystemScreenQuit();

    //SHUTDOWN SYSTEM		
    ShutDownDynLd(); // Close all applications, and shut down dynamic loader
    ShutDownTimer();
    GSSystemMouseUninstall();
    ShutdownEventsSystem();
    DMS2DeInit();
    allegro_exit();
    NRegistryUnInit();
    GSSystemVFSUnInstall();
    GSSystemDebugUninstall();
    return 0;
}
END_OF_MAIN();
