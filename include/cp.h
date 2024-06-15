

#ifndef _CP_H_INCLUDED_
#define _CP_H_INCLUDED_

typedef struct TCPPanel {
	
	l_text Name;
	
	l_text Command;
	l_text Args;
	
	/*
	p_bitmap Icon16;
	p_bitmap Icon32;
	p_bitmap Icon48;
	*/
	PIcon Icon;
	l_bool (*Run)(void);
	
} TCPPanel, *PCPPanel;

#define CPPANEL(o) ((PCPPanel)(o))


#endif
