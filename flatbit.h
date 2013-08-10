#include <stdio.h>

enum StorageOpenMode {
	FILE_CLOSED, FILE_OPEN_EXCLUSIVE
};

enum ContainerbMode {
	CONTAINER_STORE_IN_FILE
};

typedef struct {
	const char * id;
	FILE * 	handle;
	void (*setHandle) (FILE*);
} FBStorage;

FBStorage * fbStorage;

typedef struct {
	int version;
} Header;

typedef struct {
	int pk;
} Key;

typedef struct {
	int data;
} Data;

typedef struct {
	Key key;
	Data data;
} Record;

typedef struct {
	Key key;
	int index;
} Index;

typedef struct {
	int records;
	int mode;
	Index *index;
	FBStorage * storage;
} Container;

void impFBSetHandle(FILE*h) {}

Header fbHeader = { .version = 1 };
