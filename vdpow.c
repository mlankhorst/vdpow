#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <vdpau/vdpau.h>
#include <vdpau/vdpau_x11.h>

static const int output_width = 64;
static const int output_height = 64;

int main(int argc, char *argv[]) {
   Display *display;
   Window root, window;
   VdpDevice dev;
   VdpGetProcAddress *vdp_dlsym;
   VdpStatus ret;
   VdpPresentationQueueTargetCreateX11 *vdp_presentation_queue_target_create_x11;
   VdpPresentationQueueTarget target;
   
   display = XOpenDisplay(NULL);
   root = XDefaultRootWindow(display);
   window = XCreateSimpleWindow(display, root, 0, 0, output_width, output_height, 0, 0, 0);

   XSelectInput(display, window, ExposureMask | KeyPressMask);
   XMapWindow(display, window);
   XSync(display, 0);

   /* This requires libvdpau_trace, which is available in libvdpau.git */
   setenv("VDPAU_TRACE", "255", 0);

   ret = vdp_device_create_x11(display, 0, &dev, &vdp_dlsym);
   assert(ret == VDP_STATUS_OK);
   ret = vdp_dlsym(dev, VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11, (void**)&vdp_presentation_queue_target_create_x11);
   assert(ret == VDP_STATUS_OK);
   ret = vdp_presentation_queue_target_create_x11(dev, window, &target);
   assert(ret == VDP_STATUS_OK);

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

