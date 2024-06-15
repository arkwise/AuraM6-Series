# Aura OS Makefile
# 
# the xdev apps need the flags listed for compiling with the new libs.

#xdev apps have been added to the dyndl but not ready to build. this hasnt been saved i was on the plane.


OBJ_LIST1 = \
$(OBJ_DIR)/main.o \
$(OBJ_DIR)/debug.o \
$(OBJ_DIR)/vfile.o \
$(OBJ_DIR)/registry.o \
$(OBJ_DIR)/ini.o \
$(OBJ_DIR)/screen.o \
$(OBJ_DIR)/metrics.o \
$(OBJ_DIR)/mouse.o \
$(OBJ_DIR)/keyboard.o \
$(OBJ_DIR)/tasks.o \
$(OBJ_DIR)/events.o \
$(OBJ_DIR)/exports.o \
$(OBJ_DIR)/image.o \
$(OBJ_DIR)/text.o \
$(OBJ_DIR)/list.o \
$(OBJ_DIR)/resource.o \
$(OBJ_DIR)/error.o \
$(OBJ_DIR)/timer.o \
$(OBJ_DIR)/thread.o \
$(OBJ_DIR)/font.o \
$(OBJ_DIR)/utext.o \
$(OBJ_DIR)/vfile/root.o \
$(OBJ_DIR)/dms/generic.o \
$(OBJ_DIR)/grfx2.o \
$(OBJ_DIR)/dms2.o \
$(OBJ_DIR)/dynld.o

APPOBJ = \
$(OBJ_DIR)/xapps/desktop.o \
$(OBJ_DIR)/xapps/ozone.o \
$(OBJ_DIR)/xapps/nav.o \
$(OBJ_DIR)/xapps/calc.o \
$(OBJ_DIR)/xapps/snote.o \
$(OBJ_DIR)/xapps/xtext.o \
$(OBJ_DIR)/xapps/regedit.o \
$(OBJ_DIR)/xapps/immagine.o \
$(OBJ_DIR)/xapps/setscr.o \
$(OBJ_DIR)/xapps/setmouse.o \
$(OBJ_DIR)/xapps/resedit.o \
$(OBJ_DIR)/xapps/about.o \
$(OBJ_DIR)/xapps/kedit.o \
$(OBJ_DIR)/xapps/setkeyb.o \
$(OBJ_DIR)/xapps/setwall.o \
$(OBJ_DIR)/xapps/settheme.o \
$(OBJ_DIR)/xapps/console.o \
$(OBJ_DIR)/xapps/rundos.o \
$(OBJ_DIR)/xapps/dfmsinfo.o \
$(OBJ_DIR)/xapps/openwth.o \
$(OBJ_DIR)/xapps/xsystem.o \
$(OBJ_DIR)/xapps/ssplay.o \
$(OBJ_DIR)/xapps/setvol.o \
$(OBJ_DIR)/xapps/setlib.o \
$(OBJ_DIR)/xapps/imgor.o \
$(OBJ_DIR)/xapps/run.o \
$(OBJ_DIR)/xapps/ttt.o \
$(OBJ_DIR)/xapps/setup.o \
$(OBJ_DIR)/xapps/welcome.o \
$(OBJ_DIR)/xapps/snake.o \
$(OBJ_DIR)/xapps/mines.o \
$(OBJ_DIR)/xapps/o3pad.o \
$(OBJ_DIR)/xapps/laby.o \
$(OBJ_DIR)/xapps/sample.o \
$(OBJ_DIR)/xapps/xbatch.o \
$(OBJ_DIR)/xapps/orchid.o \
$(OBJ_DIR)/xapps/login.o \
$(OBJ_DIR)/xapps/myip.o \
$(OBJ_DIR)/xapps/htmlget.o \
$(OBJ_DIR)/xapps/xtheme.o \
$(OBJ_DIR)/xapps/mbox.o
#$(OBJ_DIR)/xapps/ping.o \
#$(OBJ_DIR)/xapps/convmpeg.o \
#$(OBJ_DIR)/xdev/stars.o \
#$(OBJ_DIR)/xdev/aloha.o \
#$(OBJ_DIR)/xdev/orchid.o



