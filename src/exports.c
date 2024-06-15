#define KERNEL_FILE
#include "kernel.h"
#include "internal.h"
#include "zlib.h"
#include "grfx.h"
#include "stdlib.h"
#include "debug.h"
#include "mpeg2.h"

l_ubig __udivdi3(l_ubig x, l_ubig y);

int ____registered_allegro      = 0;
int ____registered_libc         = 0;

void RegisterAllegro();
void RegisterLibC();

void RegisterExports()
{
        RegisterAllegro();
        RegisterLibC();
        /**
        *       Kernel information
        */
        SYSEXPORT(KernelName);
        SYSEXPORT(KernelCopyright);
        SYSEXPORT(KernelVersion);
        SYSEXPORT(KernelTextVersion);

        SYSEXPORT(Platform);

        SYSEXPORT(StartRect);
        SYSEXPORT(StartLogo);

        SYSEXPORT(KeyboardSetLayout);
        SYSEXPORT(KeyboardGetLayout);
        SYSEXPORT(KeyboardInstallLayout);
        SYSEXPORT(__KeyboardStop);
        SYSEXPORT(__KeyboardRestart);

        /**
        *       VFS
        */
        SYSEXPORT(ListDir);
        SYSEXPORT(FileOpen);
        SYSEXPORT(FileClose);
        SYSEXPORT(FileRead);
        SYSEXPORT(FileWrite);
        SYSEXPORT(FileSetPos);
        SYSEXPORT(FileSeek);
        SYSEXPORT(FileGetPos);
        SYSEXPORT(FileLength);
        SYSEXPORT(FileWriteBinString);
        SYSEXPORT(FileReadBinString);
        SYSEXPORT(FileWriteTextString);
        SYSEXPORT(FilePrintf);
        SYSEXPORT(file_read_char);
        SYSEXPORT(file_read_short);
        SYSEXPORT(file_read_long);
        SYSEXPORT(file_write_char);
        SYSEXPORT(file_write_short);
        SYSEXPORT(file_write_long);
        SYSEXPORT(file_get_char);
        SYSEXPORT(file_get_mlong);
        SYSEXPORT(ReadString);
        SYSEXPORT(WriteString);
        SYSEXPORT(FileNameToPath);
        SYSEXPORT(GetPathParent);
        SYSEXPORT(FileGetInfo);
        SYSEXPORT(FreeFileInfo);
        SYSEXPORT(FileSetAttributes);
        SYSEXPORT(AddFileInfoToList);
        SYSEXPORT(AddDrive);
        SYSEXPORT(FileOpen2);
        SYSEXPORT(FreeFileRef);
        SYSEXPORT(NewFileReference);
        SYSEXPORT(ListDir2);
        SYSEXPORT(_GenerateFileRefs);
        SYSEXPORT(_GenerateFileRefs2);
        SYSEXPORT(FileGets);
        SYSEXPORT(RemoveDrive);

        /**
        *       Debugging system
        */
        SYSEXPORT(DebugMessage);
        SYSEXPORT(DebugWarning);
        SYSEXPORT(DebugError);
        SYSEXPORT(DebugFatal);
        SYSEXPORT(DebugCheckPtr);

        /**
        *       Fonts
        */
        SYSEXPORT(FontLoad);
        SYSEXPORT(default_font);

        /**
        *       Metrics
        */
        SYSEXPORT(PointAssign);
        SYSEXPORT(PointInRect);
        SYSEXPORT(RectAssign);
        SYSEXPORT(RectMove);
        SYSEXPORT(GetXDelta);
        SYSEXPORT(GetYDelta);
        SYSEXPORT(GetWidth);
        SYSEXPORT(GetHeight);
        //SYSEXPORT(SizeAssign);
        SYSEXPORT(RectOverlay);
        SYSEXPORT(RemoveZoneInList);
        SYSEXPORT(RemoveZoneInListV);
        SYSEXPORT(NewListFromRect);
        SYSEXPORT(RectIntersept);
        SYSEXPORT(NewListFromRectList);
        SYSEXPORT(AddZoneInList);
        SYSEXPORT(AddZoneInListEx);
        SYSEXPORT(NewListFromRectListIntersept);
        SYSEXPORT(RectOverlayList);


        /**
        *       INI file support
        */
        SYSEXPORT(INIGet);
        SYSEXPORT(INISectionExists);
        SYSEXPORT(INIGetInteger);
        SYSEXPORT(RegistryGetKey);
        SYSEXPORT(RegistryGetInteger);
        SYSEXPORT(INIGetText);
        SYSEXPORT(INIGetNew);
        SYSEXPORT(INIGetColor);

        /**
        *       Image support
        */
        SYSEXPORT(LoadImage);
        SYSEXPORT(DestroyImage);

        /**
        *       Event system
        */
        SYSEXPORT(EventGenerators);
        SYSEXPORT(EventHandlers);
        SYSEXPORT(InstallEventGenerator);
        SYSEXPORT(InstallEventHandler);
        SYSEXPORT(RemoveEventHandler);
        SYSEXPORT(EventAssign);
        SYSEXPORT(NewEvent);
        SYSEXPORT(GenerateEvent);
        SYSEXPORT(HandleEvent);
        SYSEXPORT(PollEvents);
        SYSEXPORT(KState);
        SYSEXPORT(KeyboardEventGenerator);

        /**
        *       Tasks support
        */
        SYSEXPORT(InstallTask);
        SYSEXPORT(RemoveTask);
        SYSEXPORT(TaskList);
        SYSEXPORT(PollTasks);

        /**
        *       Internal timer
        */
        SYSEXPORT(TimersPoll);

        /**
        *       Mouse
        */
        SYSEXPORT(cursor_pointer);
        SYSEXPORT(cursor_resize);
        SYSEXPORT(cursor_clock);
        SYSEXPORT(cursor_pencil);
        SYSEXPORT(_MouseRecap);
        SYSEXPORT(Mouse);
        SYSEXPORT(MouseShow);
        SYSEXPORT(MouseHide);
        SYSEXPORT(MouseSetCursor);
        SYSEXPORT(MouseSetCursorSpeed);
        SYSEXPORT(MouseSetDblClkSpeed);
        SYSEXPORT(__MouseStop);
        SYSEXPORT(__MouseRestart);
        SYSEXPORT(NewCursor);

        SYSEXPORT(Cursors);
        SYSEXPORT(MouseSetCursorNamed);


        /**
        *       ADT - List
        */
        SYSEXPORT(KickList);

        SYSEXPORT(FreeList);
        SYSEXPORT(NewList);
        SYSEXPORT(ListAdd);
        SYSEXPORT(ListFoundItem);
        SYSEXPORT(ListRemoveItem);
        SYSEXPORT(ListAddItem);
        SYSEXPORT(ListKeyItem);

        SYSEXPORT(ListAt);
        SYSEXPORT(ListAtItem);
        SYSEXPORT(ListFoundIndex);
        SYSEXPORT(ListKey);
        SYSEXPORT(ListKeyCase);
        SYSEXPORT(NewListItem);
        SYSEXPORT(NewListItemEx);
        SYSEXPORT(ListOrderByData);
        SYSEXPORT(ListKeyCaseItem);
        SYSEXPORT(ListAddItemBefore);
        SYSEXPORT(ListRemoveItemEx);

        /**
        *       Screen Exports
        */
        SYSEXPORT(GSScreenWidth);
        SYSEXPORT(GSScreenHeight);
        SYSEXPORT(GSScreenDepth);
        SYSEXPORT(GSScreenRefreshRate);
        SYSEXPORT(__ScreenRestartGraphics);
        SYSEXPORT(__ScreenTextMode);

        SYSEXPORTAS(Me,"SystemApp");
        SYSEXPORT(TimerSetPeriod);

        /**
        *       Text
        */
        SYSEXPORT(CommonBufferText);


        /**
        *       Sound
        */
        SYSEXPORT(digi_driver);
        SYSEXPORT(voice_get_position);
        SYSEXPORT(voice_start);
        SYSEXPORT(reserve_voices);
        SYSEXPORT(install_sound);
        SYSEXPORT(create_sample);
        SYSEXPORT(allocate_voice);
        SYSEXPORT(voice_set_playmode);
        SYSEXPORT(destroy_sample);
        SYSEXPORT(deallocate_voice);
        SYSEXPORT(voice_stop);
        SYSEXPORT(set_volume);

        SYSEXPORT(RectUnMove);
        SYSEXPORT(MakeDir);
        SYSEXPORT(FileEOF);
        SYSEXPORT(FileVPathName);
        SYSEXPORT(FileRename);
        SYSEXPORT(DirRename);
        SYSEXPORT(ResolveFileName);
        SYSEXPORT(RelativeName);

        SYSEXPORT(inflateReset);
        SYSEXPORT(inflate);
        SYSEXPORT(crc32);
        SYSEXPORT(inflateEnd);
        SYSEXPORT(inflateInit_);

        SYSEXPORT(deflateInit2_);
        SYSEXPORT(deflate);
        SYSEXPORT(deflateReset);
        SYSEXPORT(deflateEnd);

        APPEXPORT(draw_sprite_greyscale);
        APPEXPORT(draw_sprite_tile);
        APPEXPORT(hsv_to_rgb);
        APPEXPORT(rgb_to_hsv);


        APPEXPORT(CreateError);
        APPEXPORT(ErrorLogEx);
        APPEXPORT(ErrorLog);
        APPEXPORT(CreateErrorHandler);
        APPEXPORT(FreeErrorHandler);
        /*
        #ifdef LWP_THREAD
                SYSEXPORTAS(lwp_yield, "Yield");
                SYSEXPORTAS(lwp_spawn, "NewThread");
                SYSEXPORTAS(lwp_kill, "KillThread");
                SYSEXPORTAS(lwp_getpid, "ThreadId");
                SYSEXPORTAS(lwp_thread_enable, "UnPauseMultitasking");
                SYSEXPORTAS(lwp_thread_disable, "PauseMultitasking");
        #else
                SYSEXPORT(Yield);
                SYSEXPORT(NewThread);
                SYSEXPORT(KillThread);
                SYSEXPORT(ThreadId);
                SYSEXPORT(UnPauseMultitasking);
                SYSEXPORT(PauseMultitasking);
        #endif
        */
}

