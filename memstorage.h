#ifndef MEMSTORAGE_H
#define MEMSTORAGE_H

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
