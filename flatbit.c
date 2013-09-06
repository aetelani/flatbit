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

//!TODO multitable, multifile, abstract file & format
// Init File actually, one table, one file for now.

int main()
{
//	FBStorage *s = openStorage();
	
	StoragePolicy storagePolicy = CONTAINER_STORE_IN_FILE;
	
//    FBStorage *s = createStorage();

    Container *c = NULL;
    
	c = makeContainer(storagePolicy);

    int recordCnt = 10;
    for (int i=0; i < recordCnt; i++)
    {
        Key key = { .pk = i };
        Data data = { .data = i };
        Record rec = { .key = key, .data = data };
        writeData(c, &rec);
    }

    for (int i=0; i < recordCnt; i++)
    {
        Key key = { .pk = i };
        unsigned int index  = getIndex(c, &key);
        Data d = getData(c, index);
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
