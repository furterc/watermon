#include "terminal.h"

extern const dbg_entry checkEntry;
extern const dbg_entry sendEntry;
extern const dbg_entry numberEntry;
extern const dbg_entry slaveHostEntry;
//extern const dbg_entry hiEntry;


const dbg_entry* dbg_entries[] =
{
		&helpEntry,
		&checkEntry,
		&sendEntry,
		&numberEntry,
		&slaveHostEntry,
//		&hiEntry,
		0
};

