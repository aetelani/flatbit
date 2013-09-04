#include <storage.h>
#include <stdlib.h>
#include <unistd.h>

FBStorage * createStorage()
{
    FBStorage * storage = NULL;
    
    if (!storage)
    {
        storage = (FBStorage*) calloc(1, sizeof(FBStorage));
        storage->id = "db";
        storage->handle = fopen(storage->id, "a+");
    }
    return storage;
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