#include <allegro/internal/aintern.h>

void RegisterAllegro()
{
        if (!____registered_allegro)
        {
                ____registered_allegro = 1;

                SYSEXPORT(makeacol);

                SYSEXPORT(create_zbuffer);
                SYSEXPORT(set_zbuffer);
                SYSEXPORT(destroy_zbuffer);

                SYSEXPORT(triangle3d_f);
                SYSEXPORT(get_rotation_matrix_f);
                SYSEXPORT(apply_matrix_f);
                SYSEXPORT(get_translation_matrix_f);
                SYSEXPORT(matrix_mul_f);
                SYSEXPORT(identity_matrix_f);
                SYSEXPORT(set_projection_viewport);
                SYSEXPORT(quad3d_f);
                SYSEXPORT(quad3d);
                SYSEXPORT(allegro_404_char);
                SYSEXPORT(getr);
                SYSEXPORT(getg);
                SYSEXPORT(getb);
                SYSEXPORT(screen);
                SYSEXPORT(rectfill);
                SYSEXPORT(line);
                SYSEXPORT(rect);
                SYSEXPORT(font);
                SYSEXPORT(draw_sprite);
                SYSEXPORT(draw_trans_sprite);
                SYSEXPORT(create_bitmap);
                SYSEXPORT(create_bitmap_ex);
                SYSEXPORT(triangle);
                SYSEXPORT(hline);
                SYSEXPORT(vline);
                SYSEXPORT(putpixel);
                SYSEXPORT(floodfill);
                SYSEXPORT(text_height);
                SYSEXPORT(circle);
                SYSEXPORT(set_clip_rect);
                SYSEXPORT(destroy_bitmap);
                SYSEXPORT(stretch_sprite);
                SYSEXPORT(textout_ex);
                SYSEXPORT(textout_centre_ex);
                SYSEXPORT(textout_justify_ex);
                SYSEXPORT(textout_right_ex);
                SYSEXPORT(text_length);
                SYSEXPORT(getpixel);
                SYSEXPORT(ellipse);
                SYSEXPORT(makecol);
                SYSEXPORT(clear_bitmap);
                SYSEXPORT(clear_to_color);
                SYSEXPORT(clear);
                SYSEXPORT(blit);
                SYSEXPORT(masked_blit);
                SYSEXPORT(stretch_blit);
                SYSEXPORT(masked_stretch_blit);
                SYSEXPORT(bitmap_color_depth);
                SYSEXPORT(set_mouse_range);
                SYSEXPORT(mouse_x);
                SYSEXPORT(mouse_x_focus);
                SYSEXPORT(mouse_y);
                SYSEXPORT(mouse_y_focus);
                SYSEXPORT(mouse_sprite);
                SYSEXPORT(create_system_bitmap);
                SYSEXPORT(xor_mode);
                SYSEXPORT(_fixup_loaded_bitmap);
                SYSEXPORT(_color_load_depth);
                SYSEXPORT(generate_332_palette);
                SYSEXPORT(get_palette);
                SYSEXPORT(getb_depth);
                SYSEXPORT(getg_depth);
                SYSEXPORT(getr_depth);
                SYSEXPORT(allegro_errno);
                SYSEXPORT(_rgb_scale_6);
                SYSEXPORT(_rgb_r_shift_24);
                SYSEXPORT(_rgb_g_shift_24);
                SYSEXPORT(_rgb_b_shift_24);
                SYSEXPORT(set_trans_blender);
                SYSEXPORT(textprintf_ex);
                SYSEXPORT(set_mouse_speed);
                SYSEXPORT(drawing_mode);
                SYSEXPORT(solid_mode);
                SYSEXPORT(circlefill);

        #ifndef _OZONE__LINUX_
                SYSEXPORT(_key_ascii_table);
                SYSEXPORT(_key_capslock_table);
                SYSEXPORT(_key_shift_table);
                SYSEXPORT(_key_control_table);
                SYSEXPORT(_key_altgr_lower_table);
                SYSEXPORT(_key_altgr_upper_table);
                SYSEXPORT(_key_accent1_lower_table);
                SYSEXPORT(_key_accent1_upper_table);
                SYSEXPORT(_key_accent2_lower_table);
                SYSEXPORT(_key_accent2_upper_table);
                SYSEXPORT(_key_accent3_lower_table);
                SYSEXPORT(_key_accent3_upper_table);
                SYSEXPORT(_key_accent4_lower_table);
                SYSEXPORT(_key_accent4_upper_table);
                SYSEXPORT(_key_accent1);
                SYSEXPORT(_key_accent2);
                SYSEXPORT(_key_accent3);
                SYSEXPORT(_key_accent4);
                SYSEXPORT(_key_accent1_flag);
                SYSEXPORT(_key_accent2_flag);
                SYSEXPORT(_key_accent3_flag);
                SYSEXPORT(_key_accent4_flag);
                SYSEXPORT(_key_standard_kb);
        #endif

                SYSEXPORT(get_config_int);
                SYSEXPORT(uconvert);
                SYSEXPORT(uszprintf);
                SYSEXPORT(set_config_data);
                SYSEXPORT(get_config_string);
                SYSEXPORT(_rgb_r_shift_32);
                SYSEXPORT(_rgb_g_shift_32);
                SYSEXPORT(_rgb_b_shift_32);
                SYSEXPORT(_rgb_a_shift_32);
                SYSEXPORT(_rgb_r_shift_16);
                SYSEXPORT(_rgb_g_shift_16);
                SYSEXPORT(_rgb_b_shift_16);
                SYSEXPORT(_rgb_r_shift_15);
                SYSEXPORT(_rgb_g_shift_15);
                SYSEXPORT(_rgb_b_shift_15);
                SYSEXPORT(_rgb_scale_5);
                SYSEXPORT(makecol_depth);
#if ALLEGRO_SUB_VERSION == 1
                SYSEXPORT(textout_ex);
                SYSEXPORT(textout_centre_ex);
#endif
                SYSEXPORT(cpu_capabilities);
                SYSEXPORT(select_palette);
                SYSEXPORT(unselect_palette);
                SYSEXPORT(makecol_depth);

                SYSEXPORT(get_rle_sprite);
                SYSEXPORT(destroy_rle_sprite);
                SYSEXPORT(draw_rle_sprite);
                SYSEXPORT(draw_lit_rle_sprite);


        }
}

/**
 * Linux LIBC
 */
