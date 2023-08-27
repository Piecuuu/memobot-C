# Memobot, a basic bot for memes and stuff, written in C, using [Concord](https://github.com/Cogmasters/concord), [SQLite3](https://www.sqlite.org/index.html) and [cURL](https://curl.se/).

# Prerequisites
* `Concord` installed. [Installing Concord](https://github.com/Cogmasters/concord#on-linux-bsd-and-mac-os-x)
* `libcurl` installed
* `libsqlite3` installed

# How to setup
## Create a `config.json` file
### A basic `config.json` example:
```json
{
  "logging": {
    "level": "trace",
    "filename": "bot.log",
    "quiet": true,
    "overwrite": true,
    "use_color": true,
    "http": {
      "enable": true,
      "filename": "http.log"
    }
  },
  "discord": {
    "token": "YOUR_DISCORD_TOKEN"
  }
}
```
## Change the channels in `src/memhandl.c`
### An Example
```c
// List of channels for sending submissions
u64snowflake memobot_mem_channel[2] = { // Change to the number of id's
  1105815069322924076,
  1106243204417851474
};

// Channel for submissions to be sent to
u64snowflake memobot_board_channel = 1105940744268689429;
```

# How to build
Run the following command in the bot's directory.
```bash
make
```

# How to run
Again, you need to be `cd`'d into the bot's directory.
```bash
./memobot
```
You see this after a few seconds:
```
Logged in as xxxxxxx#XXXX (XXXXXXXXXXXXXXXXX)
```
In short terms, this means that your bot is working.
