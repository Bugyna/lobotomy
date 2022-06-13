#include "tokenize.c"

enum {
	T_INT=0,
	T_BOOL,
	T_CHAR,
	T_STR,
	T_DOUBLE,
};


int get_type_size(int type)
{
	switch (type) {
		case T_INT:
			return 4;
		
		case T_BOOL:
			return 1;
		
		case T_CHAR:
			return 1;

		case T_STR:
			return 8;

		case T_DOUBLE:
			return 8;
	}

	return 0;
}

typedef struct
{
	int type;
	int size;

	union {
		int value;
		char c;
		char* str;
		double d;
	} val;
} OBJ;


OBJ create_obj(int value)
{
	OBJ obj;
	obj.type = T_INT;
	obj.size = value;
	obj.val.value = value;
	return obj;
}


typedef struct
{
	int len;
	int8_t data[2];
} BIG_NUM;

void bignum_from_int(BIG_NUM* bn, int n)
{
	// 0000 0000 0000 0000 0000 0000 0000 1000
	// 0x0 0x0 0x0 0x8
	// 8

	// 0000 0000 0000 0000
	printf("c: %d, %d\n", n, n >> 8);
	bn->data[1] = n;
	bn->data[0] = n >> 8;
}


char* bignum_to_str(BIG_NUM bn)
{
	char* s = malloc(16);
	// for (int i = 15; i > 0; i--) {
	// snprintf(s, 16, "%d", bn.data[1]);
	snprintf(s, 16, "%d", bn.data[1] & (bn.data[0] << 8));
	// }

	return s;
}

int main (int argc, char* argv[]) {

	if (argc > 1) {
		printf("no: %s\n", *argv);
	}

	BIG_NUM bn;
	bignum_from_int(&bn, 128);
	printf("%d, %d, %s", bn.data[0], bn.data[1], bignum_to_str(bn));

	// char t[] = "(+ 2 2)";
	// printf("t: %s, %ld\n", t, sizeof(long double));
	// for (int i = 0; i < sizeof t; i++) {
		// printf("c: %c, %d\n", t[i], i);

	// }

	// printf("\n");
	// long long double* test = calloc(1, 64);
	// intmax_t test = pow(2, 1023);
	// *test = pow(2, 1024);
	// printf("test: %Le\n", test);
	// printf("test: %Le\n", LDBL_MAX);	
	// printf("test: %Le\n", (long double)pow(2, 1024));
	// printf("test: %f\n", pow(2, 1023));
	// printf("test: %f\n", pow(2, 63));
	

	return 0;
}
