#include <stdio.h>
#include <sys/time.h>
#include <X11/Xlib.h>

static const int output_width = 64;
static const int output_height = 64;

int main(int argc, char *argv[]) {
   Display *display;
   Window root, window;
   
   display = XOpenDisplay(NULL);
   root = XDefaultRootWindow(display);
   window = XCreateSimpleWindow(display, root, 0, 0, output_width, output_height, 0, 0, 0);
}