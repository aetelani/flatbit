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
#include "flatbit.h"
#include <storage.h>
#include <datacontainer.h>
#include <storage.h>
#include <stdlib.h>

//!TODO multitable, multifile, abstract file & format
// Init File actually, one table, one file for now.

int main()
{
	enum Policy policy = CONTAINER_STORAGE_MEMORY;
	
    struct Container *c = makeContainer(policy);
    c->storage->open(c);

    int recordCnt = 3;
    for (int i=0; i < recordCnt; i++)
    {
        struct Key * key = calloc(1,sizeof(struct Key));
        struct Data data = { .data = i };
        struct Record rec = { .key = key, .data = data };
        writeData(c, &rec);
    }

	struct Record * r = malloc(sizeOfRecord);
    for (int i=1; i < recordCnt; i++)
    {
        struct Key key = { .pk = i };
        unsigned int index  = c->storage->getIndex(c, &key);
        int res = -1;
        if (index)
        { 
			printf("flatbit.c with index: %i\n", index);
			res = c->storage->read(c, r, index);
			c->storage->delete(c, index);
		}
    }
    
    if (c)
    {
        //printf("Records: %i\n", c->records);
        //printf("Founded index: %i\n", index);
        //printf("Data digged:%i", d.data);
    } else
        printf("Failed miserably\n");

    return 0;
}
