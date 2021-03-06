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
#include <stdlib.h>
#include <filestorage.h>
#include <memstorage.h>
#include <storagebase.h>
#include <datacontainer.h>
#include <stdlib.h>

int storageRead(struct Container * container, struct Record * outRecord, unsigned int index);
int storageClose(struct Container * container);
int storageOpen(struct Container * container);
unsigned int storageGetIndex(struct Container * container, struct Key * pk);
int storageDelRecord(struct Container * c, unsigned int index);
int storageWrite(struct Container *container, struct Record *record);

int storageOpen(struct Container * container)
{
	if (container->storage->base[FILE_BASE_IND])
	{
		container->storage->base[FILE_BASE_IND]->open(container);
	}
		
	if (container->storage->base[MEM_BASE_IND])
	{
		container->storage->base[MEM_BASE_IND]->open(container);
	}

	return 0;
}

int storageClose(struct Container * container)
{
	if (container->storage->base[FILE_BASE_IND])
	{
		container->storage->base[FILE_BASE_IND]->close(container);
	}
    
    if (container->storage->base[MEM_BASE_IND])
    {
		container->storage->base[MEM_BASE_IND]->close(container);
	}
		
	return 0;
}

struct Storage * storageInit(struct Container * container)
{
	assert(container && !container->storage);
	container->storage = calloc(1, sizeof(struct Storage));		
	container->storage->open = &storageOpen;
	container->storage->close = &storageClose;
	container->storage->read = &storageRead;
	container->storage->write = &storageWrite;
	container->storage->getIndex = &storageGetIndex;
	container->storage->delete = &storageDelRecord;	

	switch (container->policy) {
	case CONTAINER_STORAGE_FILE:
		container->storage->base[FILE_BASE_IND] = calloc(1, sizeof(struct StorageBase));
		container->storage->base[FILE_BASE_IND]->open = &fileStorageOpen;
		container->storage->base[FILE_BASE_IND]->close = &fileStorageClose;
		container->storage->base[FILE_BASE_IND]->write = &fileWriteRecord;
		container->storage->base[FILE_BASE_IND]->read = &fileReadRecord;
		container->storage->base[FILE_BASE_IND]->getIndex = &fileGetIndex;
		break;
	case CONTAINER_STORAGE_MEMORY:
		container->storage->base[MEM_BASE_IND] = calloc(1, sizeof(struct StorageBase));
		container->storage->base[MEM_BASE_IND]->open = &memStorageOpen;
		container->storage->base[MEM_BASE_IND]->close = &memStorageClose;
		container->storage->base[MEM_BASE_IND]->write = &memWriteRecord;
		container->storage->base[MEM_BASE_IND]->read = &memReadRecord;
		container->storage->base[MEM_BASE_IND]->getIndex = &memGetIndex;
		container->storage->base[MEM_BASE_IND]->delete = &memDelRecord;
		break;
	}	

	return container->storage;
}

struct Header * makeHeader(struct Container * container)
{
	// read container/storage configuration, generate related header.
	struct Header * header = calloc(1, sizeof(struct Header));
	header->version = 0;
	return header;
}

int writeData(struct Container * container, struct Record * record)
{
    //!TODO more methods, now only Append
//    assert(container && container->storage && container->storage->base && container->storage->base->write && container->storage->base->handle);
	if (container->storage->base[FILE_BASE_IND])
		container->storage->base[FILE_BASE_IND]->write(container, record);
    
    if (container->storage->base[MEM_BASE_IND])
		container->storage->base[MEM_BASE_IND]->write(container, record);
    return 0;
}

int keyCmp(struct Key * a, struct Key * b)
{    
    int ret = a && b ? a->pk == b->pk : 0;
//	printf("%i == %i -> %i\n", a->pk, b->pk, ret);
    return ret;
}

unsigned int storageGetIndex(struct Container * container, struct Key * pk)
{
    unsigned int index=0;
    assert(container && pk);

	if (container->storage->base[MEM_BASE_IND])
		index = container->storage->base[MEM_BASE_IND]->getIndex(container, pk);
	else
		index = container->storage->base[FILE_BASE_IND]->getIndex(container, pk);

	printf("founded index: %i\n", index);

    return index;
}

int storageRead(struct Container * container, struct Record * outRecord, unsigned int index)
{
	printf("read: value with index: %i\n", index);
	assert(container && outRecord);
    int ret = 0;
    
	if (container->storage->base[MEM_BASE_IND])
		ret = container->storage->base[MEM_BASE_IND]->read(container, outRecord, index);
	else
		ret = container->storage->base[FILE_BASE_IND]->read(container, outRecord, index);
    
    return ret;
}

int storageDelRecord(struct Container * container, unsigned int index)
{
	printf("read: value with index: %i\n", index);
	assert(container);
    int ret = 0;

	if (container->storage->base[MEM_BASE_IND])
		ret = container->storage->base[MEM_BASE_IND]->delete(container, index);
	else
		ret = container->storage->base[FILE_BASE_IND]->delete(container, index);
		
	return ret;
}

int storageWrite(struct Container *container, struct Record *record)
{
	assert(container && record);
    int ret = 0;

	if (container->storage->base[MEM_BASE_IND])
		ret = container->storage->base[MEM_BASE_IND]->write(container, record);
	else
		ret = container->storage->base[FILE_BASE_IND]->write(container, record);
		
	return ret;
}
