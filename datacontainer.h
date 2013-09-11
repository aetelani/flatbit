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

struct Key {
	int pk;
};

struct Data {
	int data;
};

struct Record {
	struct Key key;
	struct Data data;
};

typedef unsigned int Index;

struct IndexKey {
	struct Key key;
	unsigned int index;
};

enum Policy {
	CONTAINER_STORAGE_FILE=1, CONTAINER_STORAGE_MEMORY=2
};

struct Container {
	int records;
	enum Policy policy;
	Index *index;
	struct Storage * storage;
	//change this to FBStorage and use storagebase for generic api for operations that use file apia tai mitä sitten tarviikaan.!!!!
};

struct Container * makeContainer(enum Policy s);

int keyCmp(struct Key * a, struct Key * b);

int writeData(struct Container * container, struct Record * record);

struct Record * copyRecord(struct Record * r);

unsigned int getIndex(struct Container * container, struct Key * pk);

struct Data getData(struct Container * container, unsigned int ind);

#endif
