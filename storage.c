#include <storage.h>
#include <stdlib.h>
#include <unistd.h>

FBStorage * openStorage()
{
    FBStorage * storage = NULL;
  
    storage = calloc(1, sizeof(FBStorage));
    storage->id = "db";
    storage->handle = fopen(storage->id, "a+");
    
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
        failed = remove(storage->id);

    return failed;
}

int writeHeader(FBStorage * storage)
{
    if (storage)
    {
        lseek((int) storage->handle, (off_t) 0, SEEK_SET);
        fwrite(&fbHeader, sizeof(Header), 1, storage->handle);
        fflush(storage->handle);
    }
    return 0;
}
