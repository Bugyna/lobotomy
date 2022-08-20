#pragma once



typedef struct OBJ OBJ;


struct OBJ
{
	int type;
	char* name;
	int ref_count;

	union
	{
		char* str;
		int64_t num;
		double decimal;
		OBJ(*func)(OBJ);
		struct {int index, size; OBJ* list;};
		OBJ* ref;
	};
};


// OBJ* GC;


// void run_gc(OBJ** GC)
// {
	// for (int i = 0; i < GC->index; i++) {
		// if (GC[i]->ref_count == 0) {
			// GC[i] = NULL;
		// }
	// }
// }