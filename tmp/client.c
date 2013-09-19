#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <datacontainer.h>

#define PORT_NO 3033

struct Operation {
	int op;
	struct Record record;
};

int main()
{
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	struct Operation op;

	op.record.key.pk = 1;
	op.record.data.data = 1;


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
	
	send(sd, &op, sizeof(struct Operation), 0);

	return 0;
}
