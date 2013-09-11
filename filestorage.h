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
#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#define FILE_BASE_IND 0

struct Container;
struct Record;
struct Key;
struct FileStorage
{
	char * id;
}; //probably could use this instead of Container

typedef struct FileStorage FBFileStorage;

int fileStorageOpen(struct Container *);

int fileStorageClose(struct Container *);

int fileReadRecord(struct Container *container, struct Record * recordOut, unsigned int index);

int fileWriteRecord(struct Container * container, struct Record * record);

int fileWriteHeader(struct Container * container);

unsigned int fileGetIndex(struct Container * container, struct Key * pk);
#endif
