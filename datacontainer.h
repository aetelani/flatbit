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

typedef struct DataKey {
	int pk;
} Key;

typedef struct DataLoad {
	int data;
} Data;

typedef struct {
	Key key;
	Data data;
} Record;

typedef struct {
	Key key;
	int index;
} Index;

typedef struct {
	int records;
	int mode;
	Index *index;
	FBStorage * storage; 
	//change this to FBStorage and use storagebase for generic api for operations that use file apia tai mitä sitten tarviikaan.!!!!
} Container;

Container * makeContainer(StoragePolicy s);

int keyCmp(Key * a, Key * b);

int writeData(Container * container, Record * record);

Record * copyRecord(Record * r);

unsigned int getIndex(Container * container, Key * pk);

Data getData(Container * container, unsigned int ind);

#endif
