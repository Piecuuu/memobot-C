#include "emoji.h"

struct hashmap* emoji_map;

int emoji_compare(const void *a, const void *b, void *udata) {
  const struct emoji *ua = a;
  const struct emoji *ub = b;
  return strcmp(ua->distro_id, ub->distro_id);
}

bool emoji_iter(const void *item, void *udata) {
  const struct emoji *emoji = item;
  printf("EID: %" PRIu64 "\nENAME: %s\n", emoji->id, emoji->name);
  return true;
}

uint64_t emoji_hash(const void *item, uint64_t seed0, uint64_t seed1) {
  const struct emoji *user = item;
  return hashmap_sip(user->distro_id, strlen(user->distro_id), seed0, seed1);
}

void emoji_setup_hashmap() {
  emoji_add_to_hashmap("apple", 1145830544169500722, "apple");
  emoji_add_to_hashmap("artix", 1145830551610212403, "artix");
  emoji_add_to_hashmap("void", 1145830641473163397, "void");
  emoji_add_to_hashmap("debian", 1145830557159272599, "debian");
  emoji_add_to_hashmap("alma", 1145830534514217082, "alma");
  emoji_add_to_hashmap("alpine", 1145830536019972157, "alpine");
  emoji_add_to_hashmap("android", 1145830539253788793, "android");
  emoji_add_to_hashmap("antix", 1145830542252703936, "antix");
  emoji_add_to_hashmap("arch", 1145830546983882752, "arch");
  emoji_add_to_hashmap("arco", 1145830548590305300, "arco");
  emoji_add_to_hashmap("clear", 1145830553870925984, "clear");
  emoji_add_to_hashmap("deepin", 1145830558971199498, "deepin");
  emoji_add_to_hashmap("elementary", 1145830561705885706, "elementary");
  emoji_add_to_hashmap("endeavour", 1145830563459117207, "endeavour");
  emoji_add_to_hashmap("endless", 1145830566042796173, "endless");
  emoji_add_to_hashmap("fedora", 1145830568014131262, "fedora");
  emoji_add_to_hashmap("feren", 1145830571315048599, "feren");
  emoji_add_to_hashmap("kaos", 1145830763737133096, "kaos");
  emoji_add_to_hashmap("knoppix", 1145830574146211870, "knoppix");
  emoji_add_to_hashmap("kubuntu", 1145830576847335444, "kubuntu");
  emoji_add_to_hashmap("openbsd", 1145830599668543488, "openbsd");
  emoji_add_to_hashmap("openmandriva", 1145830602210279444, "openmandriva");
  emoji_add_to_hashmap("parrot", 1145831287257567312, "parrot");
  emoji_add_to_hashmap("pclinuxos", 1145830607075680310, "pclinuxos");
  emoji_add_to_hashmap("peppermint", 1145830610502426634, "peppermint");
  emoji_add_to_hashmap("raspios", 1145830612662501546, "raspios");
  emoji_add_to_hashmap("rebornos", 1145830615917285397, "rebornos");
  emoji_add_to_hashmap("redhat", 1145830618878447707, "redhat");
  emoji_add_to_hashmap("rosa", 1145830787258789971, "rosa");
  emoji_add_to_hashmap("septor", 1145830624926629959, "septor");
  emoji_add_to_hashmap("slackware", 1145830626830843936, "slackware");
  emoji_add_to_hashmap("solus", 1145830629460693102, "solus");
  emoji_add_to_hashmap("suse", 1145831360192323696, "suse");
  emoji_add_to_hashmap("ubuntu_dde", 1145830632912584786, "ubuntu_dde");
  emoji_add_to_hashmap("ubuntu_mate", 1145831447710670858, "ubuntu_mate");
  emoji_add_to_hashmap("ubuntu_unity", 1145830638772035596, "ubuntu_unity");
  emoji_add_to_hashmap("windows", 1145830643670995124, "windows");
  emoji_add_to_hashmap("xubuntu", 1145830742035800074, "xubuntu");
  emoji_add_to_hashmap("zorin", 1145830647781404814, "zorin");
  emoji_add_to_hashmap("bluestar", 1146061780402045058, "bluestar");
  emoji_add_to_hashmap("bodhi", 1146061781819719710, "bodhi");
  emoji_add_to_hashmap("bunsenlabs", 1146061784852205669, "bunsenlabs");
  emoji_add_to_hashmap("centos", 1146061788354453644, "centos");
  emoji_add_to_hashmap("freebsd", 1146061790845861909, "freebsd");
  emoji_add_to_hashmap("garuda", 1146061793886752881, "garuda");
  emoji_add_to_hashmap("gentoo", 1146061796298469406, "gentoo");
  emoji_add_to_hashmap("kali", 1146061798391431249, "kali");
  emoji_add_to_hashmap("manjaro", 1146061800983494837, "manjaro");
  emoji_add_to_hashmap("mint", 1146061802677997619, "mint");
  emoji_add_to_hashmap("mx", 1146061805886656614, "mx");
  emoji_add_to_hashmap("neon", 1146061808927522866, "neon");
  emoji_add_to_hashmap("pop", 1146061811888697414, "pop");
  emoji_add_to_hashmap("puppy", 1146061814803738664, "puppy");
  emoji_add_to_hashmap("q4os", 1146061816401764462, "q4os");
  emoji_add_to_hashmap("qubes", 1146061819077742663, "qubes");
  emoji_add_to_hashmap("tails", 1146061820956790854, "tails");
  emoji_add_to_hashmap("tinycore", 1146061823158796331, "tinycore");
  emoji_add_to_hashmap("ubuntu", 1146061851331936276, "ubuntu");
  emoji_add_to_hashmap("ubuntu_cinnamon", 1146061826484879360, "ubuntu_cinnamon");
  emoji_add_to_hashmap("linux", 1145831080256082111, "linux");
  emoji_add_to_hashmap("lite", 1145830580664143994, "lite");
  emoji_add_to_hashmap("lubuntu", 1145831154520440974, "lubuntu");
  emoji_add_to_hashmap("mabox", 1145830585135267923, "mabox");
  emoji_add_to_hashmap("mageia", 1145830588557836389, "mageia");
  emoji_add_to_hashmap("netbsd", 1145831211999166464, "netbsd");
  emoji_add_to_hashmap("netrunner", 1145830592341098588, "netrunner");
  emoji_add_to_hashmap("nixos", 1145830596761878630, "nixos");
  emoji_add_to_hashmap("rocky", 1149013740545654825, "rocky");
}

void emoji_add_to_hashmap(char* distro_id, uint64_t id, char* name) {
  hashmap_set(emoji_map, &(struct emoji){ .id = id, .name = name, .distro_id = distro_id });
}

void emoji_init_hashmap() {
  emoji_map = hashmap_new(sizeof(struct emoji), 0, 0, 0,
                          emoji_hash, emoji_compare, NULL, NULL);
}

struct emoji* emoji_get_distro_emoji(char* distro_id) {
  return (struct emoji*)hashmap_get(emoji_map, &(struct emoji){ .distro_id = distro_id });
}

bool emoji_get_formatted_emoji(char** buffer, char* distro_id) {
  struct emoji* distro_emoji = emoji_get_distro_emoji(distro_id);
  if(distro_emoji == NULL) {
    return false;
  }
  size_t length = sizeof(char)*63;
  char* str_emoji = malloc(64);
  snprintf(str_emoji, length, "<:%s:%"PRIu64">", distro_emoji->name, distro_emoji->id);
  *buffer = str_emoji;
  memcpy(*buffer, str_emoji, sizeof(*str_emoji));
  return true;
}
