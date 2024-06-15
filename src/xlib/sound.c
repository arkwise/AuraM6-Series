////////////////////////////////////////////////////////////////////////////////
//
//	Sound system
//
//	Copyright (c) 2004 Point Mad. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include"kernel.h"
#include"sound.h"
////////////////////////////////////////////////////////////////////////////////
l_uid 	nUID 		= "sound";
l_ulong AppVersion 	= ULONG_ID(0,0,0,0);
char    AppName[] 	= "Sound System";
l_uid NeededLibs[] 	= { "" };

l_int VolumeDigi = 255, VolumeMidi = 255;
////////////////////////////////////////////////////////////////////////////////
PList Playing = NULL;
PTask STask = NULL;
////////////////////////////////////////////////////////////////////////////////
// Allegro imported functions
////////////////////////////////////////////////////////////////////////////////
inline void *GetAudioStreamBuffer( PSStream stream ) {
	int pos;
	char *data = NULL;
	if (stream->bufnum == stream->active * stream->bufcount) {
		pos = voice_get_position(stream->voice);
		if (stream->active == 0) {
			if ( pos < stream->len*stream->bufcount ) return NULL;
		} else {
			if ( pos >= stream->len*stream->bufcount ) return NULL;
		}
		stream->active = 1-stream->active;
	}
	
	if (!stream->locked) {
		pos = (1-stream->active) * stream->bufcount * stream->len;
		if ( digi_driver->lock_voice )
			data = digi_driver->lock_voice(stream->voice, pos, pos+stream->bufcount*stream->len);
		if ( data ) 
			stream->locked = data;
		else
			stream->locked = (char *)stream->samp->data + (pos * ((stream->samp->bits==8) ? 1 : sizeof(short)) * ((stream->samp->stereo) ? 2 : 1));
	}
	
	return (char *)stream->locked + ((stream->bufnum % stream->bufcount) *
		stream->len * ((stream->samp->bits==8) ? 1 : sizeof(short)) *
		((stream->samp->stereo) ? 2 : 1));
}
////////////////////////////////////////////////////////////////////////////////
inline void FreeAudioStreamBuffer( PSStream stream) {
	stream->bufnum++;
	if (stream->bufnum >= stream->bufcount*2) stream->bufnum = 0;
	if ((stream->locked) && ((stream->bufnum == 0) || (stream->bufnum == stream->bufcount))) {
		if (digi_driver->unlock_voice) digi_driver->unlock_voice(stream->voice);
		stream->locked = NULL;
	}
	if (voice_get_position(stream->voice) == -1) voice_start(stream->voice);
}
////////////////////////////////////////////////////////////////////////////////
void _PauseSound ( PSStream o )  {
	if ( !o->Playing ) return;
	if ((o->locked) && (digi_driver->unlock_voice)) {
		digi_driver->unlock_voice(o->voice);
	}
	o->locked = NULL;
	o->Playing = false;
	deallocate_voice(o->voice);
	DebugMessage("Pause Stream %d", o);
} 
////////////////////////////////////////////////////////////////////////////////
l_bool _PlaySound ( PSStream o )  {
	
	if ( o->Playing ) return false;
	o->Playing = true;
	o->locked = NULL;
	o->bufnum = 0;
	o->active = 1;
	o->voice = allocate_voice(o->samp);
	if (o->voice < 0) return false;
	voice_set_playmode(o->voice, PLAYMODE_LOOP);
	DebugMessage("Playing Stream %d", o);
	return true;
} 
////////////////////////////////////////////////////////////////////////////////
// Our global sound stream polling task
////////////////////////////////////////////////////////////////////////////////
void SoundTask ( PTask o )  {
	PListItem a, n;
	void*Data;
	l_int r;
	if ( !Playing->Last ) return;
	a = Playing->Last;
	do {
		n = a->Next;
		if ( Data = GetAudioStreamBuffer(SSTREAM(a->Data)) ) {
			if ( r = SSTREAM(a->Data)->Driver->ExtractSample(SSTREAM(a->Data),Data) ) {
				FreeAudioStreamBuffer(SSTREAM(a->Data));
				if ( r == 2 ) {
					_PauseSound(SSTREAM(a->Data));
					SSTREAM(a->Data)->Finished = true;
					ListRemoveItem(Playing,a);
				}
			} else {
				_PauseSound(SSTREAM(a->Data));
				SSTREAM(a->Data)->Finished = true;
				ListRemoveItem(Playing,a);
			}
		}
		a = n;
	} while ( a != Playing->Last && Playing->Last );
}
////////////////////////////////////////////////////////////////////////////////
// Play/Pause stream functions
////////////////////////////////////////////////////////////////////////////////
l_bool PlaySoundStream( PSStream o ) {
	if ( !ListFoundItem(Playing,o) ) {
		ListAdd(Playing,NULL,o,NULL);
		_PlaySound(o);
		return true;
	} else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
l_bool PauseSoundStream( PSStream o ) {
	PListItem a = ListFoundItem(Playing,o);
	if ( a ) {
		ListRemoveItem(Playing,a);
		_PauseSound(o);
		return true;
	} else
		return false;
}
////////////////////////////////////////////////////////////////////////////////
// SStream creation/free
////////////////////////////////////////////////////////////////////////////////
PSStream NewSoundStream ( PSStreamDriver Drv, PFile f, void *Ex, l_int len, l_int bits, l_int stereo, l_int freq ) {
	
   PSStream stream;
   int i, bufcount;

   if ((digi_driver) && (digi_driver->buffer_size))
      i = digi_driver->buffer_size();
   else
      i = 2048;

   if (len >= i)
      bufcount = 1;
   else
      bufcount = (i + len-1) / len;

   stream = malloc(sizeof(TSStream));
   if (!stream)
      return NULL;

   stream->Driver 	= Drv;
   stream->f 		= f;
   stream->Ex 		= Ex;
   stream->Playing 	= false;
   stream->Finished = false;
   stream->len = len;
   stream->bufcount = bufcount;
   stream->bufnum = 0;
   stream->active = 1;
   stream->locked = NULL;
   
   stream->samp = create_sample(bits, stereo, freq, len*bufcount*2);

   if (!stream->samp) {
      free(stream);
      return NULL;
   }

   if (bits == 16) {
      unsigned short *p = stream->samp->data;
      for (i=0; i < len*bufcount*2 * ((stereo) ? 2 : 1); i++)
	 p[i] = 0x8000;
   }
   else {
      unsigned char *p = stream->samp->data;
      for (i=0; i < len*bufcount*2 * ((stereo) ? 2 : 1); i++)
	 p[i] = 0x80;
   }

   return stream;
}
////////////////////////////////////////////////////////////////////////////////
l_bool SStreamFree( PSStream o ) {
	if ( o->Playing ) PauseSoundStream(o);
	if ( !o->Driver->Free(o) ) 
		return false;
	destroy_sample(o->samp);
	free(o);
}
////////////////////////////////////////////////////////////////////////////////
// Stream position functions
////////////////////////////////////////////////////////////////////////////////
l_bool SStreamGetPosMsec( PSStream o, l_ulong *msec ) {
	return o->Driver->GetPosMsec(o,msec);
}
////////////////////////////////////////////////////////////////////////////////
l_bool SStreamGetLengthMsec( PSStream o, l_ulong *msec ) {
	return o->Driver->GetLengthMsec(o,msec);
}
////////////////////////////////////////////////////////////////////////////////
l_bool SStreamSetPosMsec( PSStream o, l_ulong msec ) {
	o->Finished = false;
	return o->Driver->SetPosMsec(o,msec);
}
////////////////////////////////////////////////////////////////////////////////
void SetVolumeDigi ( l_int Volume ) {
	VolumeDigi = Volume;
	KeySetInt("/USER/SOUND/VOLUMEDIGI",VolumeDigi);
	set_volume(VolumeDigi,VolumeMidi);
}
////////////////////////////////////////////////////////////////////////////////
void SetVolumeMidi ( l_int Volume ) {
	VolumeMidi = Volume;
	KeySetInt("/USER/SOUND/VOLUMEMIDI",VolumeMidi);
	set_volume(VolumeDigi,VolumeMidi);
}
////////////////////////////////////////////////////////////////////////////////
l_int LibMain( int argc, l_text *argv )
{
	SYSEXPORT(NewSoundStream);
	SYSEXPORT(PlaySoundStream);
	SYSEXPORT(PauseSoundStream);
	SYSEXPORT(SStreamGetPosMsec);
	SYSEXPORT(SStreamGetLengthMsec);
	SYSEXPORT(SStreamSetPosMsec);
	SYSEXPORT(SStreamFree);
	SYSEXPORT(VolumeDigi);
	SYSEXPORT(VolumeMidi);
	SYSEXPORT(SetVolumeDigi);
	SYSEXPORT(SetVolumeMidi);
	VolumeDigi = KeyGetInt("/USER/SOUND/VOLUMEDIGI",255);
	VolumeMidi = KeyGetInt("/USER/SOUND/VOLUMEMIDI",255);
	
	DebugMessage("Installing sound system...");
	Playing = NewList();
	STask = InstallTask(SoundTask);
	reserve_voices (32, -1);
	DebugMessage("Try to initialise hardware");
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") == -1) {
		DebugError("Unable to install sound");
		DebugMessage("Sound library still active");
		return true;
	}	
	set_volume(VolumeDigi,VolumeMidi);
	DebugMessage("Volume set to %d",VolumeDigi);
	DebugMessage("Midi Volume set to %d", VolumeMidi);
	DebugMessage("Sound system installed");
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void Close(void)
{
	if ( STask ) RemoveTask(STask);
	if ( Playing ) FreeList(Playing);
}
////////////////////////////////////////////////////////////////////////////////
