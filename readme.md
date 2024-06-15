
this is for the auram5new folder only. this should be the latest code from kdog.


# Aura M6
# Tech 6 Kernel 0.9.8.0
# 20 August 2023 

# development build 4000 series #

Welcome to the developer release of aura milestone 6.

M6 is basically going to be a beta version of m5 with more apps being released. i would like to fix the ability for the multi threading to work again so that the tcp apps dont hang the system like the presently do.

orchid browser has been built using curl, xmlparsing, watt32 and a few other libraries. it has yet to be tested. also an app store which i am also planning to add the ability to download freedos packages again.

there is also a irc chat server and client in development. most of this work has been done using chatgpt so it will still have errors but this has done a lot of the ground work in terms of me drawing an app UI and then linking the functions to the elements as required.

once this work is done aura will pretty much be a half decent platform to develop for the FREEDOS operating system.


# New features #

Judas HD audio library
this has been added for testing AC97 and iCH audio chipsets. unfortunately this will only output sound from the headphone jack and not speakers on laptops. kind of works but with errors but were getting there.

WATT32
the watt32 library has been fully implemented  for use with all aura xapps. while i considered using mTCP which is a great stack too i will just stay with watt32 for now. you could even still use both.

xDEV 
these are the new apps that are being built with the tcp stack.

this is for the auram5new folder only. this should be the latest code from kdog.



# Multithreading #
fix multithreading in main.c. this will need a serious looking at so that aura can handle multiple applications at once.

the second part wil be adding this to all the applications at a later date or just the newer tcp apps.
 main.c
 exports.c




# Aura + DESKTOP bugs #

