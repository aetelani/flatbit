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

#pragma once

#include <assert.h>
#include <datacontainer.h>

enum StorageOpenMode {
	FILE_CLOSED, FILE_OPEN_EXCLUSIVE, FILE_OPEN_SHARED,
	FILE_OPEN_WAL, FILE_OPEN_DIRECT
};

enum FBInterface {
	FB_WRITE, FB_READ, FB_DELETE, FB_RESPONSE_OK
};

enum FBType {
	FB_EMPTY = 0, FB_KEY = 1, FB_DATA = 2, FB_INDEX = 4,
	FB_FULL = FB_KEY | FB_DATA | FB_INDEX
};

struct Flatbit {
	enum FBInterface op;
	struct Record record;
};


//void impFBSetHandle(FILE*h) {}
