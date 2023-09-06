#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "hashmap.h"
#include <stdlib.h>

struct emoji {
  uint64_t id;
  char* name;
  char* distro_id;
};

extern struct hashmap* emoji_map;

struct emoji* emoji_get_distro_emoji(char* distro_id);
void emoji_init_hashmap();
void emoji_add_to_hashmap(char* distro_id, uint64_t id, char* name);
void emoji_setup_hashmap();
bool emoji_get_formatted_emoji(char** buffer, char* distro_id);
