RM 	= rm -rf
LN	= ln -frs
CC 	= gcc
INSTALL	= install

NAME	= libUART
TARGET 	= $(NAME).so.0.5
CFLAGS 	= -Wall -fPIC
LDFLAGS = -shared -Wl,-soname,$(TARGET)

SRC += unix/error.c
SRC += unix/uart.c
SRC += main.c
SRC += util.c

OBJ = $(SRC:.c=.o)

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: main

debug: CFLAGS += -g

debug: main

main: $(OBJ)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJ)
	$(LN) $(TARGET) $(NAME).so.0
	$(LN) $(NAME).so.0 $(NAME).so

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/lib64/
	$(INSTALL) -m 644 $(NAME).so $(DESTDIR)$(PREFIX)/lib64/
	$(INSTALL) -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib64/
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/include/
	$(INSTALL) -m 644 $(NAME).h $(DESTDIR)$(PREFIX)/include/

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJ) *~
