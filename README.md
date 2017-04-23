# colorsay
A CSGO plugin for displaying colored text.  
Tested on Ubuntu 16.04.2 LTS (4.4.0-59-generic, x86_64), but should work any debian based distro.  

I have no intention of creating a Windows version of this, but it should just be a matter of compiling.  
This plugin uses the AlliedModders CSGO SDK [mirror](https://github.com/alliedmodders/hl2sdk/tree/csgo).

# Table of Contents
   * [Releases](#releases)
   * [Features](#features)
      * [Server commands](#server-commands)
      * [Server convars](#server-convars)
      * [Client commands](#client-commands)
      * [Features to come](#features-to-come)
   * [How to add color to your messages](#how-to-add-color-to-your-messages)
      * [Color tags](#color-tags)
      * [Examples](#examples)
   * [Compilation](#compilation)
   * [Installation](#installation)


# Releases
Download either the .zip or .tar.gz and unpack it to `${CSGO_DS_DIR}/csgo/addons`.
* [v0.9](https://github.com/burnedram/csgo-plugin-color-say/releases/tag/v0.9) - First release

# Features
## Server commands
These can be used through the server console, or through `rcon`
* `colorsay`      - All chat
* `colorsay_spec` - Spectators
* `colorsay_t`    - Terrorists
* `colorsay_ct`   - Counter-Terrorists
## Server convars
* `colorsay_allow_client` - Enable to allow client commands (disabled by default)
## Client commands
These can be used by players through their console.  
By default these are disabled, but can be enabled with the server convar `colorsay_allow_client 1`.
* `colorsay say`      - All chat
* `colorsay say_team` - Player's team chat
* `colorsay echo`     - Echo a message to yourself, only visible to you!
* `colorsay list`     - List all available colors, only visible to you!
* `colorsay help`     - Display available commands, or get help about a particular command
## Features to come
* A server command `colorsay_id` which will send a message to only the specified player(s).

# How to add color to your messages
All chat related commands are colorized by formatting the message with color tags. 
## Color tags
A color tag starts with `{#` and ends with `}`. The text inside the tag defines what color to use.  
To define a color you can either use their integer value, or their name.

| Value | Name            | Color |
|:-----:|:---------------:|:-----:|
| 1     | White           |![#ffffff](https://placehold.it/15/ffffff/000000?text=+)|
| 2     | Red             |![#ee0202](https://placehold.it/15/ee0202/000000?text=+)|
| 3     | Lavender        |![#b981ef](https://placehold.it/15/b981ef/000000?text=+)|
| 4     | Green           |![#40fe40](https://placehold.it/15/40fe40/000000?text=+)|
| 5     | Pale green      |![#befe8f](https://placehold.it/15/befe8f/000000?text=+)|
| 6     | Green yellow    |![#a1fe47](https://placehold.it/15/a1fe47/000000?text=+)|
| 7     | Coral red       |![#fe4040](https://placehold.it/15/fe4040/000000?text=+)|
| 8     | Light grey      |![#c4c9cf](https://placehold.it/15/c4c9cf/000000?text=+)|
| 9     | Light Goldenrod |![#ece37a](https://placehold.it/15/ece37a/000000?text=+)|
| 10    | Echo blue       |![#aec1d7](https://placehold.it/15/aec1d7/000000?text=+)|
| 11    | Picton blue     |![#5e97d8](https://placehold.it/15/5e97d8/000000?text=+)|
| 12    | Royal blue      |![#4b69fe](https://placehold.it/15/4b69fe/000000?text=+)|
| 13    | Purple          |![#8847fe](https://placehold.it/15/8847fe/000000?text=+)|
| 14    | Neon purple     |![#d22ce5](https://placehold.it/15/d22ce5/000000?text=+)|
| 15    | Vermilion       |![#ea4b4b](https://placehold.it/15/ea4b4b/000000?text=+)|
| 16    | Orange gold     |![#e1ac39](https://placehold.it/15/e1ac39/000000?text=+)|
## Examples
* `colorsay I am {#royal blue}blue{#white} da ba dee da ba die...`
* `colorsay I am {#12}blue{#1} da ba dee da ba die...`
* `colorsay {#2}R{#16}a{#9}i{#4}n{#11}b{#12}o{#13}w`

# Compilation
## You need
* `pkg-config`
* `protoc 2.5.0`
* `g++-4.6`, and `g++-4.6-multilib` if you're compiling on a 64-bit machine.
* `git submodule init && git submodule update`

## protoc
A copy of the protobuf 2.5.0 source code is provided with the AlliedModders mirror, located in  
`hl2sdk-csgo/common/protobuf-2.5.0/src`  
But it seems to be missing some key compilation components (`autogen.sh`, `configure`),  
so download it from [source](https://github.com/google/protobuf/releases/tag/v2.5.0) instead

You can follow the official installation [guide](https://github.com/google/protobuf/tree/master/src#c-installation---unix), unless you're using a 64-bit distro, and want to run this 32-bit plugin on it. By default it will compile it as a 64-bit program and library, which can't be loaded into `srcds_linux`

To fix this run:
```
./configure --build=i686-pc-linux-gnu \
            --host=x86_64-pc-linux-gnu \
            "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
```
instead of just `./configure`.  
Also, it will probably help to add `--prefix=/usr` as mentioned in the guide

## Actually compiling the plugin
`make`

# Installation
 * Run `make install`  
 Or
 * Copy `affesay.so` and `affesay.vdf` into `${CSGO_DS_DIR}/csgo/addons`.
