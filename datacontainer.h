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

enum ContainerMode {
	CONTAINER_STORE_IN_FILE, CONTAINER_STORE_BUFFERED, CONTAINER_STORE_IN_MEMORY
};

typedef struct {
	int pk;
} Key;

typedef struct {
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
} Container;

Container * makeContainer(FBStorage * s);

int writeData(Container * container, Record * record);

int keyCmp(Key * a, Key * b);

Record * copyRecord(Record * r);

unsigned int getIndex(Container * container, Key * pk);

Data getData(Container * container, unsigned int ind);

#endif