#ifdef _OZONE__LINUX_
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
void RegisterLibC()
{
        SYSEXPORT(exp);
        SYSEXPORT(free);
        SYSEXPORT(malloc);
        SYSEXPORT(realloc);
        SYSEXPORT(strcmp);
        SYSEXPORT(stricmp);
        SYSEXPORT(strcat);
        SYSEXPORT(strlen);
        SYSEXPORT(strdup);
        SYSEXPORT(strcpy);
        SYSEXPORT(fopen);
        SYSEXPORT(fclose);
        SYSEXPORT(fseek);
        SYSEXPORT(fwrite);
        SYSEXPORT(fprintf);
        SYSEXPORT(fgets);
        SYSEXPORT(feof);
        SYSEXPORT(fread);
        SYSEXPORT(fsetpos);
        SYSEXPORT(fgetpos);
        SYSEXPORT(sprintf);
        SYSEXPORT(vsprintf);
        SYSEXPORT(ctime);
        SYSEXPORT(atof);
        SYSEXPORT(atoi);
        SYSEXPORT(sin);
        SYSEXPORT(cos);
        SYSEXPORT(rand);
        SYSEXPORT(srand);
        SYSEXPORT(memcpy);
        SYSEXPORTAS(rand,"random");
        SYSEXPORT(time);
        SYSEXPORT(localtime);
        SYSEXPORT(pow);
        SYSEXPORT(memset);
        SYSEXPORT(setjmp);
        SYSEXPORT(longjmp);
        SYSEXPORT(strtod);
        SYSEXPORT(abort);
        SYSEXPORT(gmtime);
        SYSEXPORT(fflush);
        SYSEXPORT(vfprintf);
        SYSEXPORT(fputc);
        SYSEXPORTAS(*stderr,"__dj_stderr");

        SYSEXPORT(stderr);
        SYSEXPORT(_setjmp);
        SYSEXPORT(_longjmp);
        SYSEXPORT(__strtod_internal);
        SYSEXPORT(__strtol_internal);
        SYSEXPORT(__strdup);
        SYSEXPORT(_alemu_stricmp);

        SYSEXPORT(qsort);
        SYSEXPORT(calloc);
        SYSEXPORT(floor);

        SYSEXPORT(tan);
        SYSEXPORT(sqrt);
        SYSEXPORT(exit);
        SYSEXPORT(strtoul);
}
#else


double __strtod_internal (char *__nptr, char ** __endptr, int __group) {
        return strtod(__nptr,__endptr);
}

double __strtol_internal ( char * __nptr,char ** __endptr,int __base, int __group) {
        return strtol(__nptr,__endptr,__base);
}

#endif

#ifdef _OZONE__WIN32_
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include <setjmp.h>
#define __dj_ISALNUM    0x0001
#define __dj_ISALPHA    0x0002
#define __dj_ISCNTRL    0x0004
#define __dj_ISDIGIT    0x0008
#define __dj_ISGRAPH    0x0010
#define __dj_ISLOWER    0x0020
#define __dj_ISPRINT    0x0040
#define __dj_ISPUNCT    0x0080
#define __dj_ISSPACE    0x0100
#define __dj_ISUPPER    0x0200
#define __dj_ISXDIGIT   0x0400
#define __dj_ISBLANK    0x0800
unsigned short __dj_ctype_flags[] = {
  0,                                                                                            /* CTRL+?, 0xffff */
  __dj_ISCNTRL,                                                                                 /* CTRL+@, 0x00 */
  __dj_ISCNTRL,                                                                                 /* CTRL+A, 0x01 */
  __dj_ISCNTRL,                                                                                 /* CTRL+B, 0x02 */
  __dj_ISCNTRL,                                                                                 /* CTRL+C, 0x03 */
  __dj_ISCNTRL,                                                                                 /* CTRL+D, 0x04 */
  __dj_ISCNTRL,                                                                                 /* CTRL+E, 0x05 */
  __dj_ISCNTRL,                                                                                 /* CTRL+F, 0x06 */
  __dj_ISCNTRL,                                                                                 /* CTRL+G, 0x07 */
  __dj_ISCNTRL,                                                                                 /* CTRL+H, 0x08 */
  __dj_ISCNTRL | __dj_ISSPACE | __dj_ISBLANK,                                                   /* CTRL+I, 0x09 */
  __dj_ISCNTRL | __dj_ISSPACE,                                                                  /* CTRL+J, 0x0a */
  __dj_ISCNTRL | __dj_ISSPACE,                                                                  /* CTRL+K, 0x0b */
  __dj_ISCNTRL | __dj_ISSPACE,                                                                  /* CTRL+L, 0x0c */
  __dj_ISCNTRL | __dj_ISSPACE,                                                                  /* CTRL+M, 0x0d */
  __dj_ISCNTRL,                                                                                 /* CTRL+N, 0x0e */
  __dj_ISCNTRL,                                                                                 /* CTRL+O, 0x0f */
  __dj_ISCNTRL,                                                                                 /* CTRL+P, 0x10 */
  __dj_ISCNTRL,                                                                                 /* CTRL+Q, 0x11 */
  __dj_ISCNTRL,                                                                                 /* CTRL+R, 0x12 */
  __dj_ISCNTRL,                                                                                 /* CTRL+S, 0x13 */
  __dj_ISCNTRL,                                                                                 /* CTRL+T, 0x14 */
  __dj_ISCNTRL,                                                                                 /* CTRL+U, 0x15 */
  __dj_ISCNTRL,                                                                                 /* CTRL+V, 0x16 */
  __dj_ISCNTRL,                                                                                 /* CTRL+W, 0x17 */
  __dj_ISCNTRL,                                                                                 /* CTRL+X, 0x18 */
  __dj_ISCNTRL,                                                                                 /* CTRL+Y, 0x19 */
  __dj_ISCNTRL,                                                                                 /* CTRL+Z, 0x1a */
  __dj_ISCNTRL,                                                                                 /* CTRL+[, 0x1b */
  __dj_ISCNTRL,                                                                                 /* CTRL+\, 0x1c */
  __dj_ISCNTRL,                                                                                 /* CTRL+], 0x1d */
  __dj_ISCNTRL,                                                                                 /* CTRL+^, 0x1e */
  __dj_ISCNTRL,                                                                                 /* CTRL+_, 0x1f */
  __dj_ISPRINT | __dj_ISSPACE | __dj_ISBLANK,                                                   /* ` ', 0x20 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `!', 0x21 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* 0x22 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `#', 0x23 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `$', 0x24 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `%', 0x25 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `&', 0x26 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* 0x27 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `(', 0x28 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `)', 0x29 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `*', 0x2a */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `+', 0x2b */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `,', 0x2c */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `-', 0x2d */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `.', 0x2e */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `/', 0x2f */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `0', 0x30 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `1', 0x31 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `2', 0x32 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `3', 0x33 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `4', 0x34 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `5', 0x35 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `6', 0x36 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `7', 0x37 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `8', 0x38 */
  __dj_ISALNUM | __dj_ISDIGIT | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISXDIGIT,                    /* `9', 0x39 */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `:', 0x3a */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `;', 0x3b */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `<', 0x3c */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `=', 0x3d */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `>', 0x3e */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `?', 0x3f */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `@', 0x40 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `A', 0x41 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `B', 0x42 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `C', 0x43 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `D', 0x44 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `E', 0x45 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER | __dj_ISXDIGIT,             /* `F', 0x46 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `G', 0x47 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `H', 0x48 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `I', 0x49 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `J', 0x4a */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `K', 0x4b */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `L', 0x4c */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `M', 0x4d */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `N', 0x4e */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `O', 0x4f */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `P', 0x50 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `Q', 0x51 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `R', 0x52 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `S', 0x53 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `T', 0x54 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `U', 0x55 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `V', 0x56 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `W', 0x57 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `X', 0x58 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `Y', 0x59 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISPRINT | __dj_ISUPPER,                     /* `Z', 0x5a */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `[', 0x5b */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* 0x5c */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `]', 0x5d */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `^', 0x5e */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `_', 0x5f */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* 0x60 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `a', 0x61 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `b', 0x62 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `c', 0x63 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `d', 0x64 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `e', 0x65 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT | __dj_ISXDIGIT,             /* `f', 0x66 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `g', 0x67 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `h', 0x68 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `i', 0x69 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `j', 0x6a */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `k', 0x6b */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `l', 0x6c */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `m', 0x6d */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `n', 0x6e */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `o', 0x6f */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `p', 0x70 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `q', 0x71 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `r', 0x72 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `s', 0x73 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `t', 0x74 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `u', 0x75 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `v', 0x76 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `w', 0x77 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `x', 0x78 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `y', 0x79 */
  __dj_ISALNUM | __dj_ISALPHA | __dj_ISGRAPH | __dj_ISLOWER | __dj_ISPRINT,                     /* `z', 0x7a */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `{', 0x7b */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `|', 0x7c */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `}', 0x7d */
  __dj_ISGRAPH | __dj_ISPRINT | __dj_ISPUNCT,                                                   /* `~', 0x7e */
  __dj_ISCNTRL,                                                                                 /* 0x7f */
  0,                                                                                            /* 0x80 */
  0,                                                                                            /* 0x81 */
  0,                                                                                            /* 0x82 */
  0,                                                                                            /* 0x83 */
  0,                                                                                            /* 0x84 */
  0,                                                                                            /* 0x85 */
  0,                                                                                            /* 0x86 */
  0,                                                                                            /* 0x87 */
  0,                                                                                            /* 0x88 */
  0,                                                                                            /* 0x89 */
  0,                                                                                            /* 0x8a */
  0,                                                                                            /* 0x8b */
  0,                                                                                            /* 0x8c */
  0,                                                                                            /* 0x8d */
  0,                                                                                            /* 0x8e */
  0,                                                                                            /* 0x8f */
  0,                                                                                            /* 0x90 */
  0,                                                                                            /* 0x91 */
  0,                                                                                            /* 0x92 */
  0,                                                                                            /* 0x93 */
  0,                                                                                            /* 0x94 */
  0,                                                                                            /* 0x95 */
  0,                                                                                            /* 0x96 */
  0,                                                                                            /* 0x97 */
  0,                                                                                            /* 0x98 */
  0,                                                                                            /* 0x99 */
  0,                                                                                            /* 0x9a */
  0,                                                                                            /* 0x9b */
  0,                                                                                            /* 0x9c */
  0,                                                                                            /* 0x9d */
  0,                                                                                            /* 0x9e */
  0,                                                                                            /* 0x9f */
  0,                                                                                            /* 0xa0 */
  0,                                                                                            /* 0xa1 */
  0,                                                                                            /* 0xa2 */
  0,                                                                                            /* 0xa3 */
  0,                                                                                            /* 0xa4 */
  0,                                                                                            /* 0xa5 */
  0,                                                                                            /* 0xa6 */
  0,                                                                                            /* 0xa7 */
  0,                                                                                            /* 0xa8 */
  0,                                                                                            /* 0xa9 */
  0,                                                                                            /* 0xaa */
  0,                                                                                            /* 0xab */
  0,                                                                                            /* 0xac */
  0,                                                                                            /* 0xad */
  0,                                                                                            /* 0xae */
  0,                                                                                            /* 0xaf */
  0,                                                                                            /* 0xb0 */
  0,                                                                                            /* 0xb1 */
  0,                                                                                            /* 0xb2 */
  0,                                                                                            /* 0xb3 */
  0,                                                                                            /* 0xb4 */
  0,                                                                                            /* 0xb5 */
  0,                                                                                            /* 0xb6 */
  0,                                                                                            /* 0xb7 */
  0,                                                                                            /* 0xb8 */
  0,                                                                                            /* 0xb9 */
  0,                                                                                            /* 0xba */
  0,                                                                                            /* 0xbb */
  0,                                                                                            /* 0xbc */
  0,                                                                                            /* 0xbd */
  0,                                                                                            /* 0xbe */
  0,                                                                                            /* 0xbf */
  0,                                                                                            /* 0xc0 */
  0,                                                                                            /* 0xc1 */
  0,                                                                                            /* 0xc2 */
  0,                                                                                            /* 0xc3 */
  0,                                                                                            /* 0xc4 */
  0,                                                                                            /* 0xc5 */
  0,                                                                                            /* 0xc6 */
  0,                                                                                            /* 0xc7 */
  0,                                                                                            /* 0xc8 */
  0,                                                                                            /* 0xc9 */
  0,                                                                                            /* 0xca */
  0,                                                                                            /* 0xcb */
  0,                                                                                            /* 0xcc */
  0,                                                                                            /* 0xcd */
  0,                                                                                            /* 0xce */
  0,                                                                                            /* 0xcf */
  0,                                                                                            /* 0xd0 */
  0,                                                                                            /* 0xd1 */
  0,                                                                                            /* 0xd2 */
  0,                                                                                            /* 0xd3 */
  0,                                                                                            /* 0xd4 */
  0,                                                                                            /* 0xd5 */
  0,                                                                                            /* 0xd6 */
  0,                                                                                            /* 0xd7 */
  0,                                                                                            /* 0xd8 */
  0,                                                                                            /* 0xd9 */
  0,                                                                                            /* 0xda */
  0,                                                                                            /* 0xdb */
  0,                                                                                            /* 0xdc */
  0,                                                                                            /* 0xdd */
  0,                                                                                            /* 0xde */
  0,                                                                                            /* 0xdf */
  0,                                                                                            /* 0xe0 */
  0,                                                                                            /* 0xe1 */
  0,                                                                                            /* 0xe2 */
  0,                                                                                            /* 0xe3 */
  0,                                                                                            /* 0xe4 */
  0,                                                                                            /* 0xe5 */
  0,                                                                                            /* 0xe6 */
  0,                                                                                            /* 0xe7 */
  0,                                                                                            /* 0xe8 */
  0,                                                                                            /* 0xe9 */
  0,                                                                                            /* 0xea */
  0,                                                                                            /* 0xeb */
  0,                                                                                            /* 0xec */
  0,                                                                                            /* 0xed */
  0,                                                                                            /* 0xee */
  0,                                                                                            /* 0xef */
  0,                                                                                            /* 0xf0 */
  0,                                                                                            /* 0xf1 */
  0,                                                                                            /* 0xf2 */
  0,                                                                                            /* 0xf3 */
  0,                                                                                            /* 0xf4 */
  0,                                                                                            /* 0xf5 */
  0,                                                                                            /* 0xf6 */
  0,                                                                                            /* 0xf7 */
  0,                                                                                            /* 0xf8 */
  0,                                                                                            /* 0xf9 */
  0,                                                                                            /* 0xfa */
  0,                                                                                            /* 0xfb */
  0,                                                                                            /* 0xfc */
  0,                                                                                            /* 0xfd */
  0,                                                                                            /* 0xfe */
  0,                                                                                            /* 0xff */
};

