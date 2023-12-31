#include <string.h>
#include <concord/discord.h>
#include "config.h"
#include "db.h"
#include "memhandl.h"
#include "emoji.h"
#include "distro.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>

void on_ready(struct discord *client, const struct discord_ready *event) {
  struct discord_create_guild_application_command params = {
    .name = "ping",
    .description = "Ping command!"
  };
  discord_create_guild_application_command(client, event->application->id,
                                          (u64snowflake)GUILD_ID, &params, NULL);
  discord_create_global_application_command(client, event->application->id,
                                            &(struct discord_create_global_application_command){
                                              .name = "info",
                                              .description = "Bot info"
                                            }, NULL);

  discord_create_global_application_command(client, event->application->id,
                                            &(struct discord_create_global_application_command) {
                                              .name = "force",
                                              .description = "Force a submission",
                                              .options = &(struct discord_application_command_options) {
                                                .array = &(struct discord_application_command_option)
                                                  {
                                                    .name = "messageid",
                                                    .description = "Message ID",
                                                    .required = true,
                                                    .type = (enum discord_application_command_option_types)DISCORD_APPLICATION_OPTION_STRING,
                                                  },
                                                .size = 1,
                                              }
                                            }, NULL);
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
  } else if(strcmp(event->data->name, "info") == 0) {
    discord_create_interaction_response(client, event->id,
                                        event->token, &(struct discord_interaction_response){
                                          .type = DISCORD_INTERACTION_DEFERRED_CHANNEL_MESSAGE_WITH_SOURCE,
                                        }, NULL);

    struct utsname buffer;

    char cde[64];
    char* current_distro_emoji = cde;
    char dpn[512];
    char* distro_pretty_name = dpn;
    char le[64];
    char* os_emoji = le;

    if (uname(&buffer) < 0) {
      perror("uname");
      exit(EXIT_FAILURE);
    }
    if(strcmp(buffer.sysname, "Linux") == 0) {
      char* distro_id = malloc(sizeof(char)*64);
      distro_get_id(&distro_id);
      distro_get_pretty_name(&distro_pretty_name);

      char* tmp_linux_emoji = malloc(64);
      bool result = emoji_get_formatted_emoji(&current_distro_emoji, distro_id);
      emoji_get_formatted_emoji(&tmp_linux_emoji, "linux");
      if(result == false) {
        current_distro_emoji = tmp_linux_emoji;
      }
      os_emoji = tmp_linux_emoji;

      free(distro_id);
    } else if(strcmp(buffer.sysname, "Darwin") == 0) {
      emoji_get_formatted_emoji(&os_emoji, "apple");
    } else if(strcmp(buffer.sysname, "OpenBSD") == 0) {
      emoji_get_formatted_emoji(&os_emoji, "openbsd");
    } else if(strcmp(buffer.sysname, "FreeBSD") == 0) {
      emoji_get_formatted_emoji(&os_emoji, "freebsd");
    } else if(strcmp(buffer.sysname, "NetBSD") == 0) {
      emoji_get_formatted_emoji(&os_emoji, "netbsd");
    } else if(strcmp(buffer.sysname, "Windows") == 0) {
      emoji_get_formatted_emoji(&os_emoji, "windows");
    } else {
      os_emoji = "";
    }
    char distro[256];
    snprintf(distro, sizeof(distro), "%s `%s`", current_distro_emoji, distro_pretty_name);

    char os[256];
    snprintf(os, sizeof(os), "%s `%s`", os_emoji, buffer.sysname);

    char arch[256];
    snprintf(arch, sizeof(arch), "`%s`", buffer.machine);

    struct sysinfo sysinfo_buffer;
    sysinfo(&sysinfo_buffer);


    struct discord_embed_field fields[] = {
      {
        .Inline = true,
        .name = "Operating system",
        .value = os,
      },
      #ifdef __linux
      {
        .Inline = true,
        .name = "Distro",
        .value = distro,
      },
      #endif
      {
        .Inline = true,
        .name = "Arch",
        .value = arch,
      },
    };

    struct discord_embed embed = {
      .fields = &(struct discord_embed_fields){
        .array = fields,
        .size = sizeof(fields) / sizeof *fields,
      }
    };

    discord_edit_original_interaction_response(client, event->application_id, event->token, &(struct discord_edit_original_interaction_response){
                                                .embeds = &(struct discord_embeds){
                                                  .array = &embed,
                                                  .size = 1,
                                                },
                                              }, NULL);

    free(current_distro_emoji);
    free(os_emoji);
  } else if(strcmp(event->data->name, "force") == 0) {
    if(event->member->user->id != FORCE_COMMAND_USER_ID) return;
     discord_create_interaction_response(client, event->id,
                                          event->token, &(struct discord_interaction_response){
                                          .type = DISCORD_INTERACTION_DEFERRED_CHANNEL_MESSAGE_WITH_SOURCE,
                                          .data = &(struct discord_interaction_callback_data) {
                                            .flags = DISCORD_MESSAGE_EPHEMERAL
                                          }
                                        }, NULL);

    char* message_id = event->data->options->array[0].value;

    struct discord_message message;
    struct discord_ret_message message_ret = { .sync = &message };
    CCORDcode code;

    // Get the message object and make check.
    code = discord_get_channel_message(client, event->channel_id, char_to_snowflake(message_id), &message_ret);
    assert(CCORD_OK == code && "Couldn't fetch message");

    char uid[83];
    snowflake_to_char(uid, message.author->id);

    memobot_create_row(message_id, uid);
    memobot_post_to_channel(client, &message, message_id, &message.id, &message.channel_id);
    discord_edit_original_interaction_response(client, event->application_id, event->token, &(struct discord_edit_original_interaction_response){
                                                .content = "Completed without errors.",
                                              }, NULL);
  }
}

int main(void) {
  //curl_global_init(CURL_GLOBAL_ALL); // for memobot_Memohandler
  emoji_init_hashmap();
  emoji_setup_hashmap();
  memobot_connectDB();
  memobot_init(memobot_DB);
  struct discord *client = discord_config_init("config.json");
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_reaction_add(client, &memobot_MemoHandler);
  discord_set_on_interaction_create(client, &on_interaction);
  discord_run(client);

  return 0;
}
