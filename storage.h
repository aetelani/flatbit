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

typedef enum {
    STORAGE_UNDEF=-1, STORAGE_OPEN=0, STORAGE_CLOSED, STORAGE_REMOVED
} FBStorageStatus;

typedef enum
{
	STORAGE_APPEND
} StorageMode;

struct StorageBase;
typedef struct {
	const char * id;
    int type;
    int mode;
	FILE * 	handle;
	void (*setHandle) (FILE*);
	struct StorageBase * base;
    FBStorageStatus status;
} FBStorage;

FBStorage * fbStorage;

FBStorage * openStorage();

int closeStorage(FBStorage * storage);

int removeStorage(FBStorage * storage);

typedef struct {
	int version;
} Header;

static Header fbHeader = { .version = 1 };

#endif
