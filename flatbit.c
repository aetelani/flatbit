#include "flatbit.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

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

Container * makeContainer(FBStorage * s)
{
	Container * container = NULL;
	
    if(!container)
	{
		container = (Container*) calloc(1, sizeof(Container));
		container->mode = CONTAINER_STORE_IN_FILE;
		container->storage = s;
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
	//!TODO more methods, now only Append
	fseek(container->storage->handle, 0, SEEK_END);
	fwrite(record, sizeof(Record), 1, container->storage->handle);
	++container->records;
	fflush(container->storage->handle);
    return 0;
}

int keyCmp(Key * a, Key * b)
{
	return a && b ? a->pk == b->pk : 0;
}

Record * copyRecord(Record * r)
{
	Record * record = malloc(sizeof(Record));
	memcpy(record,r, sizeof(Record));
	return record;
}

unsigned int getIndex(Container * container, Key * pk)
{
	fseek(container->storage->handle, sizeof(Header)+1, SEEK_SET);
	int readBufferSize = 1;
	Record rec; unsigned int index=0;
	for (; index <= container->records; ++index)
	{
		int read = fread(&rec, sizeof(Record), readBufferSize, container->storage->handle);
		
		if (keyCmp(&rec.key, pk))
		{
			break;
		}
	}
	
	if (read)
	{
		printf("rec->pk:%u, index:%u\n", rec.key.pk, index);
	}
    return index;
}

Data getData(Container * container, Index * ind)
{
	Record rec = { .data = 0 };
	int failed = fseek(container->storage->handle, sizeof(Record)*ind->index+sizeof(Header), SEEK_SET);
	if (!failed)
	{
		int read = fread(&rec, sizeof(Record), 1, container->storage->handle);
	}
    return rec.data;
}

int main()
{
	FBStorage *s = createStorage();
	writeHeader(s);
	Container *c = NULL;
	
	if (s)
	{
		c = makeContainer(s);
	}

	Key key = { .pk = 0 };
	Data data = { .data = 1 };
	Record rec = { .key = key, .data = data };

	writeData(c, &rec);

	int ind  = getIndex(c, &key);
	
	if (c)
	{
		printf("Records: %i\n", c->records);
		printf("Founded index: %i\n", ind);
	} else
		printf("Failed miserably\n");
				
    return 0;
}