APPLD = \
bin/xapps/desktop.app \
bin/xapps/ozone.app \
bin/xapps/nav.app \
bin/xapps/calc.app \
bin/xapps/snote.app \
bin/xapps/xtext.app \
bin/xapps/regedit.app \
bin/xapps/immagine.app \
bin/xapps/setscr.app \
bin/xapps/setmouse.app \
bin/xapps/resedit.app \
bin/xapps/about.app \
bin/xapps/kedit.app \
bin/xapps/setkeyb.app \
bin/xapps/setwall.app \
bin/xapps/settheme.app \
bin/xapps/console.app \
bin/xapps/term.app \
bin/xapps/rundos.app \
bin/xapps/dfmsinfo.app \
bin/xapps/openwth.app \
bin/xapps/xsystem.app \
bin/xapps/ssplay.app \
bin/xapps/setvol.app \
bin/xapps/setlib.app \
bin/xapps/imgor.app \
bin/xapps/run.app \
bin/xapps/ttt.app \
bin/xapps/setup.app \
bin/xapps/welcome.app \
bin/xapps/snake.app \
bin/xapps/mines.app \
bin/xapps/o3pad.app \
bin/xapps/laby.app \
bin/xapps/sample.app \
bin/xapps/xbatch.app \
bin/xapps/illkirch.app \
bin/xapps/orchid.app \
bin/xapps/mbox.app \
bin/xapps/login.app \
bin/xapps/myip.app \
bin/xapps/xtheme.app \
bin/xapps/htmlget.app
#bin/xapps/ping.app \
#bin/xapps/convmpeg.app \
#bin/xdev/aloha.app \
#bin/xdev/aorchid.app \
#bin/xdev/stars.app \




LIBOBJ = \
$(OBJ_DIR)/xlib/widget.o \
$(OBJ_DIR)/xlib/widget/iwidget.o \
$(OBJ_DIR)/xlib/widget/wclass.o \
$(OBJ_DIR)/xlib/widget/spliter.o \
$(OBJ_DIR)/xlib/skin.o \
$(OBJ_DIR)/xlib/bmp.o \
$(OBJ_DIR)/xlib/button.o \
$(OBJ_DIR)/xlib/label.o \
$(OBJ_DIR)/xlib/progress.o \
$(OBJ_DIR)/xlib/checkbox.o \
$(OBJ_DIR)/xlib/clipbrd.o \
$(OBJ_DIR)/xlib/window.o \
$(OBJ_DIR)/xlib/menu.o \
$(OBJ_DIR)/xlib/scrllbar.o \
$(OBJ_DIR)/xlib/listbox.o \
$(OBJ_DIR)/xlib/textbox.o \
$(OBJ_DIR)/xlib/canvas.o \
$(OBJ_DIR)/xlib/treeview.o \
$(OBJ_DIR)/xlib/groupbox.o \
$(OBJ_DIR)/xlib/slider.o \
$(OBJ_DIR)/xlib/listview.o \
$(OBJ_DIR)/xlib/iodlg.o \
$(OBJ_DIR)/xlib/combobox.o \
$(OBJ_DIR)/xlib/fprop.o \
$(OBJ_DIR)/xlib/tabbook.o \
$(OBJ_DIR)/xlib/cp.o \
$(OBJ_DIR)/xlib/toolbar.o \
$(OBJ_DIR)/xlib/prevscr.o \
$(OBJ_DIR)/xlib/conlib.o \
$(OBJ_DIR)/xlib/colsel.o \
$(OBJ_DIR)/xlib/fms2.o \
$(OBJ_DIR)/xlib/sound.o \
$(OBJ_DIR)/xlib/memfile.o \
$(OBJ_DIR)/xlib/grfx.o

LIBLD = \
bin/xlib/widget.dl \
bin/xlib/skin.dl \
bin/xlib/bmp.dl \
bin/xlib/clipbrd.dl \
bin/xlib/button.dl \
bin/xlib/label.dl \
bin/xlib/progress.dl \
bin/xlib/checkbox.dl \
bin/xlib/window.dl \
bin/xlib/menu.dl \
bin/xlib/scrllbar.dl \
bin/xlib/listbox.dl \
bin/xlib/textbox.dl \
bin/xlib/canvas.dl \
bin/xlib/treeview.dl \
bin/xlib/groupbox.dl \
bin/xlib/slider.dl \
bin/xlib/listview.dl \
bin/xlib/iodlg.dl \
bin/xlib/combobox.dl \
bin/xlib/fprop.dl \
bin/xlib/tabbook.dl \
bin/xlib/cp.dl \
bin/xlib/toolbar.dl \
bin/xlib/png.dl \
bin/xlib/mpeg2.dl \
bin/xlib/prevscr.dl \
bin/xlib/conlib.dl \
bin/xlib/colsel.dl \
bin/xlib/fms2.dl \
bin/xlib/grfx.dl \
bin/xlib/sound.dl \
bin/xlib/mp3.dl \
bin/xlib/memfile.dl \
bin/xlib/jpg.dl  

