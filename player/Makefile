# A simple Makefile for compiling small SDL projects

# set the compiler flags
PACKAGES := sdl2 SDL2_image SDL2_mixer SDL2_ttf
CXXFLAGS := -ggdb -gdwarf -Og -Wall -Wextra -std=gnu++20 -Wno-unused-parameter
CXXFLAGS += `pkg-config --cflags --static $(PACKAGES)`
LDFLAGS := -ggdb -gdwarf -static -mconsole
LDFLAGS += `pkg-config --libs --static $(PACKAGES)` -lgraphite2 -lfreetype -lsharpyuv
# add header files here
HDRS :=
BUILDIR := build

# add source files here
SRCS := source/main.cpp source/draw.cpp source/titlescreen.cpp source/game.cpp source/player.cpp source/tiles.cpp source/tools.cpp source/audio.cpp source/pause.cpp source/map.cpp source/room.cpp source/screens.cpp

# generate names of object files
OBJS := $(addprefix $(BUILDIR)/, $(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.d)

# default recipe
all: output

# recipe for building the final executable
output: $(OBJS) $(HDRS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.cpp: titlescreen.hpp

$(BUILDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(DEPS) $(OBJS)

.PHONY: all clean

-include $(DEPS)
