#include <memstorage.h>
#include <datacontainer.h>
#include <storagebase.h>
#include <stdlib.h>
#include <string.h>
#include <flatbitconfig.h>

extern const int maxNumberRecordsInSegment;
extern const int maxNumberOfSegments;

enum MemSegmentStatus {
    MEM_SEG_OK, MEM_SEG_FULL
};

enum MemStorageStatus {
    MEM_STORAGE_OK, MEM_STORAGE_FULL
};

const int segmentSize()
{
    return maxNumberRecordsInSegment * sizeOfRecord;
}

enum MemStorageStatus memStorageStatus(struct Container * c)
{
    return c->records <= maxNumberRecordsInSegment * maxNumberOfSegments ? MEM_STORAGE_OK : MEM_STORAGE_FULL;
}

int memStorageOpen(struct Container * c)
{
	c->storage->base[MEM_BASE_IND]->header = malloc(sizeOfHeader);
	c->storage->base[MEM_BASE_IND]->handle = calloc(c->records, segmentSize());
    assert(c->storage->base[MEM_BASE_IND]->handle && c->storage->base[MEM_BASE_IND]->header);
	printf("size of %d allocated mem at %p\n", c->records * sizeOfRecord, c->storage->base[MEM_BASE_IND]->handle);
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
	memcpy(recordOut, c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord);
	return 0;
}

int memWriteRecord(struct Container * c, struct Record * record)
{
    switch (memStorageStatus(c)) {
    case MEM_STORAGE_OK:
        break;
    case MEM_STORAGE_FULL:
        return MEM_STORAGE_FULL;
        break;
    }
        
	printf("size %d before realloc mem\n", c->records * sizeOfRecord);
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
		memcpy(&rec, c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord);
		printf("getIndex, index %i, pk:%i, rec.pk:%i", index, pk->pk, rec.key.pk);
		printf(" dataoffsetptr %p, with offset:%i\n",c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord * index);
		if (keyCmp(&rec.key, pk))
		{
			printf("found pk match\n");
			return index;
			break;
		}
	}
	return 0;
}
