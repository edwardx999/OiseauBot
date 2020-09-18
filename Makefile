RELEASE ?= 0
ifeq ($(RELEASE), 1)
	FLAGS = -std=c++17 -O2
	INTERMEDIATE = release
	MAIN = DiscordBot
else
	FLAGS = -g -std=c++17 -Og
	INTERMEDIATE = debug
	MAIN = DiscordBot_debug
endif

CC = g++-8
LINKER = -lstdc++ -lstdc++fs -lpthread -lz -lssl -lcrypto
INCLUDE = -Iinclude
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(INTERMEDIATE)/%.o, $(SRCS))
PRECOMP = pch.h
PRECOMP_OUT = $(INTERMEDIATE)/$(PRECOMP).gch

$(MAIN): $(OBJS)
	$(CC) $(INTERMEDIATE)/*.o -o $@ $(LINKER)

$(INTERMEDIATE):
	mkdir $@

$(INTERMEDIATE)/%.o: %.cpp $(PRECOMP_OUT) | $(INTERMEDIATE)
	$(CC) $< -c $(FLAGS) $(INCLUDE) -include$(INTERMEDIATE)/$(PRECOMP) -o $@

$(PRECOMP_OUT): $(PRECOMP)
	$(CC) $< $(FLAGS) $(INCLUDE) -o $@

semiclean:
	(rm $(INTERMEDIATE)/* || true)

clean: semiclean
	(rm $(PRECOMP_OUT) || true)