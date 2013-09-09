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
#include <storagebase.h>
#include <storagepolicy.h>
#include <storage.h>
#include <flatbit.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Container * makeContainer(StoragePolicy policy)
{
    struct Container * container = calloc(1, sizeof(struct Container));
    container->records = 0; //!TODO count get record count
    container->mode = policy;
    
    if (!container->storage) 
		container->storage = calloc(1, sizeof(FBStorage));
    
    switch (container->mode)
    {
		case CONTAINER_STORE_IN_FILE:
		{
			printf("CONTAINER_STORE_IN_FILE\n");			
			container->storage->base =	fileStorageInit();
			assert(container && container->storage && container->storage->base && container->storage->base->open);
			int failed = container->storage->base->open(container); //little long path, maybe container ctx would be ok
			container->storage->mode = STORAGE_APPEND;
			container->storage->type = CONTAINER_STORE_IN_FILE;
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
			/*storageBase->handle = calloc(1, sizeof(Header));
			storageBase->mode = STORAGE_APPEND;
			storageBase->type = CONTAINER_STORE_IN_MEMORY;
			container->storage = storageBase;*/
			break;
		}
		default:
			printf("policy not found\n");
			free(container->storage);
			free(container);
	}

    return container;
}

int writeData(struct Container * container, struct Record * record)
{
    //!TODO more methods, now only Append
    printf("sb->handle: %p\n", container->storage->handle);
    /*fseek(container->storage->handle, 0, SEEK_END);
    fwrite(record, sizeof(Record), 1, container->storage->handle);
    ++container->records;
    fflush(container->storage->handle);*/
    return 0;
}

int keyCmp(Key * a, Key * b)
{
    printf("%i .. %i\n", a->pk, b->pk);
    int ret = a && b ? a->pk == b->pk : 0;
    printf("return value cmp:%i\n", ret);
    return ret;
}

struct Record * copyRecord(struct Record * r)
{
    struct Record * record = malloc(sizeof(struct Record));
    memcpy(record,r, sizeof(struct Record));
    return record;
}

unsigned int getIndex(struct Container * container, Key * pk)
{   
    int readBufferSize = 1;
    const int sizeOfRecord = sizeof(struct Record);
    const int sizeOfHeader = sizeof(struct Header);
    struct Record rec; unsigned int index=0;
    int readedValue;

    printf("iterating records: %i\n", container->records);
    for (; index < container->records; ++index)
    {
        int seekTo = sizeOfHeader+sizeOfRecord*index;
        fseek(container->storage->base->handle, seekTo, SEEK_SET);
        printf("seekTo %i, ind:%i\n", seekTo, index);
        readedValue = fread(&rec, sizeof(struct Record), readBufferSize, container->storage->base->handle);
        
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

Data getData(struct Container * container, unsigned int ind)
{
    struct Record rec = { .data = 0 };
    int failed = 1;
    
    assert(container && container->storage && container->storage->base && container->storage->base->handle);
	failed = fseek(container->storage->base->handle, sizeof(struct Record)*(ind)+sizeof(struct Header), SEEK_SET);
    
    if (!failed)
    {
        int read = fread(&rec, sizeof(struct Record), 1, container->storage->base->handle);
    }
    return rec.data;
}

