#include "flatbit.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//!TODO multitable, multifile, abstract file & format
// Init File actually, one table, one file for now.

FBStorage * initStorage()
{
	fbStorage = (FBStorage*) calloc(1, sizeof(FBStorage));
	fbStorage->id = "db";
	fbStorage->handle = fopen(fbStorage->id, "a+");
	return fbStorage;
}

Container * initContainer(Container * container)
{	
    if(container && !container->open)
    {          
        if (fbStorage->handle)
        {
            container->open = FILE_OPEN_EXCLUSIVE;
            
        } else
        {
            printf("file open failed\n");           
        }
    }

    return container;
}

int writeHeader(Container * table, Header * header)
{
    if (table && table->open)
    {
		lseek((int) fbStorage->handle, (off_t) 0, SEEK_SET);
		fwrite(header, sizeof(*header), 1, fbStorage->handle);
		fflush(fbStorage->handle);
	}
    return 0;
}

int writeData(Container * table, Record * r)
{
    return 0;
}

Index * getIndex(Container * table, Key * pk)
{
    return 0;
}

Data * getData(Container * table, Index * index)
{
    return 0;
}

int main()
{
    Container *t = NULL;
    initContainer(t);
    printf("Records: %i\n", t->records);
    return 0;
}
