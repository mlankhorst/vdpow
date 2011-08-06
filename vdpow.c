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

   XSelectInput(display, window, ExposureMask | KeyPressMask);
   XMapWindow(display, window);
   XSync(display, 0);

   while (1) {
      XEvent ev;
      XNextEvent(display, &ev);
      switch (ev.type) {
         case Expose: /* Draw */ break;
         case KeyPress: goto out;
      }
   }

out:
   XDestroyWindow(display, window);
   XCloseDisplay(display);
   return 0;
}

