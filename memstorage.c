#include <memstorage.h>
#include <datacontainer.h>
#include <storagebase.h>
#include <stdlib.h>
#include <string.h>

int memStorageOpen(struct Container * c)
{
	c->storage->base[MEM_BASE_IND]->header = malloc(sizeOfHeader);
	c->storage->base[MEM_BASE_IND]->handle = calloc(c->records, sizeOfRecord);
	printf("size of %d allocated mem\n", c->records * sizeOfRecord);
	return 0;
}

int memStorageClose(struct Container * c)
{	
	free(c->storage->base[MEM_BASE_IND]->handle);
	printf("size of %d free'd mem\n", c->records * sizeOfRecord);
	return 0;
}

int memReadRecord(struct Container * c, struct Record * recordOut, unsigned int index)
{
	struct Record recOut;
	memcpy(c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, &recOut, sizeOfRecord);
	printf("read record key: %i\n", recOut.key.pk);
	return 0;
}

int memWriteRecord(struct Container * c, struct Record * record)
{
	printf("size %d before realloc mem\n", c->records * sizeOfRecord);
	void * mem = realloc(c->storage->base[MEM_BASE_IND]->handle, sizeOfRecord * ++c->records);
	c->storage->base[MEM_BASE_IND]->handle = mem;
	memcpy(c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * (c->records-1), record, sizeOfRecord);
	printf("size %d after realloc mem\n", c->records * sizeOfRecord);
	return 0;
}

int memWriteHeader(struct Container * c)
{
	struct Header *header = makeHeader();
	memcpy(c->storage->base[MEM_BASE_IND]->header, header, sizeOfHeader);
	return 0;
}

unsigned int memGetIndex(struct Container * c, struct Key * pk)
{
	struct Record rec; unsigned int index=0;
	for (; index <= c->records; index++)
	{
		memcpy(c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, &rec, sizeOfRecord);
		if (keyCmp(&rec.key, pk))
		{
			printf("found pk match\n");
			return index;
			break;
		}
	}
	return 0;
}
