#include "memhandl.h"

// List of channels for sending submissions
u64snowflake memobot_mem_channel[2] = { // Change to the number of id's
  1105815069322924076,
  1106243204417851474
};

// Channel for submissions to be sent to
u64snowflake memobot_board_channel = 1105940744268689429;





struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(mem->memory + mem->size, contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void memobot_MemoHandler(struct discord *client, const struct discord_message_reaction_add *event) {
  bool isMemChannel = false;
  for(int i = 0; i < sizeof(memobot_mem_channel); i++) {
    if(event->channel_id == memobot_mem_channel[i]) {
      isMemChannel = true;
      break;
    }
  }

  if(!isMemChannel) return;
  if(strcmp(event->emoji->name, "👍") != 0) return;

  // Get the post (if exists)
  sqlite3_stmt* stmt;
  char* sql = "SELECT * FROM posts WHERE messageid=?";
  sqlite3_prepare_v2(memobot_DB, sql, strlen(sql), &stmt, NULL);
  char message_id[20];
  char* tmp = message_id;
  snowflake_to_char(tmp, event->message_id);
  sqlite3_bind_text(stmt, 1, message_id, strlen(message_id), SQLITE_STATIC);
  int rc = sqlite3_step(stmt);

  //int id;
  //const char *userid;
  //const unsigned char *messageid;
  //const char *mediaurl;
  int isposted = 0;

  struct discord_message message;
  struct discord_ret_message message_ret = { .sync = &message };
  CCORDcode code;

  // Get the message object and make check.
  code = discord_get_channel_message(client, event->channel_id, event->message_id, &message_ret);
  assert(CCORD_OK == code && "Couldn't fetch message");

  // Check if message has attachments
  //if(message.attachments->size <= 0) return;

  // Get the first attachment
  //struct discord_attachment att = message.attachments->array[0];

  char mid[83]; // i dont know why
  char uid[83];

  switch(rc) {
    case SQLITE_ROW:
      //id = sqlite3_column_int(stmt, 0);
      //userid = sqlite3_column_text(stmt, 1);
      //messageid = sqlite3_column_text(stmt, 2);
      //mediaurl = sqlite3_column_text(stmt, 3);
      isposted = sqlite3_column_int(stmt, 4);
      break;
    case SQLITE_DONE:
      snowflake_to_char(mid, event->message_id);
      snowflake_to_char(uid, message.author->id);

      sqlite3_stmt* insertStatement;
      sqlite3_prepare_v2(memobot_DB, "INSERT INTO posts (userid, messageid, mediaurl, isposted) VALUES (?1, ?2, ?3, false)", -1, &insertStatement, NULL);
      sqlite3_bind_text(insertStatement, 1, uid, strlen(uid), SQLITE_STATIC);
      sqlite3_bind_text(insertStatement, 2, mid, strlen(mid), SQLITE_STATIC);
      sqlite3_bind_text(insertStatement, 3, "", 0, SQLITE_STATIC);
      sqlite3_step(insertStatement);
      //id = sqlite3_column_int(insertStatement, 0);
      //isposted = sqlite3_column_int(insertStatement, 4);

      sqlite3_finalize(insertStatement);
      //sqlite3_free(insertStatement);
      break;
    default:
      // Error
      printf("error\n");
      break;
  }
  sqlite3_finalize(stmt);
  //sqlite3_free(stmt);

  // Checks if its already posted
  if(isposted == 1) return;
  // A check for the number of reactions
  if(message.reactions->array[0].count < 10) return; //! CHANGE ME

  memobot_post_to_channel(client, &message, message_id, &event->message_id, &event->channel_id);
}

void memobot_post_to_channel(struct discord* client, struct discord_message* message, char* message_id, const u64snowflake* event_message_id, const u64snowflake* event_channel_id) {
  // Set as posted
  sqlite3_stmt* isPostedStatement;
  sqlite3_prepare_v2(memobot_DB, "UPDATE posts SET isposted=1 WHERE messageid=?;", -1, &isPostedStatement, NULL);
  sqlite3_bind_text(isPostedStatement, 1, message_id, strlen(message_id), SQLITE_STATIC);
  sqlite3_step(isPostedStatement);
  sqlite3_finalize(isPostedStatement);
  //sqlite3_free(isPostedStatement);

  discord_create_reaction(client, *event_channel_id, *event_message_id, 0, "⭐", NULL);
  struct discord_attachments* atts = message->attachments;
  char content[60];
  snprintf(content, sizeof(content), "<@%" PRIu64 "> | <#%" PRIu64 ">", message->author->id, message->channel_id);

  // Get the user object for the embed author
  CCORDcode code;
  struct discord_user user;
  struct discord_ret_user user_ret = { .sync = &user };
  code = discord_get_user(client, message->author->id, &user_ret); // valgrind complains to this
  assert(CCORD_OK == code && "Couldn't fetch user.");

  // Get the user's avatar
  char full_avatar_string[95];
  snprintf(full_avatar_string, sizeof(full_avatar_string), "https://cdn.discordapp.com/avatars/%" PRIu64 "/%s.png", user.id, user.avatar);
  //printf("%s\n", full_avatar_string);

  // Combine all attachments links into a string
  int i = 0;
  // It didn't quite work
  /*char tmp_a_content[4097];
  char tmp_content2[8097];
  char buffer[2049];
  while(i < atts->size) {
    sprintf(buffer, "[%d](%s) ", i+1, atts->array[i].url);
    sprintf(tmp_content2, "%s%s", tmp_a_content, buffer);
    //tmp_content = strcat(tmp_content, buffer);
    i++;
  }
  char tmp_full_content[10000];
  sprintf(tmp_full_content, "%s\n%s", content, tmp_content2);
  char* full_content = tmp_full_content;*/


  struct discord_attachments post_attachments;
  struct discord_attachment arrAtt[10];
  // For the downloaded attachments to be free'd at the end
  void* voidArr[10];

  post_attachments.size = atts->size;
  while(i < atts->size) {
    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, atts->array[i].url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers do not like requests that are made without a user-agent
      field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {
      /*
      * Now, our chunk.memory points to a memory block that is chunk.size
      * bytes big and contains the remote file.
      *
      * Do something nice with it!
      */
      /*char *content_type;
      curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &content_type);*/
      //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);

      struct discord_attachment singular_post_attachment = {
        .content = chunk.memory,
        .size = chunk.size,
        .filename = atts->array[i].filename,
      };

      arrAtt[i] = singular_post_attachment;
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    voidArr[i] = chunk.memory;
    i++;
  }

  post_attachments.array = arrAtt;

  //char post_id[21];
  //snprintf(post_id, 21, "#%d", id);
  struct discord_embed postEmbed = {
    .color = 0xfcf403, // pissy yellow
    .description = message->content,
    .author = &(struct discord_embed_author) {
      .name = user.username,
      .icon_url = full_avatar_string
    },
    //.footer = &(struct discord_embed_footer) {
    //  .text = post_id
    //},
  };

  // The message to be posted on the board
  struct discord_create_message params = {
    .content = content,

    .embeds = &(struct discord_embeds) {
      .size = 1,
      .array = &postEmbed
    },
  };

  // Checks if it should add attachments
  if(message->attachments->size > 0) {
    params.attachments = &post_attachments;
  }

  // Create message ret
  struct discord_message sent_message;
  struct discord_ret_message sent_message_ret = { .sync = &sent_message };

  code = discord_create_message(client, memobot_board_channel, &params, &sent_message_ret);
  //assert(CCORD_OK == code && "Couldn't fetch message.");

  // Create a star reaction
  discord_create_reaction(client, sent_message.channel_id, sent_message.id, 0, "⭐", NULL);

  int i2 = 0;
  while (i2 < atts->size) {
    free(voidArr[i2]); // free attachments

    i2++;
  }
}
