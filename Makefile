DEBUG ?= 1
FLAGS = -g -std=c++17
CC := g++-8
LINKER = -lstdc++ -lstdc++fs -lpthread -lz -lssl -lcrypto
INCLUDE = -Iinclude
INTERMEDIATE = debug
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(INTERMEDIATE)/%.o, $(SRCS))
PRECOMP = pch.h
PRECOMP_OUT = $(PRECOMP).gch
MAIN = DiscordBot

$(MAIN): $(OBJS)
	$(CC) $(INTERMEDIATE)/*.o -o $@ $(LINKER)

$(INTERMEDIATE):
	mkdir $@

$(INTERMEDIATE)/%.o: %.cpp $(PRECOMP_OUT) | $(INTERMEDIATE)
	$(CC) $< -c $(FLAGS) $(INCLUDE) -o $@

$(PRECOMP_OUT): $(PRECOMP)
	$(CC) $< $(FLAGS) $(INCLUDE) -o $@

semiclean:
	rm $(INTERMEDIATE)/*

clean: semiclean
	rm $(PRECOMP_OUT)