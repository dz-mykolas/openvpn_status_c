BIN:=openvpn_status
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
CPPFLAGS:=
LDFLAGS:=

.PHONY: all clean $(BIN)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
clean:
	$(RM) $(OBJ) $(BIN)