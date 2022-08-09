#define URL_FILE "urls.dat"
#define MAX_URLS 10000
#define MAX_URL_LENGTH 2048

struct URL {
  char url[MAX_URL_LENGTH];
  int scanned;
};

void write_file(int total_urls, struct URL *urls);
void read_file(int *url_count, struct URL *urls);
