# A simple Makefile for compiling small SDL projects

# set the compiler flags
PACKAGES := sdl2 SDL2_image SDL2_mixer SDL2_ttf
CXXFLAGS := -std=c++20 -ggdb -gdwarf -Og -Wall -D_DEBUG
CXXFLAGS += `pkg-config --cflags $(PACKAGES)`
LDFLAGS := -mconsole -ggdb -gdwarf
LDFLAGS += `pkg-config --libs $(PACKAGES)`
# add header files here
HDRS :=

# add source files here
SRCS := source/main.cpp source/editor.cpp source/draw.cpp source/tiles.cpp source/tools.cpp source/popups.cpp

# generate names of object files
OBJS := $(SRCS:.cpp=.o)

# default recipe
all: output

# recipe for building the final executable
output: $(OBJS) $(HDRS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean