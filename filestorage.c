#include <filestorage.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <datacontainer.h>
#include <storagebase.h>


int fileStorageRemove(struct Container * container);

int fileStorageOpen(struct Container * container)
{
	assert(container != NULL);
//	container->storage = calloc(1, sizeof(FBStorage));
	
	assert(container->storage != NULL);	
//	container->storage->base = calloc(1, sizeof(struct StorageBase));
	
	assert(container->storage->base != NULL);
	
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

int fileWriteHeader(struct Container * container)
{
	//!TODO shortuct, fixthis. abstract might not want to know about container.
	FBStorage * storage = container->storage;
	
    if (storage && storage->status == STORAGE_OPEN)
    {
        lseek((int) storage->handle, (off_t) 0, SEEK_SET);
        fwrite(&fbHeader, sizeof(struct Header), 1, storage->base->handle);
        fflush(storage->handle);
    }
    return 0;
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

int fileReadRecord(struct Container *container, struct Record * recordOut, unsigned int index)
{        
    int failed = fseek(container->storage->base->handle, sizeof(struct Record)*(index)+sizeof(struct Header), SEEK_SET);
    int read = 0;
    
    if (!failed)
    {
        read = fread(recordOut, sizeof(struct Record), 1, container->storage->base->handle);
    }
    return read;
}
