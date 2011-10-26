CFLAGS ?= -g -O2 -Wall
LIBS = -lX11 -lvdpau
OBJS := vdpow.o

all: vdpow
clean:
	git clean -d -X -f
vdpow: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
