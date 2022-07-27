#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	  size_t realsize = size * nmemb;
	    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	      char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	        if(!ptr) {
	        	    /* out of memory! */
	        	    printf("not enough memory (realloc returned NULL)\n");
	        	        return 0;
	        	          }

	          mem->memory = ptr;
	            memcpy(&(mem->memory[mem->size]), contents, realsize);
	              mem->size += realsize;
	                mem->memory[mem->size] = 0;

	                  return realsize;
}


struct MemoryStruct * get_url(char url[1000]) {

	CURL *curl;
	CURLcode res;

	struct MemoryStruct * chunk;
	chunk = malloc(sizeof(struct MemoryStruct));
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
