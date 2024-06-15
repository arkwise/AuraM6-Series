////////////////////////////////////////////////////////////////////////////////
//
//      Dynamic Loading System - Loader
//
//      (c) Copyright 2003,2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#define  KERNEL_FILE
#include "kernel.h"
#include "string.h"

l_bool UnLoadLibraries ( l_uid * );

extern jmp_buf MainPoint;

/** updated path for library **/



/**
*       Global variables
*/
PList Symbols           = 0;
PList Applications      = 0;
PList AppExtenders  = 0;
/**
*       Kernel indetification
*/
/*TApplication Me =
{
        "Codename: Phoenix Kernel (update)",
        "",
        ULONG_ID(0,5,8,0),
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        0,
        "kernel",
        0,
        1
};*/

// below is better/right? I dont know... Florian Xaver



// this code still has some of the core elements of ozone but has changed pass it.


TApplication Me =
{
        "Codename: tech7 Kernel (2024)",
        "",
        ULONG_ID(0,9,8,0),
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        0,
        0,
        0,
        0,
0,
0,
0,
0,
0,
0,
0
};

/**
*       NAME: AddSymbol
*       DESCRIPTION: Used by the macros to add or export a symbol
*       RETURN: The exported symbol
*/
PSymbol AddSymbol ( l_text Name, void *Address, PApplication App )
{
        PSymbol o;

        if ( o = (PSymbol)ListKeyItem(Symbols, Name) )
        {
                DebugWarning("Duplicate symbol: %s orignaly registered by %x", Name, o->App);
                return 0;
        }

        o = NEW(TSymbol);

        if ( !o || !Name ) return NULL;

        memset(o, 0, sizeof(TSymbol));
        o->Itm.Key              = (l_text)strdup(Name);
        o->Itm.Data             = Address;
        o->Itm.FreeData = NULL;
        o->App                  = App;

        ListAddItem(Symbols, (PListItem)o);

        return o;
}

/**
*       NAME: ResolveSymbol
*       DESCRIPTION: Resolves the symbol in a DynLD app
*       RETURN: The address of the symbol
*/
l_long ResolveSymbol ( PApplication App, l_text Name )
{
        l_long Address = 0;

        if ( !strcmp(Name, "Me") )
                Address = (l_long)App;
        else
                Address = (l_long)ListKey(Symbols, Name);

        if ( !Address )
        {
                DebugError ("0x%08x :: Unresolved external %s", App, Name);
        }

        return Address;
}

