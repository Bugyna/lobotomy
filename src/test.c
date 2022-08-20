#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>




typedef struct SCOPE SCOPE;
typedef struct OBJ OBJ;
typedef struct TREE TREE;
typedef struct FUNC FUNC;
typedef struct LIST LIST;


struct LIST
{
	int index; OBJ* list;
};


struct OBJ
{
	int type, index;
	char* name;

	union
	{
		char* str;
		int64_t number;
		double decimal;
		OBJ(*func)(OBJ);
		OBJ* list;
		OBJ* ref;
		OBJ** args;
	};
};


void do_shit(OBJ* obj)
{
	printf("name: %s\n", obj->name);
	OBJ tmp;
	tmp.name = "aaa";
	*obj = tmp;
	printf("name: %s\n", obj->name);
	
}

void a(OBJ* obj)
{
	printf("name: %s\n", obj->name);
	OBJ tmp;
	tmp.name = "aaa";
	(obj[2]) = tmp;
	printf("name: %s\n", obj->name);
}

int main()
{
	OBJ obj;
	OBJ a;
	a.name = "f";
	OBJ* list = malloc(20*sizeof(OBJ));
	obj.name = "name";
	list[2] = obj;
	list[1] = a;
	do_shit(&list[2]);
	// a(list);
	printf("name: %s %s\n", list[2].name, list[1].name);
	
	return 0;
}