#include <concord/discord.h>
#include <sqlite3.h>
#include <string.h>
#include "utils.h"
#include <assert.h>
#include <stdlib.h>

extern sqlite3* memobot_DB;
void memobot_MemoHandler(struct discord *client, const struct discord_message_reaction_add *event);
void memobot_post_to_channel(struct discord* client, struct discord_message* message, char* message_id, const u64snowflake* event_message_id, const u64snowflake* event_channel_id);
void memobot_create_row(char* message_id, char* author_id);
