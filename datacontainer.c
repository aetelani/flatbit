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

struct Container * makeContainer(enum Policy apolicy)
{
	
    struct Container * container = calloc(1, sizeof(struct Container));
    container->records = 0; //!TODO count get record count
    container->policy = apolicy;
    container->storage = storageInit(container);
    return container;
}

struct Record * copyRecord(struct Record * r)
{
    struct Record * record = malloc(sizeOfRecord);
    memcpy(record,r, sizeOfRecord);
    return record;
}

unsigned int getIndex(struct Container * container, struct Key * pk)
{   
    int readBufferSize = 1;
    struct Record rec; unsigned int index=0;
    int readedValue;
    assert(pk);

    printf("iterating records: %i\n", container->records);
    for (; index < container->records; ++index)
    {
        int seekTo = sizeOfRecord*index;
        
		if (container->storage->base[MEM_BASE_IND])
			index = container->storage->base[MEM_BASE_IND]->getIndex(container, pk);
		else
			fseek(container->storage->base[FILE_BASE_IND]->handle, seekTo, SEEK_SET);
			
        printf("seekTo %i, ind:%i\n", seekTo, index);
        
        if (container->storage->base[MEM_BASE_IND]) // fixthis reading values for testing.
			container->storage->base[MEM_BASE_IND]->read(container, &rec, index);
        else
			readedValue = fread(&rec, sizeOfRecord, readBufferSize, container->storage->base[FILE_BASE_IND]->handle);
        
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

struct Data getData(struct Container * container, unsigned int ind)
{
    struct Record rec = { .data = 0 };
    int failed = 1;
    
    assert(container && container->storage && container->storage->base[FILE_BASE_IND] && container->storage->base[FILE_BASE_IND]->handle);
	failed = fseek(container->storage->base[FILE_BASE_IND]->handle, sizeof(struct Record)*(ind)+sizeof(struct Header), SEEK_SET);
    
    if (!failed)
    {
        int read = fread(&rec, sizeof(struct Record), 1, container->storage->base[FILE_BASE_IND]->handle);
    }
    return rec.data;
}

