#include "flatbit.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

Table * initTable(Table * table)
{
	fbFile = (FBFile*) calloc(1, sizeof(FBFile));
	
    if(table && !table->open)
    {    
        fbFile->handle = fopen(table->id, "a+");

        if (fbFile->handle)
        {
            table->open = FILE_OPEN_EXCLUSIVE;
            
        } else
        {
            printf("file open failed\n");           
        }
    }

    return table;
}

int writeHeader(Table * table, Header * header)
{
    if (table && table->open)
    {
		lseek((int) fbFile->handle, (off_t) 0, SEEK_SET);
		fwrite(header, sizeof(*header), 1, fbFile->handle);
		fflush(fbFile->handle);
	}
    return 0;
}

int writeData(Table * table, Record * r)
{
    return 0;
}

Index * getIndex(Table * table, Key * pk)
{
    return 0;
}

Data * getData(Table * table, Index * index)
{
    return 0;
}

int main()
{
    Table t = { .id = (const char *)"db", .records = 0 };
    initTable(&t);
    printf("Records: %i\n", t.records);
    return 0;
}
