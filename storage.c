/*
Copyright (C) 2013  Anssi Eteläniemi

This file is part of FlatBit.

FlatBit is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FlatBit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FlatBit.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <filestorage.h>
#include <storagebase.h>
#include <datacontainer.h>
#include <stdlib.h>

struct StorageBase * fileStorageInit()
{	
	struct StorageBase * base = calloc(1, sizeof(struct StorageBase));
	base->open = &fileStorageOpen; // should be policy based, not probably explicit
	base->close = &fileStorageClose;
	base->write = &fileWriteRecord;
	base->read = &fileReadRecord;
	base->getIndex = &fileGetIndex;
	return base;
}

struct Header * makeHeader(struct Container * container)
{
	// read container/storage configuration, generate related header.
	struct Header * header = calloc(1, sizeof(struct Header));
	header->version = 0;
	return header;
}

/*int writeHeader(FBStorage * storage);

FBStorage * openStorage()
{
    FBStorage * storage = calloc(1, sizeof(FBStorage));

    if (storage)
    {
        storage->id = "db";
        storage->status = STORAGE_UNDEF;
    }    

    int fileRWFailed = access(storage->id, R_OK | W_OK);

    if (fileRWFailed)
    {
        int storageRemoveFailed = removeStorage(storage);
        
        if (storageRemoveFailed)
        {
            printf("storage remove failed, freeing handle");
            if (storage && storage->handle)
                free(storage->handle);
        }
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
        fwrite(&fbHeader, sizeof(struct Header), 1, storage->handle);
        fflush(storage->handle);
    }
    return 0;
}*/
