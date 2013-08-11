/*
Copyright (C) 2013  Anssi Eteläniemi

This file is part of FlatBit.

FlatBit is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FlatBit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FlatBit.  If not, see <http://www.gnu.org/licenses/>.
*/
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
    if (storage)
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
	printf("%i .. %i\n", a->pk, b->pk);
	int ret = a && b ? a->pk == b->pk : 0;
	printf("return value cmp:%i\n", ret);
	return ret;
}

Record * copyRecord(Record * r)
{
	Record * record = malloc(sizeof(Record));
	memcpy(record,r, sizeof(Record));
	return record;
}

unsigned int getIndex(Container * container, Key * pk)
{	
	int readBufferSize = 1;
	const int sizeOfRecord = sizeof(Record);
	const int sizeOfHeader = sizeof(Header);
	Record rec; unsigned int index=0;
	printf("iterating records: %i\n", container->records);
	for (; index <= container->records; ++index)
	{
		int seekTo = sizeOfHeader+sizeOfRecord*index;
		fseek(container->storage->handle, seekTo, SEEK_SET);
		printf("seekTo %i, ind:%i\n", seekTo, index);
		int read = fread(&rec, sizeof(Record), readBufferSize, container->storage->handle);
		
		if (keyCmp(&rec.key, pk))
		{
			printf("found pk match\n");
			break;
		}
	}
	
	if (read)
	{
		printf("rec->pk:%u, index:%u\n", pk->pk, index);
	}
    return index;
}

Data getData(Container * container, unsigned int ind)
{
	Record rec = { .data = 0 };
	int failed = fseek(container->storage->handle, sizeof(Record)*(ind)+sizeof(Header), SEEK_SET);
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

	int recordCnt = 10;
	for (int i=0; i < recordCnt; i++)
	{
		Key key = { .pk = i };
		Data data = { .data = i };
		Record rec = { .key = key, .data = data };
		writeData(c, &rec);
	}

	for (int i=; i < recordCnt; i++)
	{
		Key key = { .pk = i };
		unsigned int index  = getIndex(c, &key);
		Data d = getData(c, index);
	}
	
	if (c)
	{
		/*printf("Records: %i\n", c->records);
		printf("Founded index: %i\n", index);
		printf("Data digged:%i", d.data);*/
	} else
		printf("Failed miserably\n");
				
    return 0;
}
