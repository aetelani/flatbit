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
#ifndef MEMSTORAGE_H
#define MEMSTORAGE_H

#define MEM_BASE_IND 1

struct Container;
struct Record;
struct Key;

int memStorageOpen(struct Container *c);

int memStorageClose(struct Container *c);

int memReadRecord(struct Container *c, struct Record * out, unsigned int ind);

int memWriteRecord(struct Container *c, struct Record * in);

int memWriteHeader(struct Container *c);

unsigned int memGetIndex(struct Container *c, struct Key * key);

#endif
