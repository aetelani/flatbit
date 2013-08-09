#include "flatbit.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

Table * initTable(Table * table)
{
    if(table && !table->open)
    {    
        FILE * db_file = fopen(table->id, "a+");

        if (db_file)
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
