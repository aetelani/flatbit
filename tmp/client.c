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
struct write_result write_result;

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
	assert(stream && ptr);
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        printf("error: too small buffer\n");
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
    long code;
	write_result.data = malloc(BUFFER_SIZE);

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);    

    status = curl_easy_perform(curl);
    if(status != 0)
    {
        printf("error: unable to request data from %s:\n", url);
        printf("%s\n", curl_easy_strerror(status));
        return NULL;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200)
    {
        printf("error: server responded with code %ld\n", code);
        return NULL;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
	write_result.data[write_result.pos] = '\0';

    return write_result.data;
}

enum key_type {
	KEYTYPE_UNDEF, KEYTYPE_CTRLKEY
};
enum control_key {
	CTRLKEY_UNDEF, CTRLKEY_VISUAL_TYPE
};

int struct_depth = 0;
int print_keys(json_t * obj, enum key_type parentCtx)
{
	const char *key;
	json_t *value;
	enum key_type  keyType = KEYTYPE_UNDEF;
	enum control_key controlKey = CTRLKEY_UNDEF;
	void * iter = json_object_iter(obj);	
	while(iter)
	{
		key = json_object_iter_key(iter);
		value = json_object_iter_value(iter);		
		keyType = (key && key[0] == '_') ? KEYTYPE_CTRLKEY: KEYTYPE_UNDEF;

		if (keyType == KEYTYPE_CTRLKEY)
			printf("Switch ");
		else if (parentCtx == KEYTYPE_CTRLKEY)
			printf("");
		else
			printf("");
			
		if (parentCtx == KEYTYPE_CTRLKEY)
		{
			printf("Option: %s", key);
		}
		else
		{
			printf("Key: %s ", key);
		}


		switch (json_typeof(value)) {
		case JSON_STRING:
				printf(": %s", json_string_value(value));
				break;
		case JSON_INTEGER:
				printf(": %i", (int)json_integer_value(value));
				break;
		case JSON_REAL:
				printf(": %f", (int)json_number_value(value));
				break;
		case JSON_ARRAY:
				printf(": [...]");
				break;
		case JSON_OBJECT:
			break;				
		case JSON_TRUE:
		case JSON_FALSE:
		case JSON_NULL: 
		default:
			printf("skipped an atom");
		}
		printf("\n");

		if (json_is_object(value))
		{
			struct_depth++;
			iter = json_object_iter_next(obj, iter);

			if (!iter)
			{
				for(int i=struct_depth; i--;) printf("\t");				
				print_keys(value, keyType);
				break;
			} else
			{
				for(int i=struct_depth; i--;) printf("\t");				
				print_keys(value, keyType);
			}
		}
		else
		{			
			iter = json_object_iter_next(obj, iter);

			if (!iter)
				struct_depth--;
							
			for(int i=struct_depth; i--;) printf("\t");
		}
	}	
	return 0; //json_object_size(obj);
}

int parse_json()
{
	size_t i;
	char *text;
	char url[URL_SIZE];

	json_t *root;
	json_error_t error;

//	text = request("https://api.github.com/repos/aetelani/flatbit/commits");
	text = request("http://localhost/bitson_ex1.bitson");
	if(!text)
		return 1;

	root = json_loads(text, 0, &error);	
	free(text);    
	
	if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }

//	json_t *data, *sha, *commit, *message;

	print_keys(root, CTRLKEY_UNDEF);
	
	const char *message_text;
	for(i = 0; i < json_object_size(root); i++)
	{	
/*		data = json_array_get(root, i);
		assert(json_is_object(data));
		sha = json_object_get(data, "sha");
		assert(json_is_string(sha));
		commit = json_object_get(data, "commit");
		assert(json_is_object(commit));
		message = json_object_get(commit, "message");
		assert(json_is_string(message));		
		message_text = json_string_value(message);
		assert(message_text);		
		printf("%.8s %.*s\n", json_string_value(sha), newline_offset(message_text), message_text);*/
	}

	json_decref(root);
}

int main()
{
	parse_json();
	
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
