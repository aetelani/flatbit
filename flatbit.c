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
#include "flatbit.h"
#include <storage.h>
#include <datacontainer.h>
#include <storage.h>
#include <stdlib.h>
#include <ev.h>

#include <stdio.h>
#include <netinet/in.h>
#include <ev.h>
#include <stdlib.h>
#include <strings.h>

#define PORT_NO 3033
#define BUFFER_SIZE 1024

struct Operation {
	int op;
	struct Record record;
};

int total_clients = 0;  // Total number of connected clients

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

int startEV()
{
	struct ev_loop *loop = ev_default_loop(0);
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	struct ev_io w_accept;

	// Create server socket
	if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
	{
	  perror("socket error");
	  return -1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NO);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to address
	if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
	{
	  perror("bind error");
	}

	// Start listing on the socket
	if (listen(sd, 2) < 0)
	{
	  perror("listen error");
	  return -1;
	}

	// Initialize and start a watcher to accepts client requests
	ev_io_init(&w_accept, accept_cb, sd, EV_READ);
	ev_io_start(loop, &w_accept);

	// Start infinite loop
	while (1)
	{
	  ev_loop(loop, 0);
	}

return 0;
}

/* Accept client requests */
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sd;
	struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Accept client request
	client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_sd < 0)
	{
	  perror("accept error");
	  return;
	}

	total_clients ++; // Increment total_clients count
	printf("Successfully connected with client.\n");
	printf("%d client(s) connected.\n", total_clients);

	// Initialize and start watcher to read client requests
	ev_io_init(w_client, read_cb, client_sd, EV_READ);
	ev_io_start(loop, w_client);
}

/* Read client message */
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct Operation * buffer = malloc(sizeof(struct Operation));
	ssize_t read;

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Receive message from client socket
	read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

	if(read < 0)
	{
	  perror("read error");
	  return;
	}

	if(read == 0)
	{
	  // Stop and free watchet if client socket is closing
	  ev_io_stop(loop,watcher);
	  free(watcher);
	  perror("peer might closing");
	  total_clients --; // Decrement total_clients count
	  printf("%d client(s) connected.\n", total_clients);
	  return;
	}
	else
	{
	  printf("operation:%d\n", buffer->record.data.data);
	}

	// Send message bach to the client
	send(watcher->fd, buffer, read, 0);
	bzero(buffer, read);
}



//!TODO multitable, multifile, abstract file & format
// Init File actually, one table, one file for now.

int main()
{
	startEV();
	enum Policy policy = CONTAINER_STORAGE_MEMORY;
	
    struct Container *c = makeContainer(policy);
    c->storage->open(c);

    int recordCnt = 3;
    for (int i=0; i < recordCnt; i++)
    {
        struct Key * key = calloc(1,sizeof(struct Key));
        key->pk = i;
        struct Data data = { .data = i };
        struct Record rec = { .key = key, .data = data };
        c->storage->write(c, &rec);
    }

	struct Record * r = malloc(sizeOfRecord);
    for (int i=1; i < recordCnt; i++)
    {
        struct Key key = { .pk = i };
        unsigned int index  = c->storage->getIndex(c, &key);
        int res = -1;
        if (index)
        { 
			printf("flatbit.c with index: %i\n", index);
			res = c->storage->read(c, r, index);
			c->storage->delete(c, index);
		}
    }
    
    if (c)
    {
        //printf("Records: %i\n", c->records);
        //printf("Founded index: %i\n", index);
        //printf("Data digged:%i", d.data);
    } else
        printf("Failed miserably\n");

    return 0;
}
