#include <stdlib.h>
#include <string.h>

#include <jansson.h>
#include <curl/curl.h>

static char *request(const char *url)
{
    CURL *curl;
    CURLcode status;
    long code;

    curl = curl_easy_init(););


    curl_easy_setopt(curl, CURLOPT_URL, url);

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

int main(int argc, char *argv[])
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
    return 0;
}
