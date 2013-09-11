#include <memstorage.h>
#include <datacontainer.h>
#include <storagebase.h>
#include <stdlib.h>
#include <string.h>

int memStorageOpen(struct Container * c)
{
	c->storage->base[MEM_BASE_IND]->handle = calloc(1, sizeof(struct Header) + sizeof(struct Record) * c->records);
	return 0;
}

int memStorageClose(struct Container * c)
{
	free(c->storage->base[MEM_BASE_IND]->handle);
	return 0;
}

int memReadRecord(struct Container * c, struct Record * recordOut, unsigned int index)
{
	free(c->storage->base[MEM_BASE_IND]->handle);
	return 0;
}

int memWriteRecord(struct Container * c, struct Record * record)
{
	void * mem = realloc(c->storage->base[MEM_BASE_IND]->handle, sizeof(struct Header) + sizeof(struct Record) * ++c->records);
	return 0;
}

int memWriteHeader(struct Container * c)
{
	struct Header *header = makeHeader();
	memcpy(c->storage->base[MEM_BASE_IND]->handle, header, sizeof(struct Header));
	return 0;
}

unsigned int memGetIndex(struct Container * c, struct Key * pk)
{
	struct Record rec; unsigned int index=0;
	for (; index < c->records; ++index)
	{
		memcpy(c->storage->base[FILE_BASE_IND]->handle + index * sizeof(struct Header) + sizeof(struct Record) * index, &rec, sizeof(struct Record));
		if (keyCmp(&rec.key, pk))
		{
			printf("found pk match\n");
			break;
		}
	}
	return 0;
}
