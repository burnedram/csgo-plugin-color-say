# colorsay
A CSGO plugin for displaying colored text.

This plugin uses the AlliedModders CSGO SDK [mirror](https://github.com/alliedmodders/hl2sdk/tree/csgo)

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
