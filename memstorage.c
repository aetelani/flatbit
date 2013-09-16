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

const int segSize()
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
	c->storage->base[MEM_BASE_IND]->handle = calloc(c->records, segSize());
    assert(c->storage->base[MEM_BASE_IND]->handle && c->storage->base[MEM_BASE_IND]->header);
	printf("size of %d allocated mem at %p\n", c->records * sizeOfRecord, c->storage->base[MEM_BASE_IND]->handle);
	return 0;
}

int memStorageClose(struct Container * c)
{	
	free(c->storage->base[MEM_BASE_IND]->handle); //oops should close Keys too if pointers
	printf("size of %d free'd mem\n", c->records * sizeOfRecord);
	return 0;
}

int memReadRecord(struct Container * c, struct Record * recordOut, unsigned int index)
{
	//memcpy(recordOut, c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord);
	recordOut = c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index; //try without copy
	return 0;
}

int memDelRecord(struct Container * c, unsigned int index)
{
	struct Record * record;
	record = c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index;
	free(record->key);
	record->key = 0;
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
    
    struct Record * r =  c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * c->records;
	memcpy(c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * (c->records++), record, sizeOfRecord);

	printf("wrote key %d ", record->key->pk);
	printf("records %d after write mem\n", c->records);
	return 0;
}

int memWriteHeader(struct Container * c)
{
	struct Header *header = makeHeader();
	memcpy(c->storage->base[MEM_BASE_IND]->header, header, sizeOfHeader);
	return 0;
}

unsigned int memGetIndex(struct Container * c, struct Key * key)
{
	struct Record rec; unsigned int index=0;
	for (; index <= c->records; index++)
	{
		memcpy(&rec, c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord);
		//printf("getIndex, index %i, pk:%i, rec.pk:%i", index, key->pk, (&rec).key.pk);
		//printf(" dataoffsetptr %p, with offset:%i\n",c->storage->base[MEM_BASE_IND]->handle + sizeOfRecord * index, sizeOfRecord * index);
		if (keyCmp((&rec)->key, key))
		{
			printf("found pk match\n");
			return index;
			break;
		}
	}
	return 0;
}
