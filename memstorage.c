#include <memstorage.h>

int memStorageOpen(struct Container * c)
{
	return 0;
}

int memStorageClose(struct Container * c)
{
	return 0;
}

int memReadRecord(struct Container *container, struct Record * recordOut, unsigned int index)
{
	return 0;
}

int memWriteRecord(struct Container * container, struct Record * record)
{
	return 0;
}

int memWriteHeader(struct Container * container)
{
	return 0;
}

unsigned int memGetIndex(struct Container * container, struct Key * pk)
{
	return 0;
}
