# Makefile for simple Raylib 3D project
# Template

# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99

# Source and output
SRC = main.c
OUT = game

# Libraries
LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Build rule
all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

# Clean rule
clean:
	rm -f $(OUT)