void RegisterLibC()
{
        FILE *_stderr = stderr;
        SYSEXPORT(exp);
        SYSEXPORT(__dj_ctype_flags);
        SYSEXPORT(memmove);
        SYSEXPORT(strncpy);
        SYSEXPORT(ftell);

        SYSEXPORT(time);
        SYSEXPORT(free);
        SYSEXPORT(malloc);
        SYSEXPORT(realloc);
        SYSEXPORT(strcmp);
        SYSEXPORT(stricmp);
        SYSEXPORT(strcat);
        SYSEXPORT(strlen);
        SYSEXPORT(strdup);
        SYSEXPORT(strcpy);
        SYSEXPORT(fopen);
        SYSEXPORT(fclose);
        SYSEXPORT(fseek);
        SYSEXPORT(fwrite);
        SYSEXPORT(fprintf);
        SYSEXPORT(fgets);
        SYSEXPORT(feof);
        SYSEXPORT(fread);
        SYSEXPORT(fsetpos);
        SYSEXPORT(fgetpos);
        SYSEXPORT(sprintf);
        SYSEXPORT(vsprintf);
        SYSEXPORT(ctime);
        SYSEXPORT(atof);
        SYSEXPORT(atoi);
        SYSEXPORT(atol);
        SYSEXPORT(strtod);
        SYSEXPORT(strtol);
        SYSEXPORT(sin);
        SYSEXPORT(cos);
        SYSEXPORT(rand);
        SYSEXPORTAS(rand,"random");
        SYSEXPORT(srand);
        SYSEXPORT(memcpy);
        SYSEXPORT(pow);
        SYSEXPORT(memset);
        SYSEXPORT(setjmp);
        SYSEXPORT(longjmp);
        SYSEXPORT(localtime);
        SYSEXPORT(abort);
        SYSEXPORT(gmtime);
        SYSEXPORT(fflush);
        SYSEXPORT(getchar);
        SYSEXPORT(system);
        SYSEXPORT(puts);
        SYSEXPORT(printf);
        SYSEXPORT(vfprintf);
        SYSEXPORT(fputc);
        SYSEXPORTAS(*stderr, "__dj_stderr");
        SYSEXPORTAS(_stderr,"stderr");
        SYSEXPORTAS(strdup,"__strdup");
        SYSEXPORTAS(setjmp,"_setjmp");
        SYSEXPORTAS(longjmp,"_longjmp");
        SYSEXPORT(__strtod_internal);
        SYSEXPORT(__strtol_internal);
        SYSEXPORTAS(stricmp,"_alemu_stricmp");

        SYSEXPORT(qsort);
        SYSEXPORT(calloc);
        SYSEXPORT(floor);
        SYSEXPORT(tan);
        SYSEXPORT(sqrt);
        SYSEXPORT(exit);
        SYSEXPORT(strtoul);

}
#endif