void FreeApplication ( void *o )
{
        PApplication App = (PApplication)o;

        DebugMessage("Free Application :: %s",App->Name);

        if ( App->Close ) App->Close();
        //if ( App->ThreadId ) KillThread(App->ThreadId);

        if ( App->Name )        free(App->Name);
        if ( App->FileName )    free(App->FileName);
        if ( App->Ressource )   FreeList(App->Ressource);
        if ( App->Icon )                FreeIcon(App->Icon);
        if ( App->argv)                 FreeArgs(App->argc,App->argv);
        free(App->Data);

/* ** Due to unknown threading bugs, this section is unactivated **

        FreeList(App->EventsWaitList);

        */
        free(App);
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
PAppExtender InstallAppExtender ( void (*Init)(PApplication), void (*DeInit)(PApplication) ) {
        PAppExtender o = malloc(sizeof(TAppExtender));
        if ( !o ) return NULL;
        o->Init = Init;
        o->DeInit = DeInit;
        ListAdd(AppExtenders,NULL,o,&free);
        return o;
}
////////////////////////////////////////////////////////////////////////////////
void RemoveAppExtender ( PAppExtender o) {
        PListItem a = ListFoundItem(AppExtenders,o);
        if ( a ) ListRemoveItem(AppExtenders,a);
}
////////////////////////////////////////////////////////////////////////////////
void AppExtendersInit ( PApplication App ) {
        if  ( AppExtenders->Last ) {
                PListItem a, b;
                PAppExtender o;
                a = b = AppExtenders->Last->Next;
                do {
                        o = (PAppExtender)a->Data;
                        if ( o->Init ) o->Init(App);
                        a = a->Next;
                } while ( a != b );
        }
}
////////////////////////////////////////////////////////////////////////////////
void AppExtendersDeInit ( PApplication App ) {
        if  ( AppExtenders->Last ) {
                PListItem a, b;
                PAppExtender o;
                a = b = AppExtenders->Last;
                do {
                        o = (PAppExtender)a->Data;
                        if ( o->DeInit ) o->DeInit(App);
                        a = a->Prev;
                } while ( a != b );
        }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
PApplication GetLibrary ( l_uid id ) {

        if ( Applications->Last )
        {
                PListItem a = Applications->Last;
                PListItem b = a;
                PApplication App;
                do
                {
                        App = (PApplication)(a->Data);
                        if ( !TextCompare(App->UID,id) )
                                return App;
                        a = a->Next;
                } while ( a != b );
        }
        return NULL;
}
////////////////////////////////////////////////////////////////////////////////
l_bool LoadLibrary ( l_uid id ) {
        PApplication app = GetLibrary(id);
        if ( !app ) {
                l_text key = TextArgs("/SYSTEM/LIBRARIES/%s",id);
                if ( key ) {
                        l_text file = KeyGetText(key,NULL);
                        free(key);
                        if ( !file ) {
                                file = TextArgs("xlib/%s.dl",id);
                                if ( !file ) return false;
                        }
                        app = DynLdRun(file,NULL);
                        if ( !app ) return false;
                } else
                        return false;
        } else
                app->Users++;
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool UnLoadLibrary ( l_uid id ) {
        PApplication app = GetLibrary(id);

        if ( !app ) return false;
        app->Users--;

        if ( !app->Users ) CloseApp(app);

        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool LoadLibraries ( l_uid *ids ) {
        while ( (*ids)[0] ) {
                if ( !LoadLibrary(*ids) ) return false;
                ids ++;
        }
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool UnLoadLibraries ( l_uid *ids ) {
        while ( (*ids)[0] ) {
                if ( !UnLoadLibrary(*ids) ) DebugWarning("Unable to unload '%s'",ids);
                ids ++;
        }
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void FreeHostedApp ( PApplication App ) {
        if ( Applications->Last )
        {
                PListItem a = Applications->Last;
                PListItem n;
                PApplication A;
                do
                {
                        n = a->Next;
                        A = (PApplication)(a->Data);
                        if ( A->Host == App )
                        {
                                FreeHostedApp(A);
                                ListRemoveItem(Applications, a);
                        }
                        a = n;
                } while ( a != Applications->Last );

        }
}
////////////////////////////////////////////////////////////////////////////////
PApplication DynLdRunEx ( l_text Filename, l_int argc, l_text* argv, PApplication Host, PErrorHandler h )
{
        PApplication App                = 0;
        void *TempData                  = 0;
        PDynLdExt Ext                   = 0;
        l_ulong i                               = 0;
        l_ulong DynERROR                = 0;
        TDynLdReloc DynReloc;
        TDynLdHeader DynHead;
        PListItem itm;
        PFile f;

        if ( !argv) {
                DebugError("DYNLD :: %s : argv is NULL !", Filename);

                return NULL;
        }


        /*
        *       Open application file
        */

        f = FileOpen(Filename, "rb");

        if ( !f )
        {
                CreateError(h,ERROR_FILENOTFOUND,"File not found");
                DebugError("DYNLD :: File not found - %s", Filename);
                return NULL;
        }

        FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);

        if ( DynHead.Magic != ULONG_ID('D','n','L','d') )
        {
                FileClose(f);
                CreateError(h,ERROR_INVALIDFILE,"Application is not a valid DynLD");
                DebugError("DYNLD :: Application is not a valid DynLD.");

                return NULL;
        }

        if ( DynHead.FileFormatVersion != DYNLDVERSION )
        {
                FileClose(f);
                CreateError(h,ERROR_INVALIDFILE,"Application has invalid version number of DynLD linker");
                DebugError("DYNLD :: Application has invalid version number of DynLD linker.");

                return NULL;
        }

        App = (PApplication)malloc(sizeof(TApplication));

        if ( !App ) {
                FileClose(f);
                CreateError(h,ERROR_NOTENOUGHMEMORY,NULL);
                DebugError("DYNLD :: Not enough memory to load application.");
                return NULL;
        }

        memset(App, 0, sizeof(TApplication));

        App->Name           = FileReadBinString(f);
        App->FileName       = (l_text)strdup(Filename);
        memcpy(&App->UID,&DynHead.UID,sizeof(l_uid));
        App->Version        = DynHead.FileVersion;
        /* ** Due to unknown threading bugs, this section is unactivated **

        App->EventsWaitList = NewList();
        */
        App->Type           = DynHead.Type;
        App->Host           = Host;


        DebugMessage ("Loading application '%s'(%x), FileName - '%s'",App->Name,App,App->FileName);
        DebugMessage ("         UID - '%s' Version - 0x%08x",App->UID,App->Version);

        /*if ( DynHead.Compression != 0)
        {
                TempData = (void*)malloc(DynHead.Size);
                FileRead(TempData, 1, DynHead.Size, f);

                App->Data = (void*)malloc(DynHead.OriginalSize);
                uncompress(App->Data, &DynHead.OriginalSize, TempData, DynHead.Size);
                DynHead.Size = DynHead.OriginalSize;
                free(TempData);
        }
        else
        {*/
                App->Data = (void*)malloc(DynHead.Size);
                FileRead(App->Data, 1, DynHead.Size, f);
        //}

        if ( DynHead.MainOffset  != NoneOffset ) App->Main  = (void*)(((long)App->Data)+DynHead.MainOffset);
        if ( DynHead.CloseOffset != NoneOffset ) App->Close = (void*)(((long)App->Data)+DynHead.CloseOffset);
        if ( DynHead.LibsOffset  != NoneOffset ) App->Libs  = (void*)(((long)App->Data)+DynHead.LibsOffset);


        if ( App->Libs )
                if ( !LoadLibraries(App->Libs) ) {
                        free(App->Data);
                        free(App);
                        FileClose(f);
                        return NULL;
                }


        Ext = (PDynLdExt)malloc(sizeof(TDynLdExt)*DynHead.Importations);


        for (i = 0; i <DynHead.Importations; i++)
        {
                Ext[i].Name = FileReadBinString(f);
                FileRead(&Ext[i].Symbol, 1, 4, f);
                if ( !ResolveSymbol(App,Ext[i].Name) ) {
                        CreateError(h,ERROR_INVALIDFILE,"Unresolved extrenal");
                        ErrorLog(h,"'%s' not found\n",Ext[i].Name);
                        DynERROR++;
                }
        }

        if ( DynERROR )
        {
                App->Close = NULL;
                DebugError ("0x%08x :: %d error(s) while loading. Application has not started", App, DynERROR);
                FreeApplication(App);
                return NULL;
        }

        for (i = 0; i < DynHead.Relocations; i++)
        {
                FileRead(&DynReloc, 1, sizeof(TDynLdReloc), f);
                if ( DynReloc.Type == REL32_ABSOLUTE || DynReloc.Type == REL32_RELATIVE ) {
                        l_ulong j, addr = (l_ulong)App->Data;

                        for (j = 0; j<DynHead.Importations; j++)
                                if (Ext[j].Symbol == DynReloc.Symbol)
                                {
                                        addr = (l_ulong)ResolveSymbol(App,Ext[j].Name);
                                        break;
                                }

                        if ( DynReloc.Type == REL32_ABSOLUTE )
                                *(long*)(((((long)App->Data))+DynReloc.Address)) += (long)addr;
                        else if ( DynReloc.Type == REL32_RELATIVE )
                                *(long*)(((((long)App->Data))+DynReloc.Address)) += (long)addr-(long)App->Data-(long)DynReloc.Address;

                } else {
                        CreateError(h,ERROR_INVALIDFILE,"Unknown reloaction type");
                        DebugError("DYNLD : Unknown reloaction type");
                        DynERROR++;

                }
        }

        if ( DynHead.RessourceEntries )
                App->Ressource = DMSLoadRessource(f,DynHead.RessourceEntries);


        if ( App->Ressource ) {
                PImage i16,i32,i48;
                i16 = ListKeyCase(App->Ressource,"ICON16");
                i32 = ListKeyCase(App->Ressource,"ICON32");
                i48 = ListKeyCase(App->Ressource,"ICON48");
                if ( i16 || i32 || i48 )
                        App->Icon = NewIcon2(i16,i32,i48);
        }

        FileClose(f);

        for (i=0;i<DynHead.Importations;i++) free(Ext[i].Name);

        free(Ext);

        App->argc = argc;
        App->argv = DuplicateArgs(argc,argv);

        if ( !App->argv ) {
                DebugError("0x%08x :: argv is null",App);
                CreateError(h,ERROR_NOTENOUGHMEMORY,"argv is null");
                DynERROR++;
        }

        if ( DynERROR )
        {
                App->Close = NULL;
                DebugError ("0x%08x :: %d error(s) while loading. Application has not started", App, DynERROR);
                FreeApplication(App);
                return NULL;
        }

        /* ** Due to unknown threading bugs, this section is unactivated **

        if ( App->Type == DYNLD_TYPEAPP )
        {
                // Stack = 10k
                App->ThreadId = NewThread((void*)App->Main,Args,10240,1);
                //DebugMessage ("         Thread - 0x%08x",App->ThreadId);

                while ( App->State == DynLdStateStarting ) Yield();

                //DebugMessage ("         Started!");
        }
        else
        {*/


        DebugMessage("AppExtendersInit");

        AppExtendersInit(App);


        DebugMessage("Adding app to list");
        ListAdd(Applications,NULL,App,FreeApplication);

        DebugMessage("Calling @App->Main(Args)");
        App->State = DYNLD_STRATING;
        if ( App->Main )
                if ( !App->Main(App->argc,App->argv) ) {
                        App->Close = NULL;
                        CloseApp(App);
                        if ( App->Libs ) UnLoadLibraries(App->Libs);
                        if ( itm = ListFoundItem(Applications,App) )
                                ListRemoveItem(Applications, itm);
                        else {
                                DebugWarning("Application wasn't in list");
                                FreeApplication(App);
                        }
                        return NULL;
                }
        //}
        App->State = DYNLD_RUNNING;

        App->Users = 1;


        return App;
}
////////////////////////////////////////////////////////////////////////////////
PApplication DynLdRun ( l_text Filename, l_text Args )
{
        PApplication App;
        l_int argc = Args ? 2 : 1;
        l_text *argv = malloc(sizeof(l_text)*(argc+1));
        argv[0] = TextDup(Filename);
        if ( Args ) argv[1] = TextDup(Args);
        argv[argc] = 0;
        App = DynLdRunEx(Filename,argc,argv,NULL,NULL);
        FreeArgs(argc,argv);
        return App;
}
////////////////////////////////////////////////////////////////////////////////
PApplication DynLdRun2 ( l_text Filename, l_text Args )
{
        PApplication App;
        l_int argc = 0;
        l_text *argv;
        _ParseArgs(Args,Filename,&argc,&argv);
        App = DynLdRunEx(Filename,argc,argv,NULL,NULL);
        FreeArgs(argc,argv);
        return App;
}
////////////////////////////////////////////////////////////////////////////////
PApplication DynLdCommand ( l_text Command )
{
        PApplication App = NULL;
        l_int argc = 0;
        l_text *argv = 0;
        _ParseArgs(Command,NULL,&argc,&argv);
        if ( argv ) {
                App = DynLdRunEx(argv[0],argc,argv,NULL,NULL);
                FreeArgs(argc,argv);
        }
        return App;
}
////////////////////////////////////////////////////////////////////////////////

/* ** Due to unknown threading bugs, this section is unactivated **

void AppSendEvent ( PApplication App, l_ulong Type, l_ulong Msg, void *Extra, void *Dest, void (*Redirect) ( void *, PDynLdEvent ) )
{
        PDynLdEvent Ev = (PDynLdEvent)malloc(sizeof(TDynLdEvent));

        Ev->Ev.Type    = Type;
        Ev->Ev.Message = Msg;
        Ev->Ev.Extra   = Extra;
        Ev->Dest     = Dest;
        Ev->Redirect = Redirect;

        ListAdd(App->EventsWaitList,NULL,Ev,&free);
}

void AppSendEventEv ( PApplication App, TEvent Ev, void *Dest, void (*Redirect) ( void *, PDynLdEvent ) )
{
        PDynLdEvent Evt = (PDynLdEvent)malloc(sizeof(TDynLdEvent));

        Evt->Ev  = Ev;

        Evt->Dest     = Dest;
        Evt->Redirect = Redirect;

        ListAdd(App->EventsWaitList,NULL,Evt,&free);
}

l_bool AppGetNextEvent ( PApplication App, PDynLdEvent Ev )
{
        while (!App->EventsWaitList->Last) Yield();

        memcpy(Ev, App->EventsWaitList->Last->Next->Data, sizeof(TDynLdEvent));

        ListRemoveItem(App->EventsWaitList, App->EventsWaitList->Last->Next);

        if ( Ev->Ev.Message == MSG_KILLAPP && Ev->Ev.Type == EV_MESSAGE ) return false;

        return true;
}

void AppRedirectEvent ( PApplication App, PDynLdEvent Ev )
{
        if ( Ev->Redirect ) Ev->Redirect(Ev->Dest, Ev);
}*/
////////////////////////////////////////////////////////////////////////////////
void BreakApp ( PApplication App ) {
        PListItem itm;
        DebugMessage ("Break :: %s (0x%08x)",App->Name,App);
        DebugMessage ("Try to call close function.");
        CloseApp(App);
        DebugWarning("Closed.");
        if ( App->Libs ) UnLoadLibraries(App->Libs);
        if ( itm = ListFoundItem(Applications,App) )
                ListRemoveItem(Applications, itm);
        else {
                DebugWarning("Application wasn't in list");
                FreeApplication(App);
        }
        longjmp(MainPoint, 1);
        DebugWarning("Should never came here, calling object may no more exists");
}
////////////////////////////////////////////////////////////////////////////////
void CloseApp ( PApplication App ) {

        DebugMessage ("Close :: %s",App->Name);

        FreeHostedApp(App);

        if ( App->Close )
        {

                App->Close();
                App->Close = NULL;
        }
        DebugMessage("AppExtendersDeInit");

        AppExtendersDeInit(App);


        if ( Symbols->Last )
        {
                PListItem a, b, n;

                a = b = Symbols->Last->Next;

                do
                        {
                                n = a->Next;

                                if ( SYMBOL(a)->App == App )
                                {
                                        ListRemoveItem(Symbols, a);
                                        if ( a == b ) b = Symbols->Last->Next;
                                }

                                a = n;
                        } while ( a != b );
        }


        App->State = DynLdStateToFree; // App will be free by DYNLDTCP Thread


}
////////////////////////////////////////////////////////////////////////////////
PTask DynLdTCPId = 0;

extern l_bool GSExit;
////////////////////////////////////////////////////////////////////////////////
void DynLdTCP ( PTask o ) {

        if ( Applications->Last )
        {
                PListItem a = Applications->Last;
                PListItem n;
                PApplication App;

                do
                {
                        n = a->Next;
                        App = (PApplication)(a->Data);
                        if ( App->State == DynLdStateToFree )
                        {
                                if ( App->Libs ) UnLoadLibraries(App->Libs);
                                ListRemoveItem(Applications, a);

                        }
                        if ( (App->State == DYNLD_STRATING) && GSExit ) {
                                BreakApp(App);
                        }

                        a = n;
                } while ( a != Applications->Last );
        }




}
////////////////////////////////////////////////////////////////////////////////
l_bool DynLdInstallLibrary ( l_text file ) {
        l_text key,n;
        TDynLdHeader DynHead;
        PFile f = FileOpen(file, "rb");

        if ( !f )
        {
                DebugError("DYNLD :: File not found - %s", file);
                return false;
        }

        FileRead(&DynHead, 1, sizeof(TDynLdHeader), f);
        if ( DynHead.Magic != ULONG_ID('D','n','L','d') )
        {
                FileClose(f);
                DebugError("DYNLD :: Library is not a valid DynLD. Library not installed.");
                return false;
        }

        if ( DynHead.FileFormatVersion > DYNLDVERSION )
        {
                FileClose(f);
                DebugError("DYNLD :: Library has invalid version number of DynLD linker. Library not installed.");
                return false;
        }

        FileClose(f);

        key = TextArgs("/SYSTEM/LIBRARIES/%s",DynHead.UID);
        if ( !key ) return false;
        n       = TextArgs("%s",DynHead.UID);
        if ( !n ) return false;

        NewKey("/SYSTEM/LIBRARIES",n);

        KeySetText(key,file);

        free(key);
        free(n);


        return true;
}
////////////////////////////////////////////////////////////////////////////////
void _ParseArgs ( l_text Args, l_text Initial, int *argc, l_text **argv ) {
        l_text Buffer[256];
        l_text p = Args;
        l_int n = 0, g =0;

        if ( Initial )
                Buffer[n++] = TextDup(Initial);

        if ( Args ) {
                while ( *Args ) {
                        if ( *Args == '"' ) {
                                if ( g ) if ( Args-p ) Buffer[n++] = TextNDup(p,Args-p);
                                p = Args+1;
                                g ^= 1;
                        }

                        if ( ( *Args == ' ' ) && !g ) {
                                if ( Args-p ) Buffer[n++] = TextNDup(p,Args-p);
                                p = Args+1;
                        }
                        Args++;
                }

                if ( Args-p ) Buffer[n++] = TextNDup(p,Args-p);
        }

        if ( argc && argv ) {
                Buffer[n] = 0;
                *argc = n;
                *argv = malloc((n+1)*sizeof(l_text));
                memcpy(*argv,Buffer,(n+1)*sizeof(l_text));
        }

}
////////////////////////////////////////////////////////////////////////////////
l_text *DuplicateArgs (  int argc, l_text *argv ) {
        l_int n = 0;
        l_text *nargv = malloc(sizeof(l_text)*(argc+1));
        if ( !nargv ) return NULL;
        while ( n < argc ) {
                nargv[n] = TextDup(argv[n]);
                n++;
        }
        nargv[argc] = 0;
        return nargv;

}
////////////////////////////////////////////////////////////////////////////////
void FreeArgs (  int argc, l_text *argv ) {
        l_int n = 0;
        while ( n < argc ) {
                free(argv[n]);
                n++;
        }
        free(argv);
}
////////////////////////////////////////////////////////////////////////////////
void RegistryLoadLibraries ( l_text Name ) {
        PRegKey o =     ResolveKey(Name);
        if ( !o ) return;
        DebugMessage("Run registry entries form %s",Name);
        if ( o->Last ) {
                PRegKey a = o->Last->Next;
                PRegKey b = a;
                l_uid id;
                do {
                        memset(&id,0,sizeof(l_uid));
                        TextCopy((l_text)&id,a->Name);
                        LoadLibrary(id);
                        a = a->Next;
                } while ( a != b );
        }
}
////////////////////////////////////////////////////////////////////////////////
void InitDynLd ( void )
{
        Symbols = NewList();

        if ( !Symbols ) DebugFatal("DynLd::ERROR_NOTENOUGHMEMORY");

        Applications = NewList();

        if ( !Applications ) DebugFatal("DynLd::ERROR_NOTENOUGHMEMORY");

        AppExtenders = NewList();

        if ( !AppExtenders ) DebugFatal("DynLd::ERROR_NOTENOUGHMEMORY");

        SYSEXPORT(AddSymbol);
        SYSEXPORT(CloseApp);
        SYSEXPORT(ResolveSymbol);
        SYSEXPORT(DynLdInstallLibrary);

        SYSEXPORT(InstallAppExtender);
        SYSEXPORT(RemoveAppExtender);

        SYSEXPORT(_ParseArgs);
        SYSEXPORT(FreeArgs);
        SYSEXPORT(BreakApp);

        SYSEXPORTAS (Applications, "DynLdApplications");
        SYSEXPORT(DynLdRun);
        SYSEXPORT(DynLdRun2);
        SYSEXPORT(DynLdRunEx);
        SYSEXPORT(DynLdCommand);

        /* ** Due to unknown threading bugs, this section is unactivated **

        SYSEXPORT(AppSendEvent);
        SYSEXPORT(AppSendEventEv);
        SYSEXPORT(AppGetNextEvent);
        SYSEXPORT(AppRedirectEvent);*/

        DynLdTCPId = InstallTask(DynLdTCP);

}

void ShutDownDynLd ( void )
{
        DebugMessage ("Shutting down DynLd...");

        RemoveTask(DynLdTCPId);


        if ( Applications->Last )
        {
                PListItem a = Applications->Last;
                PListItem b = a;
                PApplication App;

                do
                {
                        App = (PApplication)(a->Data);

                        /* ** Due to unknown threading bugs, this section is unactivated **

                        if ( App->Type == DYNLD_TYPEAPP )
                        {
                                DebugMessage("Kill thread :: 0x%08x : %s : %d", App, App->Name, App->ThreadId);
                                AppSendEvent(App, EV_MESSAGE, MSG_KILLAPP, NULL, NULL, NULL);

                                while ( App->State != DynLdStateDied ) Yield();

                                KillThread(App->ThreadId);

                                App->ThreadId = 0;
                                App->Close = NULL;
                        }
                        else */
                        if ( App->Close ) {
                                DebugMessage ("Close :: %s",App->Name);

                                App->Close();
                                App->Close = NULL;

                        }
                                DebugMessage("AppExtendersDeInit");
                        AppExtendersDeInit(App);

                        a = a->Prev;
                } while ( a != b );
        }

        FreeList(Symbols);
        FreeList(Applications);
        FreeList(AppExtenders);

        DebugMessage ("DynLd Stopped");
}
////////////////////////////////////////////////////////////////////////////////
