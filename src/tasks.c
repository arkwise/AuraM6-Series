/**
*	(c) Copyright 2003,2004 Point Mad, Lukas Lipka. All rights reserved.
*
*	FILE:			tasks.c
*
*	PROJECT:		Phoenix engine - Core
*
*	DESCRIPTION:	Tasks handling and switching.
*
*	CONTRIBUTORS:
*					Lukas Lipka
*
*	TODO:			@
*
*	BUG:			@
*
*	MISTAKE:		@
*
*	IDEA:			@
*/
#define KERNEL_FILE
#include "kernel.h"

PTask   TaskList = NULL;
PTask   LastTask = NULL;

PTask InstallTask(void (*Task)(PTask Task))
{
	PTask t 	= NEW(TTask);
	t->Enabled	= TRUE;
	t->Task		= Task;
	t->Next		= NULL;
	t->Prev		= LastTask;

	if (!TaskList)
		TaskList = t;
	else
		LastTask->Next = t;

	LastTask = t;

	return t;
}

void RemoveTask(PTask Task)
{
	if (!Task)
		return;

	if (Task->Prev) Task->Prev->Next = Task->Next;
	else TaskList = Task->Next;

	if (Task->Next) Task->Next->Prev = Task->Prev;
	else LastTask = Task->Prev;

	free(Task);
}

void PollTasks(void)
{
	PTask p = TaskList;

	while (p)   // not nice code?
	{
		if (p->Enabled) p->Task(p);

		p = p->Next;
	}
}

/*
void RunTasks(void *arg)
{
	while ( 1 )
	{
		PollTasks();

		Yield();
	}
}*/