JPGOBJ = obj/dms/jpg/jpgalleg.o obj/dms/jpg/io.o obj/dms/jpg/encode.o obj/dms/jpg/decode.o obj/dms/jpg/color.o

PNGOBJ = obj/dms/png/png.o \
obj/dms/png/pngset.o \
obj/dms/png/pngget.o \
obj/dms/png/pngrutil.o \
obj/dms/png/pngtrans.o \
obj/dms/png/pngwutil.o \
obj/dms/png/pngread.o \
obj/dms/png/pngrio.o \
obj/dms/png/pngwio.o \
obj/dms/png/pngwrite.o \
obj/dms/png/pngrtran.o \
obj/dms/png/pngwtran.o \
obj/dms/png/pngmem.o \
obj/dms/png/pngerror.o \
obj/dms/png/pngpread.o \
obj/xlib/png.o

MP3OBJ=obj/dms/mp3/decoder/layer2.o \
obj/dms/mp3/decoder/layer3.o \
obj/dms/mp3/decoder/interfac.o \
obj/dms/mp3/decoder/decode_i.o \
obj/dms/mp3/decoder/dct64_i3.o \
obj/dms/mp3/decoder/common.o \
obj/dms/mp3/mp3.o \
obj/dms/mp3/decoder/tabinit.o

ILLKIRCHOBJ= \
obj/illkirch/main.o \
obj/illkirch/devwdg.o \
obj/illkirch/devclass.o \
obj/illkirch/about.o \
obj/illkirch/menuedit.o \
obj/illkirch/msgman.o \
obj/illkirch/viewprop.o

TCPOBJ=obj/dms/tcp/BSDNAME.O \
obj/dms/tcp/ELIB.O \
obj/dms/tcp/FRAGMENT.O \
obj/dms/tcp/INCHKSUM.O \
obj/dms/tcp/INTEL.O \
obj/dms/tcp/PCARP.O \
obj/dms/tcp/PCBOOTP.O \
obj/dms/tcp/PCBSD.O \
obj/dms/tcp/PCBUF.O \
obj/dms/tcp/PCCONFIG.O \
obj/dms/tcp/PCDBUG.O \
obj/dms/tcp/PCICMP.O \
obj/dms/tcp/PCPING.O \
obj/dms/tcp/PCPKT.O \
obj/dms/tcp/PCRECV.O \
obj/dms/tcp/PCSED.O \
obj/dms/tcp/PCSTAT.O \
obj/dms/tcp/PCTCP.O \
obj/dms/tcp/SELECT.O \
obj/dms/tcp/SOCKET.O \
obj/dms/tcp/SOCK_DBU.O \
obj/dms/tcp/SOCK_INI.O \
obj/dms/tcp/SOCK_PRN.O \
obj/dms/tcp/TEST.O \
obj/dms/tcp/UDP_DOM.O \
obj/dms/tcp/UDP_NDS.O \
obj/dms/tcp/VSSCANF.O \
obj/dms/tcp/KBHIT.O \
obj/dms/tcp/_MODDI3.O \
obj/dms/watt/netaddr.o \
obj/dms/watt/neterr.o \
obj/dms/watt/getopt.o \
obj/dms/watt/sock_ini.o \
obj/dms/watt/lookup.o \
obj/dms/watt/misc.o \
obj/dms/watt/version.o \
obj/dms/tcp/_DIVDI3.O

