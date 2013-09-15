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
#ifndef STORAGEBASE_H
#define STORAGEBASE_H

struct Header;
struct Key;
struct Container;
struct Record;
typedef void StorageHandle;

static const int FILE_BASE_IND = 0;
static const int MEM_BASE_IND = 1;

struct StorageBase
{
    StorageHandle * handle;
	struct Header * header;
    int (*write)(struct Container *container, struct Record *record);
    int (*read)(struct Container *container, struct Record * recordOut, unsigned int index);
    int (*open)(struct Container *container);
    int (*close)(struct Container *container);    
    unsigned int (*getIndex)(struct Container * container, struct Key * pk);
    int (*flush)(struct StorageBase * base);
};

#endif
