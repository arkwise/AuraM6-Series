#ifndef COMMAND_H
#define COMMAND_H

#include "kernel.h"

void CommandExit();
void CommandCd(int argc, l_text *argv, l_text CurrentDir);
void CommandDir(int argc, l_text *argv, l_text CurrentDir);
void CommandMkdir(int argc, l_text *argv, l_text CurrentDir);
void CommandCopy(int argc, l_text *argv);
void CommandHelp();
void CommandLsPci();
void CommandColors();
void CommandEcho(int argc, l_text *argv);
void CommandRunScript(int argc, l_text *argv);
void Commanddf(); // Added CommandDF prototype

// Add more Command function prototypes as necessary

#endif // COMMAND_H
