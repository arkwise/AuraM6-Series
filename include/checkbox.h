#ifndef _CHECKBOX_H_INCLUDED_
#define _CHECKBOX_H_INCLUDED_

typedef struct TCheckbox *PCheckbox;
typedef struct TCheckbox
{
	struct TWidget o;

	l_bool Checked;
	char *Caption;
} TCheckbox;

PCheckbox CreateCheckbox(PApplication App, TRect r, l_text Caption);

#define CHECKBOX(o) ((PCheckbox)(o))

#endif /* _CHECKBOX_H_INCLUDED_ */
