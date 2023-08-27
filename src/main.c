#include <string.h>
#include <concord/discord.h>
#include "config.h"
#include "db.h"
#include "memhandl.h"

void on_ready(struct discord *client, const struct discord_ready *event) {
  struct discord_create_guild_application_command params = {
    .name = "ping",
    .description = "Ping command!"
  };
  discord_create_guild_application_command(client, event->application->id,
                                          (u64snowflake)GUILD_ID, &params, NULL);
  printf("Logged in as %s#%s (%" PRIu64 ")\n", event->user->username, event->user->discriminator, event->user->id);
}

void on_interaction(struct discord *client, const struct discord_interaction *event) {
  if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
    return; /* return if interaction isn't a slash command */

  if (strcmp(event->data->name, "ping") == 0) {
    discord_create_interaction_response(client, event->id,
                                        event->token, &(struct discord_interaction_response){
                                          .type = DISCORD_INTERACTION_DEFERRED_CHANNEL_MESSAGE_WITH_SOURCE,
                                        }, NULL);

    discord_edit_original_interaction_response(client, event->application_id, event->token, &(struct discord_edit_original_interaction_response){
                                                .content = "Pong!",
                                              }, NULL);
  }
}

int main(void) {
  //curl_global_init(CURL_GLOBAL_ALL); // for memobot_Memohandler
  memobot_connectDB();
  memobot_init(memobot_DB);
  struct discord *client = discord_config_init("config.json");
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_reaction_add(client, &memobot_MemoHandler);
  discord_set_on_interaction_create(client, &on_interaction);
  discord_run(client);
}
