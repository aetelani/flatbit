#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <datacontainer.h>
#include <flatbit.h>

#define PORT_NO 3033

int main()
{
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	struct Flatbit backend;

	// Create client socket
	if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
	{
	  perror("socket error");
	  return -1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NO);
	addr.sin_addr.s_addr =  htonl(INADDR_ANY);

	// Connect to server socket
	if(connect(sd, (struct sockaddr *)&addr, sizeof addr) < 0)
	{
	  perror("Connect error");
	  return -1;
	}

    const int recordCnt = 3;
    for (int i=0; i < recordCnt; i++)
    {
        struct Data td = { .data = i };
        struct Key tk = { .pk = i };
        struct Record rec = { .key = tk, .data = td };
//        c->storage->write(c, &rec);
		backend.op = FB_WRITE;
		backend.record.data = td;
		backend.record.key = tk;
		backend.record.status = FB_KEY | FB_DATA;
		
        send(sd, &backend, sizeof(struct Flatbit), 0);
        perror("send write op");
    }

    for (int i=1; i < recordCnt; i++)
    {
        struct Key key = { .pk = i };
//        unsigned int index  = c->storage->getIndex(c, &key);
        int res = -1;
        if (index)
        { 
//			printf("flatbit.c with index: %i\n", index);
			backend.op = FB_READ;
			send(sd, &backend, sizeof(struct Flatbit), 0);
			perror("send read op");
			/*****/
			struct Flatbit * buffer = malloc(sizeof(struct Flatbit));
			ssize_t read = recv(sd, buffer, sizeof(struct Flatbit), 0);
			printf("read op:%d \n", buffer->op);
			//send(sd, buffer, sizeof(struct Operation), 0);
			/*******/
			//res = c->storage->read(c, r, index);
			
			backend.op = FB_DELETE;
			backend.record.status = FB_KEY;
			backend.record.key = key;
			send(sd, &backend, sizeof(struct Flatbit), 0);
			perror("send delete op");
//			c->storage->delete(c, index);
		}
    }
    close(sd);

	return 0;
}