#ifdef _OZONE__DJGPP_

#include <setjmp.h>
#include <io.h>
#include <bios.h>
#include <fcntl.h>
#include <dos.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <mem.h>
#include <math.h>
#include <string.h>
#include <process.h>
#include <float.h>
#include <dpmi.h>
#include <sys/fsext.h>
#include <errno.h>
#include <crt0.h>
#include <unistd.h>
#include <sys/dxe.h>
#include <go32.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <ctype.h>
#include <dirent.h>
#include <termios.h>
#include <sys/vfs.h>
#include <signal.h>
#include <sys/resource.h>
#include <locale.h>
#include <grp.h>
#include <dir.h>
#include <search.h>
#include <sys/types.h>
#include <regex.h>
#include <signal.h>
#include <sys/mono.h>
#include <utime.h>
#include <sys/times.h>
#include <string.h>
#include <pwd.h>
#include <mntent.h>
#include <pc.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/movedata.h>
#include <libc/ttyprvt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <glob.h>
#include <ftw.h>
#include <fnmatch.h>
#include <sys/farptr.h>
#include <sys/nearptr.h>
#include <assert.h>
#include <sys/exceptn.h>
#include <tcp.h>
#include <arpa/inet.h>
#include <JUDAS.H>

extern unsigned short timer_frequency;
extern unsigned short timer_systemcount;
extern unsigned short timer_ds;
extern int timer_code_lock_start;
extern int timer_code_lock_end;
extern void (*timer_function)();
extern __dpmi_paddr timer_oldvect;
extern void timer_handler(void);
extern volatile unsigned timer_count;

int ____registered = 0;