WATTOBJ=obj/dms/watt/chksum0.o \
obj/dms/watt/cpumodel.o \
obj/dms/watt/accept.o \
obj/dms/watt/adr2asc.o \
obj/dms/watt/asc2adr.o \
obj/dms/watt/bind.o \
obj/dms/watt/bsddbug.o \
obj/dms/watt/bsdname.o \
obj/dms/watt/btree.o \
obj/dms/watt/chksum.o \
obj/dms/watt/close.o \
obj/dms/watt/connect.o \
obj/dms/watt/country.o \
obj/dms/watt/crc.o \
obj/dms/watt/echo.o \
obj/dms/watt/fcntl.o \
obj/dms/watt/fortify.o \
obj/dms/watt/get_ai.o \
obj/dms/watt/get_ni.o \
obj/dms/watt/geteth.o \
obj/dms/watt/gethost.o \
obj/dms/watt/gethost6.o \
obj/dms/watt/getname.o \
obj/dms/watt/getnet.o \
obj/dms/watt/getopt.o \
obj/dms/watt/getprot.o \
obj/dms/watt/getput.o \
obj/dms/watt/getserv.o \
obj/dms/watt/gettod.o \
obj/dms/watt/ioctl.o \
obj/dms/watt/ip4_frag.o \
obj/dms/watt/ip4_in.o \
obj/dms/watt/ip4_out.o \
obj/dms/watt/ip6_in.o \
obj/dms/watt/ip6_out.o \
obj/dms/watt/language.o \
obj/dms/watt/linkaddr.o \
obj/dms/watt/listen.o \
obj/dms/watt/lookup.o \
obj/dms/watt/loopback.o \
obj/dms/watt/misc.o \
obj/dms/watt/netaddr.o \
obj/dms/watt/netback.o \
obj/dms/watt/neterr.o \
obj/dms/watt/nettime.o \
obj/dms/watt/nsapaddr.o \
obj/dms/watt/oldstuff.o \
obj/dms/watt/pc_cbrk.o \
obj/dms/watt/pcarp.o \
obj/dms/watt/pcbootp.o \
obj/dms/watt/powerpak.o \
obj/dms/watt/pcbuf.o \
obj/dms/watt/pcconfig.o \
obj/dms/watt/pcdbug.o \
obj/dms/watt/pcdhcp.o \
obj/dms/watt/pcicmp.o \
obj/dms/watt/pcicmp6.o \
obj/dms/watt/pcintr.o \
obj/dms/watt/pcmulti.o \
obj/dms/watt/pcping.o \
obj/dms/watt/pcpkt.o \
obj/dms/watt/pcpkt32.o \
obj/dms/watt/pcqueue.o \
obj/dms/watt/pcrarp.o \
obj/dms/watt/pcrecv.o \
obj/dms/watt/pcsed.o \
obj/dms/watt/pcstat.o \
obj/dms/watt/pctcp.o \
obj/dms/watt/poll.o \
obj/dms/watt/ports.o \
obj/dms/watt/ppp.o \
obj/dms/watt/pppoe.o \
obj/dms/watt/presaddr.o \
obj/dms/watt/printk.o \
obj/dms/watt/qmsg.o \
obj/dms/watt/receive.o \
obj/dms/watt/res_comp.o \
obj/dms/watt/res_data.o \
obj/dms/watt/res_debu.o \
obj/dms/watt/res_init.o \
obj/dms/watt/res_loc.o \
obj/dms/watt/res_mkqu.o \
obj/dms/watt/res_quer.o \
obj/dms/watt/res_send.o \
obj/dms/watt/select.o \
obj/dms/watt/settod.o \
obj/dms/watt/shutdown.o \
obj/dms/watt/signal.o \
obj/dms/watt/sock_dbu.o \
obj/dms/watt/sock_in.o \
obj/dms/watt/sock_ini.o \
obj/dms/watt/sock_io.o \
obj/dms/watt/sock_prn.o \
obj/dms/watt/sock_scn.o \
obj/dms/watt/sock_sel.o \
obj/dms/watt/socket.o \
obj/dms/watt/sockopt.o \
obj/dms/watt/split.o \
obj/dms/watt/stream.o \
obj/dms/watt/strings.o \
obj/dms/watt/syslog.o \
obj/dms/watt/syslog2.o \
obj/dms/watt/tcp_fsm.o \
obj/dms/watt/get_xbyr.o \
obj/dms/watt/tftp.o \
obj/dms/watt/timer.o \
obj/dms/watt/transmit.o \
obj/dms/watt/udp_dom.o \
obj/dms/watt/udp_rev.o \
obj/dms/watt/version.o \
obj/dms/watt/fsext.o \
obj/dms/watt/wdpmi.o \
obj/dms/watt/x32vm.o \
obj/dms/watt/rs232.o \
obj/dms/watt/get_ip.o \
obj/dms/watt/pcsarp.o \
obj/dms/watt/idna.o \
obj/dms/watt/punycode.o \
obj/dms/watt/tcp_md5.o \
obj/dms/watt/dynip.o \
obj/dms/watt/winpcap.o \
obj/dms/watt/winmisc.o \
obj/dms/watt/packet32.o \
obj/dms/watt/adler32.o \
obj/dms/watt/compress.o \
obj/dms/watt/crc32.o \
obj/dms/watt/gzio.o \
obj/dms/watt/uncompr.o \
obj/dms/watt/deflate.o \
obj/dms/watt/trees.o \
obj/dms/watt/zutil.o \
obj/dms/watt/inflate.o \
obj/dms/watt/infback.o \
obj/dms/watt/inftrees.o \
obj/dms/watt/inffast.o

