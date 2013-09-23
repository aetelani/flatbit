#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <jansson.h>
#include <curl/curl.h>
#include <datacontainer.h>
#include <flatbit.h>

#define PORT_NO 3033
#define BUFFER_SIZE  (256 * 1024)
#define URL_SIZE     256

struct write_result
{
    char *data;
    int pos;
};

/* Return the offset of the first newline in text or the length of
   text if there's no newline */
static int newline_offset(const char *text)
{
    const char *newline = strchr(text, '\n');
    if(!newline)
        return strlen(text);
    else
        return (int)(newline - text);
}

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

static char *request(const char *url)
{
    CURL *curl;
    CURLcode status;
    char * data;
    long code;

    curl = curl_easy_init();

	struct write_result write_result = {
			.data = data,
			.pos = 0
		};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);    

    status = curl_easy_perform(curl);
    if(status != 0)
    {
        fprintf(stderr, "error: unable to request data from %s:\n", url);
        fprintf(stderr, "%s\n", curl_easy_strerror(status));
        return NULL;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200)
    {
        fprintf(stderr, "error: server responded with code %ld\n", code);
        return NULL;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return data;
}

int parse_json()
{
	size_t i;
	char *text;
	char url[URL_SIZE];

	json_t *root;
	json_error_t error;

	text = request("https://api.github.com/repos/aetelani/flatbit/commits");
	if(!text)
		return 1;

	root = json_loads(text, 0, &error);
	free(text);    
	assert(root && json_is_array(root));

	json_t *data, *sha, *commit, *message;
	const char *message_text;
	for(i = 0; i < json_array_size(root); i++)
	{
		data = json_array_get(root, i);
		assert(json_is_object(data));

		sha = json_object_get(data, "sha");
		assert(json_is_string(sha));

		commit = json_object_get(data, "commit");
		assert(json_is_object(commit));

		message = json_object_get(commit, "message");
		assert(json_is_string(message));

		message_text = json_string_value(message);
		assert(message_text);

		printf("%.8s %.*s\n", json_string_value(sha), newline_offset(message_text), message_text);
	}

	json_decref(root);
}

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

	return 0;
}
