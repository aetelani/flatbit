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
#ifndef DATACONTAINER_H
#define DATACONTAINER_H
#include <storage.h>
#include <storagepolicy.h>

struct StorageBase;

typedef struct DataKey {
	int pk;
} Key;

typedef struct DataLoad {
	int data;
} Data;

struct Record {
	Key key;
	Data data;
};

typedef struct {
	Key key;
	int index;
} Index;

struct Container {
	int records;
	int mode;
	Index *index;
	FBStorage * storage;
	//change this to FBStorage and use storagebase for generic api for operations that use file apia tai mitä sitten tarviikaan.!!!!
};

struct Container * makeContainer(StoragePolicy s);

int keyCmp(Key * a, Key * b);

int writeData(struct Container * container, struct Record * record);

struct Record * copyRecord(struct Record * r);

unsigned int getIndex(struct Container * container, Key * pk);

Data getData(struct Container * container, unsigned int ind);

#endif
