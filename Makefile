# install g++-4.6 g++-4.6-multilib
# https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html
# 4.6 is GLIBCXX_3.4.15, use the newest version that csgo-ds has, which can be found using
# strings csgo-ds/bin/libstdc++.so.6 | grep "^GLIBCXX_"
GCC := g++-4.6
WARNINGS := -Wall -Werror
OPTIONS := -m32 -pipe -msse -mfpmath=sse -std=c++0x
# When updating to a newer version of g++, use -std=c++11
FLAGS := -fvisibility=hidden -fvisibility-inlines-hidden -fexceptions -fno-threadsafe-statics
EXTRA_FLAGS := -fno-rtti
DEFINES := -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_snprintf=snprintf \
		  -D_vsnprintf=vsnprintf -DGNUC -D_LINUX -DPOSIX -DCOMPILER_GCC
INCLUDES := -Ihl2sdk-csgo/public -Ihl2sdk-csgo/public/engine -Ihl2sdk-csgo/public/mathlib \
		   -Ihl2sdk-csgo/public/tier0 -Ihl2sdk-csgo/public/tier1 -Ihl2sdk-csgo/public/game/server

SOURCE_DIR := src
BUILD_DIR := build
CSGO_LIB_DIR := hl2sdk-csgo/lib/linux
CSGO_LIBS := $(addprefix $(CSGO_LIB_DIR)/,libtier0.so libvstdlib.so tier1_i486.a interfaces_i486.a)
CSGO_LINKING := -Lhl2sdk-csgo/lib/linux $(patsubst $(CSGO_LIB_DIR)/%.a,-l:%.a,$(patsubst $(CSGO_LIB_DIR)/lib%.so,-l%, $(CSGO_LIBS)))
PROTOBUF_LINKING := $(patsubst -lprotobuf,-l:libprotobuf.a, $(shell pkg-config --cflags --libs protobuf))

PROTOBUF_FLAGS := $(WARNINGS) $(OPTIONS) $(FLAGS) -I$(SOURCE_DIR)/protobuf
PLUGIN_FLAGS := $(WARNINGS) $(OPTIONS) $(FLAGS) $(EXTRA_FLAGS) $(DEFINES) $(INCLUDES) -I$(SOURCE_DIR) -I$(SOURCE_DIR)/protobuf

NETMESSAGES_PB_SOURCE := $(addprefix $(SOURCE_DIR)/protobuf/,netmessages.pb.cc netmessages.pb.h) 
CSTRIKE_PB_SOURCE := $(addprefix $(SOURCE_DIR)/protobuf/,cstrike15_usermessages.pb.cc cstrike15_usermessages.pb.h) 
PROTOBUF_SOURCE := $(NETMESSAGES_PB_SOURCE) $(CSTRIKE_PB_SOURCE)

OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SOURCE_DIR)/*.cpp)) $(patsubst $(SOURCE_DIR)/protobuf/%.pb.cc,$(BUILD_DIR)/%.pb.o,$(PROTOBUF_SOURCE))

.PHONY: all
all: prepare colorsay.so colorsay.vdf

.PHONY: list
list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | xargs

.PHONY: prepare
prepare: $(PROTOBUF_SOURCE)

colorsay.so: $(OBJECT_FILES) $(CSGO_LIBS)
	$(GCC) $(OBJECT_FILES) $(CSGO_LINKING) $(PROTOBUF_LINKING) $(OPTIONS) -shared -o colorsay.so

$(BUILD_DIR)/colorsay.o: $(addprefix $(SOURCE_DIR)/,colorsay.cpp colorsay.h recipientfilters.h protobuf/cstrike15_usermessages.pb.h) | $(BUILD_DIR)
	$(GCC) $< $(PLUGIN_FLAGS) -c -o $@

$(BUILD_DIR)/%.pb.o: $(addprefix $(SOURCE_DIR)/protobuf/,%.pb.cc %.pb.h) | $(BUILD_DIR)
	$(GCC) $< $(PROTOBUF_FLAGS) -c -o $@

$(BUILD_DIR)/%.o: $(addprefix $(SOURCE_DIR)/,%.cpp %.h) | $(BUILD_DIR)
	$(GCC) $< $(PLUGIN_FLAGS) -c -o $@

$(NETMESSAGES_PB_SOURCE): hl2sdk-csgo/public/engine/protobuf/netmessages.proto
	protoc --proto_path=$(dir $<) --proto_path=/usr/include --cpp_out=$(dir $@) $<

$(CSTRIKE_PB_SOURCE): hl2sdk-csgo/public/game/shared/csgo/protobuf/cstrike15_usermessages.proto hl2sdk-csgo/public/engine/protobuf/netmessages.proto
	protoc $(addprefix --proto_path=,$(dir $^) /usr/include) --cpp_out=$(dir $@) $<

$(PROTOBUF_SOURCE): | $(SOURCE_DIR)/protobuf

$(SOURCE_DIR)/protobuf:
	mkdir -p $(SOURCE_DIR)/protobuf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -f colorsay.so colorsay.vdf
	rm -rf $(BUILD_DIR)
	rm -rf $(SOURCE_DIR)/protobuf

CSGO_USER := steam
CSGO_GROUP := steam
INSTALL_DIR := /home/$(CSGO_USER)/csgo-ds/csgo/addons

.PHONY: install
install: $(INSTALL_DIR) $(addprefix $(INSTALL_DIR)/,colorsay.so colorsay.vdf)

$(INSTALL_DIR):
	mkdir -p $@
	chown $(CSGO_USER):$(CSGO_GROUP) $@

$(INSTALL_DIR)/colorsay.so: colorsay.so
	cp $< $@
	chown $(CSGO_USER):$(CSGO_GROUP) $@

$(INSTALL_DIR)/colorsay.vdf: colorsay.vdf
	cp colorsay.vdf $@
	chown $(CSGO_USER):$(CSGO_GROUP) $@

define VDF_CONTENT
"Plugin"
{
		"file"  "addons/colorsay"
}
endef

export VDF_CONTENT
colorsay.vdf:
	echo "$$VDF_CONTENT" > colorsay.vdf
	