MPEGOBJ=obj/dms/mpeg2/mpeg2.o 


all : $(OBJ_DIR)/dynldlnk.o $(DYNLDLNK) $(LIBOBJ) $(LIBLD) $(APPOBJ) $(ILLKIRCHOBJ) $(APPLD) $(OBJ_LIST1) $(OBJ_LIST2) $(MAIN)
	@echo Aura M6 0.9.8.0 has been built.

$(DYNLDLNK) : obj/dynldlnk.o
	gcc -g -o $(DYNLDLNK) obj/dynldlnk.o $(LIB_DYNLNK)

$(MAIN) : $(OBJ_LIST1) $(OBJ_LIST2)
	gcc -g -o $(MAIN) $(OBJ_LIST1) src/xapps/TIMERASM.o $(OBJ_LIST2) $(LIB_LIST)

$(OBJ_DIR)/xlib/%.o: src/xlib/%.c
	gcc $(CFLAGS) -Dfar= -Dnear= -Dhuge= -c $< -o $@

src/xapps/TIMERASM.o: src/xapps/TIMERASM.ASM
	nasm $(AFLAGS) $<

$(OBJ_DIR)/xapps/mbox.o: src/xapps/mbox.c
	gcc $(CFLAGS) -c $< -o $@
#trying to link meminfo to term

#$(OBJ_DIR)/xapps/meminfo.o: src/xapps/meminfo.c include/meminfo.h
#	gcc $(CFLAGS) -c $< -o $@

#$(OBJ_DIR)/xapps/meminfo.o: $(OBJ_DIR)/xapps/term.o
#	gcc $(LFLAGS) $^ -o $@

$(OBJ_DIR)/exports.o: src/exports.c src/xapps/TIMERASM.o
	gcc $(CFLAGS) -c $< -o $@ src/xapps/TIMERASM.o

$(OBJ_DIR)/xapps/myip.o: src/xapps/myip.c
	gcc $(CFLAGS) -c $< -o $@ -lwatt

$(OBJ_DIR)/xapps/%.o: src/xapps/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/illkirch/%.o: src/illkirch/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/desktop/%.o: src/xapps/desktop/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/xlib/%.o: src/dms/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/xlib/widget/%.o: src/dms/widget/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/vfile/%.o: src/vfile/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/xapps/command.o: src/xapps/command.c include/command.h
	$(CC) $(CFLAGS) -c /src/xapps/command.c -o $(OBJ_DIR)/xapps/command.o

$(OBJ_DIR)/xapps/sys.o: src/xapps/sys.c include/command.h
	$(CC) $(CFLAGS) -c /src/xapps/sys.c -o $(OBJ_DIR)/xapps/sys.o	

obj/dms/png/%.o: src/dms/png/%.c
	gcc $(CFLAGS) -c $< -o $@

obj/dms/mpeg2/%.o: src/dms/mpeg2/%.c
	gcc $(CFLAGS) -c $< -o $@

obj/dms/mp3/decoder/%.o: src/dms/mp3/decoder/%.c
	gcc $(CFLAGS) -c $< -o $@

obj/dms/mp3/%.o: src/dms/mp3/%.c
	gcc $(CFLAGS) -c $< -o $@

obj/dms/jpg/%.o: src/dms/jpg/%.c
	gcc $(CFLAGS) -c $< -o $@

obj/dms/jpg/%.o: src/dms/jpg/i386/%.s
	$(CC) -g -x assembler-with-cpp -Iinclude -c $< -o $@

bin/xlib/jpg.dl: $(JPGOBJ)
	$(DYNLDLNK) bin/xlib/jpg.dl $(JPGOBJ)

