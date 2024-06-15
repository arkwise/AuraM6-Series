#ifndef _TASK_H_INCLUDED_
#define _TASK_H_INCLUDED_

typedef struct TTask *PTask;
typedef struct TTask
{
  l_bool         Enabled;
  void          (*Task)(PTask Task);

  PTask         Prev;
  PTask         Next;
} TTask;

#define ENABLE_TASK(t)  t->Enabled = TRUE
#define DISABLE_TASK(t) t->Enabled = FALSE

/* that generates events without handling them and runs the
 tasks in tasklist. It's HIGHLY recommended to add this in
 big loops */
#define KEEP_MULTITASK	Yield();

/* that adds a task to the task list */
PTask   InstallTask(void (*Task)(PTask Task));

/* that removes a task from the task list */
void    RemoveTask(PTask Task);

/* that keeps multitasking. Use KEEP_MULTITASK macro to keep multitasking alive
 when you're using big loops. Instead of big loops, it's recommended to add a
 temporary task */
void  RunTasks(void *arg);


void  PollTasks(void);

/* tasklist contains all installed tasks */
extern PTask    TaskList;

#endif
