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
#include <datacontainer.h>
#include <storagepolicy.h>
#include <storage.h>
#include <flatbit.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Container * makeContainer(StoragePolicy policy)
{
    Container * container = calloc(1, sizeof(Container));
    container->records = 0; //!TODO count get recotd count
    container->mode = policy;
    
    switch (container->mode)
    {
		case CONTAINER_STORE_IN_FILE:
		{
			printf("CONTAINER_STORE_IN_FILE\n");
			FBStorageBase *sb = calloc(1, sizeof(FBStorageBase));
			sb->handle = openStorage()->handle; //fix return type
			printf("sb->handle: %p\n", sb->handle);
			sb->mode = STORAGE_FILE_APPEND; //! TODO file mode
			sb->type = CONTAINER_STORE_IN_FILE;
			container->storage = sb;
			break;
		}
		case CONTAINER_STORE_BUFFERED:
		{
			printf("CONTAINER_STORE_BUFFERED\n");
			break;
		}
		case CONTAINER_STORE_IN_MEMORY:
		{
			printf("CONTAINER_STORE_IN_MEMORY\n");
			break;
		}
		default:
			printf("policy not found\n");
			free(container);
	}

    return container;
}

int writeData(Container * container, Record * record)
{
    //!TODO more methods, now only Append
    printf("sb->handle: %p\n", container->storage->handle);
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
    int readedValue;

    printf("iterating records: %i\n", container->records);
    for (; index <= container->records; ++index)
    {
        int seekTo = sizeOfHeader+sizeOfRecord*index;
        fseek(container->storage->handle, seekTo, SEEK_SET);
        printf("seekTo %i, ind:%i\n", seekTo, index);
        readedValue = fread(&rec, sizeof(Record), readBufferSize, container->storage->handle);
        
        if (keyCmp(&rec.key, pk))
		{
            printf("found pk match\n");
            break;
        }
    }
    
    if (readedValue)
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

