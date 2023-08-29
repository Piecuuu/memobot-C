#include "distro.h"

static const char filename[] = "/etc/os-release";
char *search = "=";

void distro_get_id(char** buf) {
  char *token;

  FILE *file = fopen(filename, "r");
  if(file != NULL){
    char line[256];
    while(fgets(line, sizeof line, file) != NULL)
    {
      // Token will point to the part before the =.
      token = strtok(line, search);
      if(strcmp(token, "ID") == 0) {
        // Token will point to the part after the =.
        token = strtok(NULL, search);
        memcpy(*buf, strtok(strtok(token, "\n"), "\""), strlen(token)+1);
        break;
      }
    }
    fclose(file);
  }
}

void distro_get_pretty_name(char** buf) {
  char *token;

  FILE *file = fopen(filename, "r");
  if(file != NULL){
    char line[256];
    while(fgets(line, sizeof line, file) != NULL)
    {
      // Token will point to the part before the =.
      token = strtok(line, search);
      if(strcmp(token, "PRETTY_NAME") == 0) {
        // Token will point to the part after the =.
        token = strtok(NULL, search);
        memcpy(*buf, strtok(strtok(token, "\n"), "\""), strlen(token)+1);
        break;
      }
    }
    fclose(file);
  }
}
