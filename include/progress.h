typedef struct TProgressBar *PProgressBar;
typedef struct TProgressBar
{
	/** Inherited functions and variables from TWidget **/
	struct	TWidget o;

	/** Number of steps to be done till 100% **/
	l_ulong	Steps;

	/** Number of steps done, since start **/
	l_ulong	Promile;

	/** Increases the promile variable, adding %Steps each time **/
	l_int (*Step)(PProgressBar b, l_int Steps);

} TProgressBar;

#define PROGRESSBAR(w)	((PProgressBar)(w))

PProgressBar CreateProgressBar(PApplication App,TRect r, l_uint Steps);
