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
#include <datacontainer.h>
#include <storagebase.h>
#include <storagepolicy.h>
#include <storage.h>
#include <flatbit.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Container * makeContainer(enum Policy apolicy)
{
	
    struct Container * container = calloc(1, sizeof(struct Container));
    container->records = 0; //!TODO count get record count
    container->policy = apolicy;
    container->storage = storageInit(container);
    return container;
}

struct Record * copyRecord(struct Record * r)
{
    struct Record * record = malloc(sizeOfRecord);
    memcpy(record,r, sizeOfRecord);
    return record;
}

