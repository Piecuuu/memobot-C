#include <concord/discord.h>
#include <sqlite3.h>
#include <string.h>
#include "utils.h"
#include <assert.h>
#include <stdlib.h>

extern sqlite3* memobot_DB;
void memobot_MemoHandler(struct discord *client, const struct discord_message_reaction_add *event);
