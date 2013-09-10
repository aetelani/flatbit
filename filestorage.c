#include <filestorage.h>
#include <stdlib.h>
#include <unistd.h>
#include <datacontainer.h>
#include <storagebase.h>


int fileStorageRemove(struct Container * container);

int fileStorageOpen(struct Container * container)
{
	assert(container && container->storage && container->storage->base);
	
	FBStorage * storage = container->storage; //shortuct

    if (storage)
    {
        storage->id = "db";
        storage->status = STORAGE_UNDEF;
    }    

    int fileRWFailed = access(storage->id, R_OK | W_OK);

    if (fileRWFailed)
    {
        /*int storageRemoveFailed = fileStorageRemove(container); //do fallback
        
        if (storageRemoveFailed)
        {
            printf("storage remove failed, freeing handle");
            if (storage && storage->base->handle)
                free(storage->base->handle);
        }*/
    }
    
    storage->base->handle = fopen(storage->id, "a+");

    if (storage->handle)
    {
        int headerFailed = fileWriteHeader(container);
        storage->status = headerFailed ? STORAGE_UNDEF : STORAGE_OPEN;
    }
    
	return storage->status;
}

int fileStorageClose(struct Container * container)
{
	int failed;
    
    if (container->storage)
    {
        failed = fclose(container->storage->handle);

        if (failed)
            printf("closing storage failed\n");
    }
    return failed;
}

int fileStorageRemove(struct Container * container)
{
    int failed;

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
    printf("sb->handle: %p\n", container->storage->handle);
    fseek(container->storage->base->handle, 0, SEEK_END);
    fwrite(record, sizeof(struct Record), 1, container->storage->base->handle);
    ++container->records;
    fflush(container->storage->handle);
    return 0;
}

int fileReadRecord(struct Container *container, struct Record * recordOut, Index index)
{        
    int failed = fseek(container->storage->base->handle, sizeof(struct Record)*(index)+sizeof(struct Header), SEEK_SET);
    int read = 0;
    
    if (!failed)
    {
        read = fread(recordOut, sizeof(struct Record), 1, container->storage->base->handle);
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
	assert(container && container->storage && container->storage->handle);
	struct Header * header = makeHeader(container);
	
	switch (container->storage->status) {
	case STORAGE_OPEN:		
		lseek((int) container->storage->handle, (off_t) 0, SEEK_SET);
		fwrite(header, sizeof(struct Header), 1, container->storage->handle);
		fflush(container->storage->handle);		
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
