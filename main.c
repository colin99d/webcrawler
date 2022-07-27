#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
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

char* substr(const char *src, int m, int n)
{
	// get the length of the destination string
	int len = n - m;

	// allocate (len + 1) chars for destination (+1 for extra null character)
	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	// extracts characters between m'th and n'th index from source string
	// and copy them into the destination string
	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
			*dest = *(src + i);
			dest++;
	}

	// null-terminate the destination string
	*dest = '\0';

	// return the destination string
	return dest - len;
}

void check_href(char *html) {
	int i;
	int j;
	char *sub_str;
	char *tmp_str;

	for (i = 0; i < strlen(html); i++) {
		sub_str = substr(html, i, i+5);
		if (strcmp(sub_str, "href=") == 0) {
			i += 7;
			j = i;
			while (strcmp(&html[i], "\"") != 0) {
				printf("%c", html[i]);
				i++;
			}
			tmp_str = substr(html, j, i);

			printf("%s\n", sub_str);
			printf("%s\n", tmp_str);
		}
	}
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

int main() {
	struct MemoryStruct * html = get_url("https://www.example.com");
  printf("%s\n", html->memory);
	/* check_href(html->memory); */
	return 0;
}