bin/xlib/mp3.dl: $(MP3OBJ)
	$(DYNLDLNK) bin/xlib/mp3.dl $(MP3OBJ)

bin/xlib/png.dl: $(PNGOBJ)
	$(DYNLDLNK) bin/xlib/png.dl $(PNGOBJ)

bin/xlib/mpeg2.dl: $(MPEGOBJ)
	$(DYNLDLNK) bin/xlib/mpeg2.dl $(MPEGOBJ)

bin/xapps/illkirch.app: $(ILLKIRCHOBJ) res/illkirch.res
	$(DYNLDLNK) -Rres/illkirch.res bin/xapps/illkirch.app $(ILLKIRCHOBJ)

bin/xapps/setscr.app: $(OBJ_DIR)/xapps/setscr.o res/setscr.res
	$(DYNLDLNK) -Rres/setscr.res bin/xapps/setscr.app $(OBJ_DIR)/xapps/setscr.o

bin/xapps/ssplay.app: $(OBJ_DIR)/xapps/ssplay.o res/ssplay.res
	$(DYNLDLNK) -Rres/ssplay.res bin/xapps/ssplay.app $(OBJ_DIR)/xapps/ssplay.o

bin/xapps/run.app: $(OBJ_DIR)/xapps/run.o res/run.res
	$(DYNLDLNK) -Rres/run.res bin/xapps/run.app $(OBJ_DIR)/xapps/run.o

bin/xapps/console.app: $(OBJ_DIR)/xapps/console.o res/cons.res
	$(DYNLDLNK) -Rres/cons.res bin/xapps/console.app $(OBJ_DIR)/xapps/console.o

bin/xapps/term.app: $(OBJ_DIR)/xapps/term.o $(OBJ_DIR)/xapps/command.o $(OBJ_DIR)/xapps/sys.o res/cons.res
	$(DYNLDLNK) -Rres/term.res bin/xapps/term.app $(OBJ_DIR)/xapps/term.o $(OBJ_DIR)/xapps/command.o $(OBJ_DIR)/xapps/sys.o



bin/xapps/about.app: $(OBJ_DIR)/xapps/about.o res/about.res
	$(DYNLDLNK) -Rres/about.res bin/xapps/about.app $(OBJ_DIR)/xapps/about.o

bin/xapps/welcome.app: $(OBJ_DIR)/xapps/welcome.o res/welcome.res
	$(DYNLDLNK) -Rres/welcome.res bin/xapps/welcome.app $(OBJ_DIR)/xapps/welcome.o

bin/xapps/sample.app: $(OBJ_DIR)/xapps/sample.o res/sample.res
	$(DYNLDLNK) -Rres/welcome.res bin/xapps/sample.app $(OBJ_DIR)/xapps/sample.o

bin/xapps/snake.app: $(OBJ_DIR)/xapps/snake.o res/snake.res
	$(DYNLDLNK) -Rres/snake.res bin/xapps/snake.app $(OBJ_DIR)/xapps/snake.o

bin/xapps/ttt.app: $(OBJ_DIR)/xapps/ttt.o
	$(DYNLDLNK) bin/xapps/ttt.app $(OBJ_DIR)/xapps/ttt.o

bin/xapps/laby.app: $(OBJ_DIR)/xapps/laby.o
	$(DYNLDLNK) bin/xapps/laby.app $(OBJ_DIR)/xapps/laby.o

bin/xapps/orchid.app: $(OBJ_DIR)/xapps/orchid.o
	$(DYNLDLNK) bin/xapps/orchid.app $(OBJ_DIR)/xapps/orchid.o

bin/xapps/mbox.app: $(OBJ_DIR)/xapps/mbox.o res/ssplay.res
	$(DYNLDLNK) -Rres/ssplay.res bin/xapps/mbox.app $(OBJ_DIR)/xapps/mbox.o

bin/xapps/login.app: $(OBJ_DIR)/xapps/login.o
	$(DYNLDLNK) bin/xapps/login.app $(OBJ_DIR)/xapps/login.o

bin/xapps/convmpeg.app: $(OBJ_DIR)/xapps/convmpeg.o
	$(DYNLDLNK) bin/xapps/convmpeg.app $(OBJ_DIR)/xapps/convmpeg.o

bin/xapps/mines.app: $(OBJ_DIR)/xapps/mines.o
	$(DYNLDLNK) bin/xapps/mines.app $(OBJ_DIR)/xapps/mines.o

