////////////////////////////////////////////////////////////////////////////////
//
//      Data Management System V2 - Core file
//
//      (c) Copyright 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define DMS2

#include"kernel.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
l_bool  BinarySave2( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *ExtraSave, PErrorHandler h ) {
        if ( FileWrite(BINDATA(Data)->Data,BINDATA(Data)->Size,1,file) != (l_int)BINDATA(Data)->Size ) return false;
        if ( EndOffset ) FileGetPos(file,EndOffset);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  BinaryLoad2( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *ExtraLoad, PErrorHandler h ) {
        l_ulong Size;
        void *Dat;
        if ( EndOffset ) {
                l_ulong Start;
                FileGetPos(file,&Start);
                Size = (*EndOffset)-Start;
        } else
                Size = FileLength(file);
        Dat = malloc(Size);
        if ( !Dat ) return false;
        if ( FileRead(Dat,Size,1,file) != (l_int)Size ) return false;
        *Data = NewBinData(Dat,Size);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  TextSave2( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *ExtraSave, PErrorHandler h ) {
        l_ulong Size = TextLen((l_text)Data);
        if ( FileWrite(Data,Size,1,file) != (l_int)Size ) return false;
        if ( EndOffset ) FileGetPos(file,EndOffset);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  TextLoad2( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *ExtraLoad, PErrorHandler h ) {
        l_ulong Size;
        void *Dat;
        if ( EndOffset ) {
                l_ulong Start;
                FileGetPos(file,&Start);
                Size = (*EndOffset)-Start;
        } else
                Size = FileLength(file);
        Dat = malloc(Size+1);
        if ( !Dat ) return false;
        memset(Dat,0,Size+1);
        if ( FileRead(Dat,Size,1,file) != (l_int)Size ) return false;
        *Data = Dat;
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  UTextSave2( PFile file, l_ptr  Data, l_ulong *EndOffset, l_ulong *ExtraSave, PErrorHandler h ) {
        l_ulong Size = UTextLen((l_utext)Data)*2;
        if ( FileWrite(Data,Size,1,file) != (l_int)Size ) return false;
        if ( EndOffset ) FileGetPos(file,EndOffset);
        return true;
}
////////////////////////////////////////////////////////////////////////////////
l_bool  UTextLoad2( PFile file, l_ptr *Data, l_ulong *EndOffset, l_ulong *ExtraLoad, PErrorHandler h ) {
        l_ulong Size;
        void *Dat;
        if ( EndOffset ) {
                l_ulong Start;
                FileGetPos(file,&Start);
                Size = (*EndOffset)-Start;
        } else
                Size = FileLength(file);
        Dat = malloc(Size+2);
        if ( !Dat ) return false;
        memset(Dat,0,Size+2);
        if ( FileRead(Dat,Size,1,file) != (l_int)Size ) return false;
        *Data = Dat;
        return true;
}
////////////////////////////////////////////////////////////////////////////////
void InitGenericSubCodecs ( void ) {

        GenericText = NewSubCodec(DTYPE_TEXT,0,0,0,0,&TextSave2,&TextLoad2,0,0,0,0);
        GenericUText = NewSubCodec(DTYPE_UTEXT,0,0,0,0,&UTextSave2,&UTextLoad2,0,0,0,0);
        GenericBinary = NewSubCodec(DTYPE_BINDATA,0,0,0,0,&BinarySave2,&BinaryLoad2,0,0,0,0);

}
