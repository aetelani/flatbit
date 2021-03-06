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

#ifndef STORAGE_H
#define STORAGE_H
#include <stdio.h>
#include <filestorage.h>
#include <memstorage.h>
#include <assert.h>

#define STORAGE_MAX_COUNT 4

typedef enum {
    STORAGE_UNDEF=-1, STORAGE_OPEN=0, STORAGE_CLOSED, STORAGE_REMOVED
} FBStorageStatus;

typedef enum
{
	STORAGE_APPEND
} StorageMode;

struct StorageBase;
struct Storage {
	const char * id;
    int (*write)(struct Container * restrict c, struct Record *record);
    int (*read)(struct Container * restrict c, struct Record * recordOut, unsigned int index);
    int (*open)(struct Container * restrict c);
    int (*close)(struct Container * restrict c);
    int (*delete)(struct Container * restrict c, unsigned int index);
    unsigned int (*getIndex)(struct Container * restrict c, struct Key * pk);    
	struct StorageBase * base[STORAGE_MAX_COUNT];
    FBStorageStatus status;
};

struct Storage * storageInit(struct Container * container);

struct Header {
	int version;
};

struct Header * makeHeader();

int keyCmp(struct Key * a, struct Key * b);

#endif
