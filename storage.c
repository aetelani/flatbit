#include <storage.h>
#include <stdlib.h>
#include <unistd.h>

int writeHeader(FBStorage * storage);

FBStorage * openStorage()
{
    FBStorage * storage = calloc(1, sizeof(FBStorage));
    int cleanStorage = 0;

    if (storage)
    {
        storage->id = "db";
        storage->status = STORAGE_UNDEF;
    }    

    int fileRWFailed = access(storage->id, R_OK | W_OK);

    if (fileRWFailed)
    {
        cleanStorage = !removeStorage(storage);
        storage->status = STORAGE_REMOVED;
    }
    
    storage->handle = fopen(storage->id, "a+");

    if (storage->handle)
    {
        int headerFailed = writeHeader(storage);
        storage->status = headerFailed ? STORAGE_UNDEF : STORAGE_OPEN;
    }
    
    return storage;
}

int closeStorage(FBStorage * storage)
{
    int failed;
    
    if (storage)
    {
        failed = fclose(storage->handle);

        if (failed)
            printf("closing storage failed\n");
    }
    return failed;
}

int removeStorage(FBStorage * storage)
{
    int failed;

    if (storage)
        return STORAGE_UNDEF;
    
    if (storage->id)
        failed = remove(storage->id);

    if (!failed)
        storage->status = STORAGE_REMOVED;

    return failed;
}

int writeHeader(FBStorage * storage)
{
    if (storage && storage->status == STORAGE_OPEN)
    {
        lseek((int) storage->handle, (off_t) 0, SEEK_SET);
        fwrite(&fbHeader, sizeof(Header), 1, storage->handle);
        fflush(storage->handle);
    }
    return 0;
}
