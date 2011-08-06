CFLAGS ?= -g -O2
LIBS = -lX11
OBJS := vdpow.o

all: vdpow
clean:
	git clean -d -X -f
vdpow: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
