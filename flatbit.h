#include <stdio.h>

enum FileOpenMode {
	FILE_CLOSED, FILE_OPEN_EXCLUSIVE
};

typedef struct FileHeader {
	int version;
	} Header;

typedef struct	RecordKey {
	int key;
	} Key;

typedef struct RecordData {
	int data;
	} Data;

typedef struct RecordEntry {
	Key key;
	Data data;
	} Record;

typedef struct IndexPair {
	Key key; int index;
	} Index;

typedef struct ContainerData {
	int records;
	int open;
	} Container;
	
typedef struct Storage {
	const char * id;
	FILE * 	handle;
	void (*setHandle) (FILE*);
} FBStorage;

void impFBSetHandle(FILE*h) {}

FBStorage * fbStorage;