void RegisterLibC()
{
  if (!____registered)
  {
        FILE *_stderr = &__dj_stderr;


        ____registered = 1;
        SYSEXPORT(exp);

        SYSEXPORT(qsort);
        SYSEXPORT(calloc);
        SYSEXPORT(floor);

        SYSEXPORTAS(stricmp,"_alemu_stricmp");
        SYSEXPORTAS(setjmp,"_setjmp");
        SYSEXPORTAS(longjmp,"_longjmp");
        SYSEXPORTAS(strdup,"__strdup");
        SYSEXPORT(__strtod_internal);
        SYSEXPORT(__strtol_internal);

        SYSEXPORTAS(_stderr,"stderr");

        SYSEXPORT(srand);
        SYSEXPORT(setjmp);
        SYSEXPORT(longjmp);
        SYSEXPORT(__dpmi_allocate_memory);
        SYSEXPORT(rewinddir);
        SYSEXPORT(__dpmi_clear_debug_watchpoint);
        SYSEXPORT(__exit);
        SYSEXPORT(__dpmi_unlock_linear_region);
        SYSEXPORT(__dj_assert);
        SYSEXPORT(insque);
        SYSEXPORT(mblen);
        SYSEXPORT(__dpmi_get_and_disable_virtual_interrupt_state);
        SYSEXPORT(usleep);
        SYSEXPORT(intensevideo);
        SYSEXPORT(__dpmi_set_multiple_descriptors);
        SYSEXPORT(getpid);
        SYSEXPORT(putchar);
        SYSEXPORT(getgid);
        SYSEXPORT(execl);
        SYSEXPORT(__dpmi_lock_linear_region);
        SYSEXPORT(statfs);
        SYSEXPORT(bioskey);
        SYSEXPORT(symlink);
        SYSEXPORT(outpw);
        SYSEXPORT(strcspn);
        SYSEXPORT(sprintf);
        SYSEXPORT(_dos_gettime);
        SYSEXPORT(textattr);
        SYSEXPORT(__dpmi_set_real_mode_interrupt_vector);
        SYSEXPORT(feof);
        SYSEXPORT(blinkvideo);
        SYSEXPORT(getche);
        SYSEXPORT(_exit);
        SYSEXPORT(__crt0_setup_arguments);
        SYSEXPORT(setcbrk);
        SYSEXPORT(utime);
        SYSEXPORT(_truename);
        SYSEXPORT(getgrent);
        SYSEXPORT(__djgpp_memory_handle);
        SYSEXPORT(tcgetattr);
        SYSEXPORT(free);
        SYSEXPORT(strcat);
        SYSEXPORT(dup);
        SYSEXPORT(gettext);
        SYSEXPORT(strcmp);
        SYSEXPORT(cgets);
        SYSEXPORT(getpass);
        SYSEXPORT(memccpy);
        SYSEXPORT(__dpmi_install_resident_service_provider_callback);
        SYSEXPORT(__dpmi_simulate_real_mode_procedure_retf_stack);
        SYSEXPORT(ScreenUpdate);
        SYSEXPORT(regfree);
        SYSEXPORT(pause);
        SYSEXPORT(getpgrp);
        SYSEXPORT(spawnlp);
        SYSEXPORT(globfree);
        SYSEXPORT(execlp);
        SYSEXPORT(__dpmi_allocate_real_mode_callback);
        SYSEXPORT(lock);
        SYSEXPORT(nice);
        SYSEXPORT(acosh);
        SYSEXPORT(mkdir);
        SYSEXPORT(sleep);
        SYSEXPORT(__dpmi_get_memory_information);
        SYSEXPORT(__djgpp_exception_toggle);
        SYSEXPORT(searchpath);
        SYSEXPORT(mkstemp);
        SYSEXPORT(_go32_dpmi_get_protected_mode_interrupt_vector);
        SYSEXPORT(__djgpp_nearptr_enable);
        SYSEXPORT(vsprintf);
        SYSEXPORT(__dpmi_get_virtual_interrupt_state);
        SYSEXPORT(_go32_info_block);
        SYSEXPORT(log10);
        SYSEXPORT(settime);
        SYSEXPORT(ScreenClear);
        SYSEXPORT(pow10);
        SYSEXPORT(_bios_disk);
        SYSEXPORT(telldir);
        SYSEXPORT(__dpmi_get_vendor_specific_api_entry_point);
        SYSEXPORT(fgetpos);
        SYSEXPORT(glob);
        SYSEXPORT(vprintf);
        SYSEXPORT(__dpmi_get_page_size);
        SYSEXPORT(setlinebuf);
        SYSEXPORT(_dos_lock);
        SYSEXPORT(_write);
        SYSEXPORT(rand);
        SYSEXPORT(__dpmi_serialize_on_shared_memory);
        SYSEXPORT(modf);
        SYSEXPORT(ceil);
        SYSEXPORT(putc);
        SYSEXPORT(clock);
        SYSEXPORT(_bios_timeofday);
        SYSEXPORT(strupr);
        SYSEXPORT(ScreenGetCursor);
        SYSEXPORT(wctomb);
        SYSEXPORT(__dpmi_set_debug_watchpoint);
        SYSEXPORT(__libc_termios_init);
        SYSEXPORT(getc);
        SYSEXPORT(bsearch);
        SYSEXPORT(_djstat_describe_lossage);
        SYSEXPORT(insline);
        SYSEXPORT(ungetc);
        SYSEXPORT(getw);
        SYSEXPORT(putch);
        SYSEXPORT(closedir);
        SYSEXPORT(ScreenRetrieve);
        SYSEXPORT(rewind);
        SYSEXPORT(_dosmemputl);
        SYSEXPORT(_go32_dpmi_allocate_real_mode_callback_retf);
        SYSEXPORT(getlogin);
        SYSEXPORT(biosdisk);
        SYSEXPORT(sysconf);
        SYSEXPORT(_read);
        SYSEXPORT(spawnle);
        SYSEXPORT(_movedataw);
        SYSEXPORT(__dj_stdin);
        SYSEXPORT(setvbuf);
        SYSEXPORT(biosequip);
        SYSEXPORT(ScreenPutString);
        SYSEXPORT(setbuf);
        SYSEXPORT(abs);
        SYSEXPORT(puts);
        SYSEXPORT(_go32_dpmi_remaining_virtual_memory);
        SYSEXPORT(getftime);
        SYSEXPORT(_status87);
        SYSEXPORT(__dpmi_terminate_and_stay_resident);
        SYSEXPORT(strtoull);
        SYSEXPORT(_dos_findnext);
        SYSEXPORT(_dos_setfileattr);
        SYSEXPORT(findfirst);
        SYSEXPORT(setdisk);
        SYSEXPORT(execv);
        SYSEXPORT(select);
        SYSEXPORT(ftime);
        SYSEXPORT(int86);
        SYSEXPORT(__dpmi_relock_real_mode_region);
        SYSEXPORT(rmdir);
        SYSEXPORT(_dos_settime);
        SYSEXPORT(strncat);
        SYSEXPORT(getpagesize);
        SYSEXPORT(_open);
        SYSEXPORT(putenv);
        SYSEXPORT(__dpmi_set_segment_limit);
        SYSEXPORT(strlwr);
        SYSEXPORT(tcsetattr);
        SYSEXPORT(fstat);
        SYSEXPORT(spawnl);
        SYSEXPORT(atanh);
        SYSEXPORT(clreol);
        SYSEXPORT(readdir);
        SYSEXPORT(spawnve);
        SYSEXPORT(fputc);
        SYSEXPORT(regcomp);
        SYSEXPORT(execvp);
        SYSEXPORT(setrlimit);
        SYSEXPORT(chsize);
        SYSEXPORT(__dpmi_yield);
        SYSEXPORT(__dpmi_free_shared_memory);
        SYSEXPORT(cfgetospeed);
        SYSEXPORT(__dpmi_set_extended_exception_handler_vector_rm);
        SYSEXPORT(_fpreset);
        SYSEXPORT(scanf);
        SYSEXPORT(__dpmi_simulate_real_mode_interrupt);
        SYSEXPORT(addmntent);
        SYSEXPORT(getwd);
        SYSEXPORT(__dpmi_allocate_linear_memory);
        SYSEXPORT(gethostname);
        SYSEXPORT(_set_screen_lines);
        SYSEXPORT(fnsplit);
        SYSEXPORT(mktime);
        SYSEXPORT(cputs);
        SYSEXPORT(__dpmi_set_descriptor_access_rights);
        SYSEXPORT(getegid);
        SYSEXPORT(getrusage);
        SYSEXPORT(random);
        SYSEXPORT(fputs);
        SYSEXPORT(setdate);
        SYSEXPORT(ftw);
        SYSEXPORT(rindex);
        SYSEXPORT(_dos_close);
        SYSEXPORT(atof);
        SYSEXPORT(ferror);
        SYSEXPORT(_go32_dpmi_free_real_mode_callback);
        SYSEXPORT(_osmajor);
        SYSEXPORT(isatty);
        SYSEXPORT(sinh);
        SYSEXPORT(movetext);
        SYSEXPORT(strcoll);
        SYSEXPORT(fdopen);
        SYSEXPORT(movedata);
        SYSEXPORT(__dpmi_allocate_dos_memory);
        SYSEXPORT(inportsb);
        SYSEXPORT(open);
        SYSEXPORT(__dpmi_map_conventional_memory_in_memory_block);
        SYSEXPORT(_doprnt);
        SYSEXPORT(sbrk);
        SYSEXPORT(ftruncate);
        SYSEXPORT(_atold);
        SYSEXPORT(_dos_read);
        SYSEXPORT(__dpmi_free_memory);
        SYSEXPORT(__dpmi_set_extended_exception_handler_vector_pm);
        SYSEXPORT(_go32_dpmi_allocate_real_mode_callback_iret);
        SYSEXPORT(mknod);
        SYSEXPORT(fwrite);
        SYSEXPORT(gets);
        SYSEXPORT(fnmatch);
        SYSEXPORT(strncpy);
        SYSEXPORT(outp);
        SYSEXPORT(window);
        SYSEXPORT(__file_tree_walk);
        SYSEXPORT(hasmntopt);
        SYSEXPORT(sys_nerr);
        SYSEXPORT(times);
        SYSEXPORT(endgrent);
        SYSEXPORT(_lfn_get_ftime);
        SYSEXPORT(_dosmemgetb);
        SYSEXPORT(atoi);
        SYSEXPORT(bdos);
        SYSEXPORT(wcstombs);
        SYSEXPORT(getkey);
        SYSEXPORT(rawclock);
        SYSEXPORT(__dpmi_physical_address_mapping);
        SYSEXPORT(biosprint);
        SYSEXPORT(_chmod);
        SYSEXPORT(ttyname);
        SYSEXPORT(regexec);
        SYSEXPORT(_mono_putc);
        SYSEXPORT(_dos_creatnew);
        SYSEXPORT(__dpmi_get_real_mode_interrupt_vector);
        SYSEXPORT(memcpy);
        SYSEXPORT(__dpmi_get_free_memory_information);
        SYSEXPORT(opendir);
        SYSEXPORT(_dos_open);
        SYSEXPORT(execvpe);
        SYSEXPORT(_go32_dpmi_set_real_mode_interrupt_vector);
        SYSEXPORT(getgrgid);
        SYSEXPORT(_go32_interrupt_stack_size);
        SYSEXPORT(popen);
        SYSEXPORT(memset);
        SYSEXPORT(setlocale);
        SYSEXPORT(_go32_dpmi_lock_code);
        SYSEXPORT(settimeofday);
        SYSEXPORT(dosmemget);
        SYSEXPORT(stat);
        SYSEXPORT(_dosmemputw);
        SYSEXPORT(fileno);
        SYSEXPORT(ScreenSetCursor);
        SYSEXPORT(__dpmi_int);
        SYSEXPORT(puttext);
        SYSEXPORT(__dj_ctype_toupper);
        SYSEXPORT(__dpmi_get_raw_mode_switch_addr);
        SYSEXPORT(mprotect);
        SYSEXPORT(_go32_was_ctrl_break_hit);
        SYSEXPORT(bcmp);
        SYSEXPORT(waitpid);
        SYSEXPORT(strerror);
        SYSEXPORT(mkfifo);
        SYSEXPORT(_go32_dpmi_resize_dos_memory);
        SYSEXPORT(outportsw);
        SYSEXPORT(__dpmi_get_descriptor_access_rights);
        SYSEXPORT(remove);
        SYSEXPORT(setpgid);
        SYSEXPORT(_dosexterr);
        SYSEXPORT(chdir);
        SYSEXPORT(getlongpass);
        SYSEXPORT(_control87);
        SYSEXPORT(cfgetispeed);
        SYSEXPORT(mbstowcs);
        SYSEXPORT(ScreenUpdateLine);
        SYSEXPORT(getuid);
        SYSEXPORT(fflush);
        SYSEXPORT(clearerr);
        SYSEXPORT(dosmemput);
        SYSEXPORT(_djstat_flags);
        SYSEXPORT(itoa);
        SYSEXPORT(fpurge);
        SYSEXPORT(__dpmi_get_extended_exception_handler_vector_pm);
        SYSEXPORT(ldiv);
        SYSEXPORT(__dpmi_set_protected_mode_interrupt_vector);
        SYSEXPORT(_strtold);
        SYSEXPORT(asinh);
        SYSEXPORT(strxfrm);
        SYSEXPORT(_dos_setdrive);
        SYSEXPORT(fprintf);
        SYSEXPORT(setitimer);
        SYSEXPORT(llabs);
        SYSEXPORT(wait);
        SYSEXPORT(gettextinfo);
        SYSEXPORT(__dpmi_set_processor_exception_handler_vector);
        SYSEXPORT(_mono_printf);
        SYSEXPORT(lseek);
        SYSEXPORT(delline);
        SYSEXPORT(inportsw);
        SYSEXPORT(strtol);
        SYSEXPORT(getdate);
        SYSEXPORT(getitimer);
        SYSEXPORT(_get_volume_info);
        SYSEXPORT(_clear87);
        SYSEXPORT(log2);
        SYSEXPORT(strdup);
        SYSEXPORT(normvideo);
        SYSEXPORT(setpwent);
        SYSEXPORT(tell);
        SYSEXPORT(__FSEXT_alloc_fd);
        SYSEXPORT(_go32_dpmi_free_iret_wrapper);
        SYSEXPORT(setbuffer);
        SYSEXPORT(strcasecmp);
        SYSEXPORT(outportsb);
        SYSEXPORT(execle);
        SYSEXPORT(fread);
        SYSEXPORT(__dpmi_get_state_of_debug_watchpoint);
        SYSEXPORT(_creat);
        SYSEXPORT(gmtime);
        SYSEXPORT(_dos_getdiskfree);
        SYSEXPORT(getgrnam);
        SYSEXPORT(gppconio_init);
        SYSEXPORT(pow2);
        SYSEXPORT(swab);
        SYSEXPORT(fopen);
        SYSEXPORT(setmode);
        SYSEXPORT(_go32_dpmi_chain_protected_mode_interrupt_vector);
        SYSEXPORT(__dpmi_free_real_mode_callback);
        SYSEXPORT(_dosmemgetl);
        SYSEXPORT(calloc);
        SYSEXPORT(time);
        SYSEXPORT(_fmode);
        SYSEXPORT(__dpmi_get_memory_block_size_and_base);
        SYSEXPORT(getrlimit);
        SYSEXPORT(__dpmi_resize_memory);
        SYSEXPORT(cfsetspeed);
        SYSEXPORT(__dpmi_get_multiple_descriptors);
        SYSEXPORT(ScreenAttrib);
        SYSEXPORT(_go32_rmcb_stack_size);
        SYSEXPORT(strpbrk);
        SYSEXPORT(textbackground);
        SYSEXPORT(getchar);
        SYSEXPORT(__dpmi_set_descriptor);
        SYSEXPORT(ctime);
        SYSEXPORT(stpcpy);
        SYSEXPORT(ScreenPutChar);
        SYSEXPORT(dup2);
        SYSEXPORT(strftime);
        SYSEXPORT(brk);
        SYSEXPORT(hypot);
        SYSEXPORT(outportsl);
        SYSEXPORT(__dpmi_free_serialization_on_shared_memory);
        SYSEXPORT(setmntent);
        SYSEXPORT(_go32_dpmi_get_real_mode_interrupt_vector);
        SYSEXPORT(__dpmi_get_selector_increment_value);
        SYSEXPORT(bcopy);
        SYSEXPORT(getpwnam);
        SYSEXPORT(_dos_setftime);
        SYSEXPORT(cfree);
        SYSEXPORT(tmpnam);
        SYSEXPORT(__dj_stdout);
        SYSEXPORT(realloc);
        SYSEXPORT(getch);
        SYSEXPORT(_go32_dpmi_remaining_physical_memory);
        SYSEXPORT(memchr);
        SYSEXPORT(link);
        SYSEXPORT(sys_errlist);
        SYSEXPORT(tcsendbreak);
        SYSEXPORT(__dpmi_get_segment_base_address);
        SYSEXPORT(__dpmi_get_coprocessor_status);
        SYSEXPORT(fnmerge);
        SYSEXPORT(setgrent);
        SYSEXPORT(getenv);
        SYSEXPORT(_movedatal);
        SYSEXPORT(strncasecmp);
        SYSEXPORT(labs);
        SYSEXPORT(chown);
        SYSEXPORT(access);
        SYSEXPORT(_use_lfn);
        SYSEXPORT(filelength);
        SYSEXPORT(textcolor);
        SYSEXPORT(_dos_findfirst);
        SYSEXPORT(uname);
        SYSEXPORT(creat);
        SYSEXPORT(bioscom);
        SYSEXPORT(__dpmi_simulate_real_mode_procedure_iret);
        SYSEXPORT(__dpmi_get_descriptor);
        SYSEXPORT(spawnv);
        SYSEXPORT(__dpmi_get_capabilities);
        SYSEXPORT(_setcursortype);
        SYSEXPORT(strtod);
        SYSEXPORT(cfmakeraw);
        SYSEXPORT(inpw);
        SYSEXPORT(alarm);
        SYSEXPORT(__djgpp_set_ctrl_c);
        SYSEXPORT(rename);
        SYSEXPORT(__FSEXT_call_open_handlers);
        SYSEXPORT(strchr);
        SYSEXPORT(fsync);
        SYSEXPORT(log);
        SYSEXPORT(uclock);
        SYSEXPORT(__FSEXT_get_function);
        SYSEXPORT(__dpmi_create_alias_descriptor);
        SYSEXPORT(__djgpp_set_page_attributes);
        SYSEXPORT(fgetc);
        SYSEXPORT(__dpmi_segment_to_descriptor);
        SYSEXPORT(cfsetispeed);
        SYSEXPORT(_preserve_fncase);
        SYSEXPORT(signal);
        SYSEXPORT(__dpmi_get_version);
        SYSEXPORT(tcdrain);
        SYSEXPORT(malloc);
        SYSEXPORT(_os_flavor);
        SYSEXPORT(fclose);
        SYSEXPORT(pipe);
        SYSEXPORT(strcpy);
        SYSEXPORT(_go32_want_ctrl_break);
        SYSEXPORT(sscanf);
        SYSEXPORT(cprintf);
        SYSEXPORT(fseek);
        SYSEXPORT(__djgpp_map_physical_memory);
        SYSEXPORT(biosmemory);
        SYSEXPORT(execve);
        SYSEXPORT(memcmp);
        SYSEXPORT(_dos_getdrive);
        SYSEXPORT(_dos_getftime);
        SYSEXPORT(__dpmi_mark_real_mode_region_as_pageable);
        SYSEXPORT(getmntent);
        SYSEXPORT(__dpmi_allocate_ldt_descriptors);
        SYSEXPORT(strspn);
        SYSEXPORT(atexit);
        SYSEXPORT(__dpmi_get_and_set_virtual_interrupt_state);
        SYSEXPORT(chmod);
        SYSEXPORT(_dos_commit);
        SYSEXPORT(perror);
        SYSEXPORT(fpathconf);
        SYSEXPORT(__dpmi_mark_page_as_demand_paging_candidate);
        SYSEXPORT(strtok);
        SYSEXPORT(_get_dev_info);
        SYSEXPORT(_dos_unlock);
        SYSEXPORT(localtime);
        SYSEXPORT(__dpmi_set_segment_base_address);
        SYSEXPORT(execlpe);
        SYSEXPORT(_go32_dpmi_set_protected_mode_interrupt_vector);
        SYSEXPORT(abort);
        SYSEXPORT(_doscan);
        SYSEXPORT(tmpfile);
        SYSEXPORT(__djgpp_memory_handle_list);
        SYSEXPORT(_dos_setdate);
        SYSEXPORT(fgetgrent);
        SYSEXPORT(ScreenCols);
        SYSEXPORT(endmntent);
        SYSEXPORT(modfl);
        SYSEXPORT(asctime);
        SYSEXPORT(wherey);
        SYSEXPORT(_osminor);
        SYSEXPORT(_is_executable);
        SYSEXPORT(__djgpp_nearptr_disable);
        SYSEXPORT(__dj_ctype_flags);
        SYSEXPORT(unlink);
        SYSEXPORT(cscanf);
        SYSEXPORT(lldiv);
        SYSEXPORT(_dos_write);
        SYSEXPORT(_dosmemgetw);
        SYSEXPORT(strtoll);
        SYSEXPORT(fork);
        SYSEXPORT(ScreenGetChar);
        SYSEXPORT(__dpmi_resize_dos_memory);
        SYSEXPORT(tanh);
        SYSEXPORT(pow);
        SYSEXPORT(__dpmi_reset_debug_watchpoint);
        SYSEXPORT(truncate);
        SYSEXPORT(__dpmi_get_protected_mode_interrupt_vector);
        SYSEXPORT(localeconv);
        SYSEXPORT(_dos_getdate);
        SYSEXPORT(ScreenMode);
        SYSEXPORT(__FSEXT_add_open_handler);
        SYSEXPORT(_movedatab);
        SYSEXPORT(sound);
        SYSEXPORT(endpwent);
        SYSEXPORT(getcwd);
        SYSEXPORT(wherex);
        SYSEXPORT(mbtowc);
        SYSEXPORT(strncmp);
        SYSEXPORT(ftell);
        SYSEXPORT(_flush_disk_cache);
        SYSEXPORT(_close);
        SYSEXPORT(gettime);
        SYSEXPORT(write);
        SYSEXPORT(strnicmp);
        SYSEXPORT(utimes);
        SYSEXPORT(__dpmi_free_dos_memory);
        SYSEXPORT(__dpmi_set_page_attributes);
        SYSEXPORT(__dj_ctype_tolower);
        SYSEXPORT(__dpmi_free_physical_address_mapping);
        SYSEXPORT(sync);
        SYSEXPORT(__dpmi_allocate_shared_memory);
        SYSEXPORT(__dpmi_get_and_enable_virtual_interrupt_state);
        SYSEXPORT(ScreenRows);
        SYSEXPORT(highvideo);
        SYSEXPORT(strrchr);
        SYSEXPORT(inportsl);
        SYSEXPORT(_dos_creat);
        SYSEXPORT(getdfree);
        SYSEXPORT(_fixpath);
        SYSEXPORT(strtoul);
        SYSEXPORT(pclose);
        SYSEXPORT(stricmp);
        SYSEXPORT(_mono_clear);
        SYSEXPORT(_go32_dpmi_allocate_dos_memory);
        SYSEXPORT(__dpmi_set_coprocessor_emulation);
        SYSEXPORT(siglongjmp);
        SYSEXPORT(__dpmi_resize_linear_memory);
        SYSEXPORT(srandom);
        SYSEXPORT(remque);
        SYSEXPORT(spawnvp);
        SYSEXPORT(difftime);
        SYSEXPORT(atol);
        SYSEXPORT(getxkey);
        SYSEXPORT(int86x);
        SYSEXPORT(__dpmi_get_state_save_restore_addr);
        SYSEXPORT(fgets);
        SYSEXPORT(div);
        SYSEXPORT(kill);
        SYSEXPORT(lowvideo);
        SYSEXPORT(strsep);
        SYSEXPORT(__dpmi_free_ldt_descriptor);
        SYSEXPORT(strlen);
        SYSEXPORT(spawnvpe);
        SYSEXPORT(errno);
        SYSEXPORT(gettimeofday);
        SYSEXPORT(disable);
        SYSEXPORT(__dpmi_map_device_in_memory_block);
        SYSEXPORT(read);
        SYSEXPORT(getcbrk);
        SYSEXPORT(memmove);
        SYSEXPORT(getdisk);
        SYSEXPORT(_crt0_startup_flags);
        SYSEXPORT(ffs);
        SYSEXPORT(__file_exists);
        SYSEXPORT(tcflow);
        SYSEXPORT(setftime);
        SYSEXPORT(exit);
        SYSEXPORT(tcflush);
        SYSEXPORT(findnext);
        SYSEXPORT(gotoxy);
        SYSEXPORT(inp);
        SYSEXPORT(_dosmemputb);
        SYSEXPORT(__dpmi_get_page_attributes);
        SYSEXPORT(freopen);
        SYSEXPORT(mktemp);
        SYSEXPORT(setenv);
        SYSEXPORT(fsetpos);
        SYSEXPORT(geteuid);
        SYSEXPORT(__FSEXT_set_function);
        SYSEXPORT(printf);
        SYSEXPORT(biostime);
        SYSEXPORT(crlf2nl);
        SYSEXPORT(__dj_stderr);
        SYSEXPORT(putw);
        SYSEXPORT(__dpmi_simulate_real_mode_procedure_retf);
        SYSEXPORT(_go32_dpmi_allocate_iret_wrapper);
        SYSEXPORT(_get_dos_version);
        SYSEXPORT(getopt);
        SYSEXPORT(delay);
        SYSEXPORT(umask);
        SYSEXPORT(sigsetjmp);
        SYSEXPORT(__dpmi_get_processor_exception_handler_vector);
        SYSEXPORT(qsort);
        SYSEXPORT(_go32_dpmi_free_dos_memory);
        SYSEXPORT(ScreenVisualBell);
        SYSEXPORT(unlock);
        SYSEXPORT(getdtablesize);
        SYSEXPORT(fcntl);
        SYSEXPORT(getpwuid);
        SYSEXPORT(getpwent);
        SYSEXPORT(__dpmi_error);
        SYSEXPORT(clrscr);
        SYSEXPORT(spawnlpe);
        SYSEXPORT(_rename);
        SYSEXPORT(cosh);
        SYSEXPORT(ungetch);
        SYSEXPORT(pathconf);
        SYSEXPORT(regerror);
        SYSEXPORT(strstr);
        SYSEXPORT(__crt0_load_environment_file);
        SYSEXPORT(_go32_dpmi_lock_data);
        SYSEXPORT(_lfn_gen_short_fname);
        SYSEXPORT(_djstat_fail_bits);
        SYSEXPORT(vfprintf);
        SYSEXPORT(_dos_getfileattr);
        SYSEXPORT(_conio_kbhit);
        SYSEXPORT(system);
        SYSEXPORT(seekdir);
        SYSEXPORT(__dpmi_allocate_specific_ldt_descriptor);
        SYSEXPORT(__dpmi_get_extended_exception_handler_vector_rm);
        SYSEXPORT(__dpmi_discard_page_contents);
        SYSEXPORT(fscanf);
        SYSEXPORT(close);
        SYSEXPORT(enable);
        SYSEXPORT(cfsetospeed);
        SYSEXPORT(__crt0_glob_function);
        SYSEXPORT(__dpmi_get_segment_limit);
        SYSEXPORT(__udivdi3);
        SYSEXPORT(acos);
        SYSEXPORT(asin);
        SYSEXPORT(atan);
        SYSEXPORT(atan2);
        SYSEXPORT(cos);
        SYSEXPORT(exp);
        SYSEXPORT(fabs);
        SYSEXPORT(floor);
        SYSEXPORT(fmod);
        SYSEXPORT(frexp);
        SYSEXPORT(ldexp);
        SYSEXPORT(sin);
        SYSEXPORT(sqrt);
        SYSEXPORT(tan);
        SYSEXPORT(cbrt);
        SYSEXPORT(exp10);
        SYSEXPORT(exp2);
        SYSEXPORT(expm1);
        SYSEXPORT(log1p);
        SYSEXPORT(powi);
        SYSEXPORT(sincos);

        /* for watt32 library external resolve */
        SYSEXPORT(_w32__ip_delay1);
        SYSEXPORT(_w32_tcp_open);
        SYSEXPORT(_inet_ntoa);
        SYSEXPORT(inet_ntoa);
        SYSEXPORT(_w32_sock_read);
        SYSEXPORT(gethostid);
        SYSEXPORT(_w32_sock_delay);
        SYSEXPORT(_w32_resolve);
        SYSEXPORT(_w32_sock_flush);
        SYSEXPORT(_w32__ip_delay2);
        SYSEXPORT(watt_sock_init);
        SYSEXPORT(_w32_sock_dataready);
        SYSEXPORT(_w32_dbug_init);
        SYSEXPORT(_w32__ip_delay0);
        SYSEXPORT(_w32_sock_puts);

        SYSEXPORT(_w32_sock_fastwrite);
        SYSEXPORT(_w32_sock_close);
        SYSEXPORT(_w32_usr_init);
        SYSEXPORT(_w32_sock_setbuf);
        SYSEXPORT(_w32_tcp_tick);
        SYSEXPORT(_w32_sockerr);
        SYSEXPORT(_w32_lookup_host);
        SYSEXPORT(tzset);

        /* for convmpeg.app */
        SYSEXPORT(mpeg2_init);
        SYSEXPORT(mpeg2_parse);
        SYSEXPORT(mpeg2_buffer);
        SYSEXPORT(mpeg2_info);
        SYSEXPORT(mpeg2_close);\

        /* for mbox music play using judas library */
        SYSEXPORT(judas_memunlock);
        SYSEXPORT(judas_memlock);

        SYSEXPORT(timer_code_lock_end);
        SYSEXPORT(timer_systemcount);
        SYSEXPORT(timer_frequency);
        SYSEXPORT(timer_function);
        SYSEXPORT(timer_oldvect);
        SYSEXPORT(timer_handler);
        SYSEXPORT(timer_count);
        SYSEXPORT(timer_code_lock_start);
        SYSEXPORT(timer_ds);
        SYSEXPORT(judas_errortext);
        SYSEXPORT(judas_uninit);
        SYSEXPORT(judas_init);
        SYSEXPORT(judas_device);
        SYSEXPORT(judas_setmusicmastervolume);
        SYSEXPORT(judas_error);
        SYSEXPORT(judas_config);
        SYSEXPORT(judas_loadxm);
        SYSEXPORT(judas_loadmod);
        SYSEXPORT(judas_loads3m);
        SYSEXPORT(judas_playxm);
        SYSEXPORT(judas_playmod);
        SYSEXPORT(judas_plays3m);
        SYSEXPORT(judas_songisplaying);
        SYSEXPORT(judas_forwardxm);
        SYSEXPORT(judas_forwardmod);
        SYSEXPORT(judas_forwards3m);
        SYSEXPORT(judas_rewindxm);
        SYSEXPORT(judas_rewindmod);
        SYSEXPORT(judas_rewinds3m);
        SYSEXPORT(judas_setmusicmastervolume);
        SYSEXPORT(judas_freexm);
        SYSEXPORT(judas_freemod);
        SYSEXPORT(judas_frees3m);
        SYSEXPORT(judas_update);
        SYSEXPORT(judas_stopxm);
        SYSEXPORT(judas_stopmod);
        SYSEXPORT(judas_stops3m);
  }
}

#endif