bin/xapps/ping.app: $(OBJ_DIR)/xapps/ping.o
	$(DYNLDLNK) bin/xapps/ping.app $(OBJ_DIR)/xapps/ping.o

bin/xapps/o3pad.app: $(OBJ_DIR)/xapps/o3pad.o
	$(DYNLDLNK) bin/xapps/o3pad.app $(OBJ_DIR)/xapps/o3pad.o

bin/xapps/setup.app: $(OBJ_DIR)/xapps/setup.o res/setup.res
	$(DYNLDLNK) -Rres/setup.res bin/xapps/setup.app $(OBJ_DIR)/xapps/setup.o

bin/xapps/imgor.app: $(OBJ_DIR)/xapps/imgor.o res/imgor.res
	$(DYNLDLNK) -Rres/imgor.res bin/xapps/imgor.app $(OBJ_DIR)/xapps/imgor.o

bin/xapps/myip.app: $(OBJ_DIR)/xapps/myip.o 
	$(DYNLDLNK) bin/xapps/myip.app $(OBJ_DIR)/xapps/myip.o

bin/xapps/htmlget.app: $(OBJ_DIR)/xapps/htmlget.o
	$(DYNLDLNK) bin/xapps/htmlget.app $(OBJ_DIR)/xapps/htmlget.o

bin/xapps/xtheme.app: $(OBJ_DIR)/xapps/xtheme.o
	$(DYNLDLNK) bin/xapps/xtheme.app $(OBJ_DIR)/xapps/xtheme.o

#bin/xdev/orchid.app: $(OBJ_DIR)/xdev/orchid.o 
#	$(DYNLDLNK) bin/xdev/orchid.app $(OBJ_DIR)/xdev/orchid.o	

#bin/xdev/aloha.app: $(OBJ_DIR)/xdev/aloha.o 
#	$(DYNLDLNK) bin/xdev/aloha.app $(OBJ_DIR)/xdev/aloha.o	

#bin/xdev/stars.app: $(OBJ_DIR)/xdev/stars.o 
#	$(DYNLDLNK) bin/xdev/stars.app $(OBJ_DIR)/xdev/stars.o	

bin/xapps/%.app: $(OBJ_DIR)/xapps/%.o
	$(DYNLDLNK) $@ $<

bin/xlib/fms2.dl: $(OBJ_DIR)/xlib/fms2.o res/fms2.res
	$(DYNLDLNK) -Rres/fms2.res bin/xlib/fms2.dl $(OBJ_DIR)/xlib/fms2.o

bin/xlib/prevscr.dl: $(OBJ_DIR)/xlib/prevscr.o res/prvscr.res
	$(DYNLDLNK) -Rres/prvscr.res bin/xlib/prevscr.dl $(OBJ_DIR)/xlib/prevscr.o

bin/xlib/cp.dl: $(OBJ_DIR)/xlib/cp.o res/cp.res
	$(DYNLDLNK) -Rres/cp.res bin/xlib/cp.dl $(OBJ_DIR)/xlib/cp.o

bin/xlib/widget.dl: $(OBJ_DIR)/xlib/widget.o $(OBJ_DIR)/xlib/widget/iwidget.o $(OBJ_DIR)/xlib/widget/wclass.o $(OBJ_DIR)/xlib/widget/spliter.o
	$(DYNLDLNK) bin/xlib/widget.dl $(OBJ_DIR)/xlib/widget.o $(OBJ_DIR)/xlib/widget/iwidget.o $(OBJ_DIR)/xlib/widget/wclass.o $(OBJ_DIR)/xlib/widget/spliter.o

bin/xlib/%.dl: $(OBJ_DIR)/xlib/%.o
	$(DYNLDLNK) $@ $<

#cpux is for the interrupt to check cpu performance.
cpu.com: cpu.asm
	
	nasm -f bin cpu.asm -o cpux.com 


clean:
	del c:/*.o
	del c:/xlib/*.o
	del c:/xlib/widget/*.o
	del c:/xapps/*.o
	del c:/dms/*.o
	del c:/dms/png/*.o
	del c:/dms/jpg/*.o
	del c:/dms/jpg/*.o
	del c:/dms/mp3/*.o
	del c:/dms/mp3/decoder*.o
	del c:/vfile/*.o
	del c:/bin/xapps/*.app
	del c:/bin/xlib/*.dl
