#include "flatbit.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//!TODO multitable, multifile, abstract file & format
// Init File actually, one table, one file for now.

FBStorage * createStorage()
{
	FBStorage * storage = NULL;
	
	if (!storage)
	{
		storage = (FBStorage*) calloc(1, sizeof(FBStorage));
		storage->id = "db";
		storage->handle = fopen(storage->id, "a+");
	}
	return storage;
}

Container * makeContainer()
{
	Container * container = NULL;
	
    if(!container)
	{
		container = (Container*) calloc(1, sizeof(Container));
		container->mode = CONTAINER_STORE_IN_FILE;
		container->records = 0; //!TODO count get recotd count
	}
    return container;
}

int writeHeader(FBStorage * storage)
{
    //if (container)
    {
		lseek((int) storage->handle, (off_t) 0, SEEK_SET);
		fwrite(&fbHeader, sizeof(Header), 1, storage->handle);
		fflush(storage->handle);
	}
    return 0;
}

int writeData(Container * container, Record * record)
{
	fwrite(record, sizeof(Record), 1, container->storage->handle);
	fflush(container->storage->handle);
    return 0;
}

Index * getIndex(Container * c, Key * pk)
{
    return 0;
}

Data * getData(Container * c, Index * ind)
{
    return 0;
}

int main()
{
	FBStorage *s = createStorage();
	writeHeader(s);
	Container *t = NULL;
	
	if (s)
	{
		t = makeContainer();
	}
	
	if (t)
		printf("Records: %i\n", t->records);
	else
		printf("Failed miserably\n");
		
    return 0;
}
