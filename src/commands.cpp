#include "terminal.h"

#ifdef _DEBUG
//extern const dbg_entry testEntry;
extern const dbg_entry buzzerEntry;
extern const dbg_entry numberEntry;
//extern const dbg_entry loEntry;
//extern const dbg_entry hiEntry;


const dbg_entry* dbg_entries[] =
{
		&helpEntry,
//		&testEntry,
		&buzzerEntry,
		&numberEntry,
//		&loEntry,
//		&hiEntry,
		0
};

#endif
