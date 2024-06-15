
#ifndef _SOUND_H_INCLUDED_
#define _SOUND_H_INCLUDED_

typedef struct TSStreamDriver *PSStreamDriver;
typedef struct TSStream *PSStream;


typedef struct TSStreamDriver {
	
	l_ulong Codec; // DMS linked codec
	
	l_int (*ExtractSample) ( PSStream s, void *Buffer );
	
	l_bool (*SetPosMsec) ( PSStream s, l_ulong msec );
	
	l_bool (*GetPosMsec) ( PSStream s, l_ulong *msec );
	l_bool (*GetLengthMsec) ( PSStream s, l_ulong *msec );
	
	l_bool (*Free) ( PSStream s );
	
} TSStreamDriver;


typedef struct TSStream {
	
	PSStreamDriver Driver;
	
	PFile f;
	
	void *Ex;
	
	l_bool Finished;
	l_bool Playing;
	
	// Allegro AudioStream
	
	l_int voice;                        /* the voice we are playing on */
	struct SAMPLE *samp;                /* the sample we are using */
	l_int len;                          /* buffer length */
	l_int bufcount;                     /* number of buffers per sample half */
	l_int bufnum;                       /* current refill buffer */
	l_int active;                       /* which half is currently playing */
	void *locked;                       /* the locked buffer */
	
} TSStream;

#define SSTREAM(o) ((PSStream)(o))

PSStream NewSoundStream ( PSStreamDriver Drv, PFile f, void *Ex, l_int len, l_int bits, l_int stereo, l_int freq );

l_bool PlaySoundStream( PSStream o );
l_bool PauseSoundStream( PSStream o );

l_bool SStreamGetPosMsec( PSStream o, l_ulong *msec );
l_bool SStreamGetLengthMsec( PSStream o, l_ulong *msec );

l_bool SStreamSetPosMsec( PSStream o, l_ulong msec );

l_bool SStreamFree( PSStream o ); 

void SetVolumeMidi ( l_int Volume );
void SetVolumeDigi ( l_int Volume );

extern l_int VolumeDigi, VolumeMidi;

#endif
