#include "_files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_file(int total_urls, struct URL *urls) {
  FILE *outfile = fopen(URL_FILE, "wb");
  int i;

  for (i = 0; i < total_urls; i++) {
    fwrite(&urls[i], sizeof(struct URL), 1, outfile);
  }
  fclose(outfile);
}

void read_file(int *url_count, struct URL *urls) {
  FILE *infile = fopen(URL_FILE, "rb");
  struct URL input;
  int i = 0;

  if (infile == NULL) {
    fprintf(stderr, "\nError opened file\n");
  } else {
    while (fread(&input, sizeof(struct URL), 1, infile)) {
      urls[i].scanned = input.scanned;
      strcpy(urls[i].url, input.url);
      (*url_count)++;
      i++;
    }
  }
  fclose(infile);
}
