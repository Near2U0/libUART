# libUART Main Makefile

NAME_LIBUART		= libUART
NAME_LIBUART_CXX	= libUART_CXX
NAME_LIBUART_DEMO	= libUART_Demo
NAME_LIBUART_CXX_DEMO	= libUART_CXX_Demo

SO_LIBUART		= $(NAME_LIBUART).so
SO_LIBUART_CXX		= $(NAME_LIBUART_CXX).so

SO_MAJOR_LIBUART	= $(SO_LIBUART).0
SO_MAJOR_LIBUART_CXX	= $(SO_LIBUART_CXX).0

SO_MINOR_LIBUART	= $(SO_MAJOR_LIBUART).5
SO_MINOR_LIBUART_CXX	= $(SO_MAJOR_LIBUART_CXX).1

TARGET_LIBUART		= $(SO_MINOR_LIBUART)
TARGET_LIBUART_CXX	= $(SO_MINOR_LIBUART_CXX)
TARGET_LIBUART_DEMO	= $(NAME_LIBUART_DEMO)
TARGET_LIBUART_CXX_DEMO	= $(NAME_LIBUART_CXX_DEMO)

RM			= rm -rf
LN			= ln -frs
CC			= gcc
CXX			= g++
INSTALL			= install

CFLAGS			+= -Wall
CFLAGS			+= -Wextra
CFLAGS_LIB		+= -fPIC

CXXFLAGS		+= -std=c++11
CXXFLAGS		+= -Wall
CXXFLAGS		+= -Wextra
CXXFLAGS_LIB		+= -fPIC

LDFLAGS_LIBUART		+= -shared
LDFLAGS_LIBUART		+= -Wl,-soname,$(SO_LIBUART)

LDFLAGS_LIBUART_CXX	+= -shared
LDFLAGS_LIBUART_CXX	+= -Wl,-soname,$(SO_LIBUART_CXX)

include ./src/files.mk

OBJ_LIBUART		= $(SRC_LIBUART:.c=.o)
OBJ_LIBUART_DEMO	= $(SRC_LIBUART_DEMO:.c=.o)
OBJ_LIBUART_CXX		= $(SRC_LIBUART_CXX:.cpp=.o)
OBJ_LIBUART_CXX_DEMO	= $(SRC_LIBUART_CXX_DEMO:.cpp=.o)

# Default install directory
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

# Buil all
all: libUART

# Build libUART C Library
libUART: $(OBJ_LIBUART)
	@echo "[LD] $(TARGET_LIBUART)"
	@$(CC) $(LDFLAGS_LIBUART) -o $(TARGET_LIBUART)  $(OBJ_LIBUART)

src/libUART/%.o: src/libUART/%.c
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) $(CFLAGS_LIB) -c -o $@ $<

# Build libUART C Demo Application
libUART_Demo: $(OBJ_LIBUART_DEMO)
	@echo "[LD] $(TARGET_LIBUART_DEMO)"
	@$(CC) -o $(TARGET_LIBUART)  $(OBJ_LIBUART_DEMO)

src/libUART_Demo/%.o: src/libUART_Demo/%.c
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Build libUART C++ Library
libUART_CXX: $(OBJ_LIBUART_CXX)
	@echo "[LD] $(TARGET_LIBUART_CXX)"
	@$(CXX) $(LDFLAGS_LIBUART_CXX) -o $(TARGET_LIBUART_CXX) $(OBJ_LIBUART_CXX)

src/libUART_CXX/%.o: src/libUART_CXX/%.cpp
	@echo "[CXX] $<"
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS_LIB) -c -o $@ $<

# Build libUART C++ Demo Application
libUART_CXX_Demo: $(OBJ_LIBUART_CXX_DEMO)
	@echo "[LD] $(TARGET_LIBUART_CXX_DEMO)"
	@$(CXX) -o $(TARGET_LIBUART_CXX_DEMO) $(OBJ_LIBUART_CXX_DEMO)

src/libUART_CXX_Demo/%.o: src/libUART_CXX_Demo/%.cpp
	@echo "[CC] $<"
	@$(CXX) $(CFLAGS) -c -o $@ $<

# Clean
.PHONY: clean
clean:
	@echo "[CLEAN]"
	@$(RM) $(TARGET_LIBUART) $(OBJ_LIBUART) *~
