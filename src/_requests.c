#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "_requests.h"


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	MemoryStruct *mem = (MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}


// TODO: make this a pointer
MemoryStruct * get_url(char url[2048]) {

	CURL *curl;
	CURLcode res;

	MemoryStruct * chunk;
	chunk = malloc(sizeof(MemoryStruct));
	chunk->memory = malloc(1);
	chunk->size = 0;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl);

	}
	return chunk;
}
