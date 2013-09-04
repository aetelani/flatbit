#include <datacontainer.h>
#include <storage.h>
#include <flatbit.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Container * makeContainer(FBStorage * s)
{
    Container * container = NULL;
    
    if(!container)
    {
        container = calloc(1, sizeof(Container));
        container->mode = CONTAINER_STORE_IN_FILE;
        container->storage = s;
        container->records = 0; //!TODO count get recotd count
    }

    return container;
}

int writeData(Container * container, Record * record)
{
    //!TODO more methods, now only Append
    fseek(container->storage->handle, 0, SEEK_END);
    fwrite(record, sizeof(Record), 1, container->storage->handle);
    ++container->records;
    fflush(container->storage->handle);
    return 0;
}

int keyCmp(Key * a, Key * b)
{
    printf("%i .. %i\n", a->pk, b->pk);
    int ret = a && b ? a->pk == b->pk : 0;
    printf("return value cmp:%i\n", ret);
    return ret;
}

Record * copyRecord(Record * r)
{
    Record * record = malloc(sizeof(Record));
    memcpy(record,r, sizeof(Record));
    return record;
}

unsigned int getIndex(Container * container, Key * pk)
{   
    int readBufferSize = 1;
    const int sizeOfRecord = sizeof(Record);
    const int sizeOfHeader = sizeof(Header);
    Record rec; unsigned int index=0;
    int readedValue;

    printf("iterating records: %i\n", container->records);
    for (; index <= container->records; ++index)
    {
        int seekTo = sizeOfHeader+sizeOfRecord*index;
        fseek(container->storage->handle, seekTo, SEEK_SET);
        printf("seekTo %i, ind:%i\n", seekTo, index);
        readedValue = fread(&rec, sizeof(Record), readBufferSize, container->storage->handle);
        
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

Data getData(Container * container, unsigned int ind)
{
    Record rec = { .data = 0 };
    int failed = fseek(container->storage->handle, sizeof(Record)*(ind)+sizeof(Header), SEEK_SET);
    if (!failed)
    {
        int read = fread(&rec, sizeof(Record), 1, container->storage->handle);
    }
    return rec.data;
}