#1
build new login at boot for users (this has already been added to the registry so it can handle different users.this will be a replacement for the welcome widget that used to load at boot in desktop.c

#2
fix windows being minimised on the desktop. perhaps send them to the back of the wallpaper app until it is clicked on by the user on the task bar to bring it forward.

#3
messagebox functions in desktop.c need to be able to use iwidget before the desktop is loaded.

#5
new setup application based on the desktop selection manager on boot.

third option will be to install aura SDK and updates.

this wont be too hard (chelson) will be working on this project

#6
we need to fix the file system issues where the code should be telling the user dates of the files, sizes and filetypes etc. this issue is in nav also. 

#7
wallpaper app is broken.. seems to crash?

#8 
screen resolutions not working?

#9
missing images/icons in nav

# 10 CROSS COMPILING?
fix the makefile to build the aura xdev apps from linux if they have dependencies. this will probably be a btter option to try and build this under linux to take advantage of the libraries.

ideally we need some programs to build under DOS fine but also allowing them to build under linux for better library support will help a lot.


# file system
we need to add support for:

#11
.com,.exe,.bat files again

.c, .h, html

.py and others.


#6
we need to fix the file system issues where the code should be telling the user dates of the files, sizes and filetypes etc. this issue is in nav also. 






# illkirch E (final alpha?) #
basically in its current state illkirch d will save iwidget versions of a UI. we eventually need to make this generate code to some degree just so users can draw their app user interfaces and add functions later.

in a beta version it would be nice to test the apps through iwidget with functionality but thats a long way a way.

for example the code really only needs to draw these options for now just to speed things up in terms of ui development

# 1 drawing a window with a title
        RectAssign(&r, 0, 0, 255, 90);
        w = CreateWindow(&Me, r, "Welcome", WF_FRAME | WF_CAPTION | WF_MINIMIZE | WF_CENTERED);
        InsertWidget(DeskTop, WIDGET(w));
        WIDGET(w)->AppEvHdl = &AppEventHandler;

# 2 drawing a text lable
        RectAssign(&r, 5, 0, 250, 15);
        l = CreateLabel(&Me, r, "Welcome to Aura, ");
        WIDGET(l)->BackgroundColor = COL_3DFACE;
        InsertWidget(WIDGET(w), WIDGET(l));

# 3 another text lable below the other ^
        RectAssign(&r, 5, 20, 250, 35);
        l = CreateLabel(&Me, r, "Which desktop interface would you like to use?");
        WIDGET(l)->BackgroundColor = COL_3DFACE;
        InsertWidget(WIDGET(w), WIDGET(l));
# 4 how to draw a button and then link to the function BTN_Ok2 etc
        RectAssign(&r, 5, 40, 250, 60);
        b = CreateButton(&Me, r, "Aura Desktop", BTN_Ok2);
        InsertWidget(WIDGET(w), WIDGET(b));

        RectAssign(&r, 5, 65, 250, 85);
        b = CreateButton(&Me, r, "Ozone Desktop", BTN_Ok);
        InsertWidget(WIDGET(w), WIDGET(b));

        WidgetDrawAll(WIDGET(w));
        return true;

# 5 the functions linkined to the buttons.

   case BTN_Ok:
        {
                FileRun("xapps/ozone.app", NULL);
                WidgetDispose(WIDGET(w));
                return true;
        }

   case BTN_Ok2:
        {
                WidgetDispose(WIDGET(w));
                Livecd();
                return true;
        }

# drawing a new menu #

Menu2 = NewMenu(
            NewMenuItemEx("Games", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/games",
                          NewMenuItemEx("Media", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/media",
                                        NewMenuItemEx("Office", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/tools",
                                                      NewMenuItemEx("Extra", NULL, 0, 0, NULL, &GenerateSubMenu, "SYSTEM/MENU/extra",
                                                                    NewMenuItem("About", NULL, IDM_NAV, 0, NULL, NULL)))))); 





usage notes:

-safemode is safe mode
-resetup is a kernel reinit
alt + F8 is screenshot with mouse
alt + f12 = screenshot no mouse
ctrl + F = nav/file manager
ctrl + R = regedit
ctrl + X = quit but this is broken (crashes before the dialog box is displayed this is probably due to iwidgets being a bit broken)

version setup:

dynld.c will set the kernel version


 # OLD STUFF & GOALS + DEVELOOPMENT #




# Ozone & Aura Desktop for DOS
this project is a continuation of aura gui and ozone gui's code base being merged together (as many of the aura apps were not public code).
basically we are going to make this into a stand alone GUI/SHELL for freeDOS and other dos's that has net applications and is to encourge a community reach and education of the dos operating systems.

shells and graphical user interfaces are an important thing in modern computing and this project aims to bring a linux like shell/gui to the freedos system.

the original works were done by lukas lipka, point mad, floria xaiver, chase finn (finn tech) and chelson aitcheson (doscore).

Aura always was opensource https://sourceforge.net/projects/auraguifreedos/ but this code was messy, buggy and not maintained very well. this new project is much cleaner code and everything is working.

# GOALS
OUr mission is to make Aura/ozone desktop a modern usable retro platform that allows users to connect online easily and develop dos and aura applications that can be shared and developed online new and old system.

# 1 TCP (Already DONE√)
TCP library (wattcp) has been added to the "aura m5" kernel which requires a ne2000 driver or any packet driver to run the aura kernel and connect to the internet.
the apps htmlget, myip and PING are NOT public license but i (chelson) have shared them anyway in order for people to make their own apps in the future.

# 2 Judas Library HD ac97 audio (azalia) (Working with errors √)
the plan is to add this to the aura kernel in the next coming weeks. i have been testing this on real hardware with some pretty good success from this project https://github.com/volkertb/JUDAS but i have also made a large amount of updates for more hardware devices.

# 3 netplay, doslife online games service (in development)
earlier in 2020 we built a tic tac toe game that can be played via server and over LAN. the basic logic was figured out and we will add this game to aura gui. one of the goals i would like to see is people create online games and chat services etc like the good old msn zone days.

while i am working on the Dead Sea's game i am using some of the online game features we are writing for use on the doslife service which will give coders an API to allow them to create simple dos games that can connect to the doscore server no matter how basic or primitive the games are.

having an API, SDK and a platform for people to create and share DOS based games and entertainment could make dos a popular retro platform with a purpose!

# 4 Aura SDK (kind of done-ish)
while the code will always be hosted here i am planing on releasing a zip, bootable cd/USB SDK that has a pre built DJGPP development environment with all the required libraries ready to rock and roll to encourage people to develop games using the kit and the Doslife API.

the illkirch development app needs to generate code but this inst too hard.

# 5 Seal 2 GUI
we will also compiled the seal 2 gui and going to dub it as 2.1.3 or something with, judas, doslife and tcp also included but this will be later.

# conclusion
while i love freedos and old and new computers aura will aim to be a modern linux like shell/gui for use on modern systems more so than older systems. trust me when i say it should work fine on most old systems, my test system is a pentium 1 mmx with 32mb ram and it runs fine. so this as the base line we should cover most modern computers.

# want to help but you cant code?
if you love these kinds of projects and. want to see some cool development happen for freedos and gui/shell developments then fund us!
https://www.patreon.com/chelsonofdoscore

all the funds you contribute will help bring this to reality. i love dos and you do thats why you are here but sometimes we dont have the spare time or the know how to code everything. if we can get this to a point where we have a great api and SDK many people will be able to code thier own games
