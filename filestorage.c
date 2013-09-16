#include <filestorage.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <datacontainer.h>
#include <storagebase.h>



int fileStorageRemove(struct Container * container);

int fileStorageOpen(struct Container * container)
{
	assert(container && container->storage && container->storage->base);
	
	struct Storage * storage = container->storage; //shortuct

    if (storage)
    {
        storage->id = "db";
        storage->status = STORAGE_UNDEF;
    }    
  
    storage->base[FILE_BASE_IND]->handle = fopen(storage->id, "a+");

    if (container->storage->base[FILE_BASE_IND]->handle)
    {
        int headerFailed = fileWriteHeader(container);
        storage->status = headerFailed ? STORAGE_UNDEF : STORAGE_OPEN;
    }
    
	return storage->status;
}

int fileStorageClose(struct Container * container)
{
	int failed = fclose(container->storage->base[FILE_BASE_IND]->handle);

    return failed;
}

int fileStorageRemove(struct Container * container)
{
    int failed = 1;

    if (!container->storage)
        return STORAGE_UNDEF;
    
    if (container->storage->id)
        failed = remove(container->storage->id);

    if (!failed)
        container->storage->status = STORAGE_REMOVED;

    return failed;
}

int fileWriteRecord(struct Container * container, struct Record * record)
{
    fseek(container->storage->base[FILE_BASE_IND]->handle, 0, SEEK_END);
    fwrite(record, sizeof(struct Record), 1, container->storage->base[FILE_BASE_IND]->handle);
    ++container->records;
    fflush(container->storage->base[FILE_BASE_IND]->handle);
    return 0;
}

int fileReadRecord(struct Container *container, struct Record * recordOut, Index index)
{        
    int failed = fseek(container->storage->base[FILE_BASE_IND]->handle, sizeof(struct Record)*(index)+sizeof(struct Header), SEEK_SET);
    int read = 0;
    
    if (!failed)
    {
        read = fread(recordOut, sizeof(struct Record), 1, container->storage->base[FILE_BASE_IND]->handle);
    }
    return read;
}

Index fileGetIndex(struct Container * container, struct Key * pk)
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
        fseek(container->storage->base[FILE_BASE_IND]->handle, seekTo, SEEK_SET);
        printf("seekTo %i, ind:%i\n", seekTo, index);
        readedValue = fread(&rec, sizeof(struct Record), readBufferSize, container->storage->base[FILE_BASE_IND]->handle); //mmap/read might be better
        
        if (keyCmp(rec.key, pk))
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

int fileRemoveStorage(struct Container * container)
{
	assert(container && container->storage && container->storage->id);
    
    int failed = remove(container->storage->id);

    if (!failed)
        container->storage->status = STORAGE_REMOVED;

    return failed;
}

int fileWriteHeader(struct Container * container)
{
	struct Header * header = makeHeader(container);
	
	switch (container->storage->status) {
	case STORAGE_OPEN:		
		lseek((int) container->storage->base[FILE_BASE_IND]->handle, (off_t) 0, SEEK_SET);
		fwrite(header, sizeof(struct Header), 1, container->storage->base[FILE_BASE_IND]->handle);
		fflush(container->storage->base[FILE_BASE_IND]->handle);
		break;
	case STORAGE_CLOSED:
	case STORAGE_UNDEF:
	case STORAGE_REMOVED:
	default:
		free(header);
		printf("unsupported storage status\n");
	}
    return 0;
}


