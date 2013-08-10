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
		container->open = FILE_OPEN_EXCLUSIVE;
		container->records = 0; //!TODO count get recotd count
	}
    return container;
}

int writeHeader(Container * table, Header * header)
{
    if (table && table->open)
    {
		lseek((int) fbStorage->handle, (off_t) 0, SEEK_SET);
		fwrite(header, sizeof(*header), 1, fbStorage->handle);
		fflush(fbStorage->handle);
	}
    return 0;
}

int writeData(Container * table, Record * r)
{
    return 0;
}

Index * getIndex(Container * table, Key * pk)
{
    return 0;
}

Data * getData(Container * table, Index * index)
{
    return 0;
}

int main()
{
    Container *t = NULL;
    FBStorage *s = createStorage();
    t = makeContainer();
    printf("Records: %i\n", t->records);
    return 0;
}
