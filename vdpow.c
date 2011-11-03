#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <vdpau/vdpau.h>
#include <vdpau/vdpau_x11.h>
#include "vdpow.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))

#include "swan.h"

VdpDevice dev;

int main(int argc, char *argv[]) {
   Display *display;
   Window root, window;
   VdpStatus ret;
   VdpPresentationQueueTarget target;
   VdpDecoder dec;
   VdpVideoSurface surf[8];
   VdpOutputSurface osurf[8];
   VdpVideoMixer mixer;
   VdpProcamp camp = { VDP_PROCAMP_VERSION, 0., 1., 1., 0. };
   VdpCSCMatrix mat;

   int i;

   VdpVideoMixerFeature mixer_features[] = {
   };
   VdpVideoMixerParameter mixer_parameters[] = {
      VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_WIDTH,
      VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_HEIGHT
   };
   const void *mixer_values[ARRAY_SIZE(mixer_parameters)] = {
      &input_width,
      &input_height
   };

   display = XOpenDisplay(NULL);
   root = XDefaultRootWindow(display);
   window = XCreateSimpleWindow(display, root, 0, 0, output_width, output_height, 0, 0, 0);
   XSelectInput(display, window, ExposureMask | KeyPressMask);
   XMapWindow(display, window);
   XSync(display, 0);

   /* This requires libvdpau_trace, which is available in libvdpau.git */
   setenv("VDPAU_TRACE", "255", 0);

   ret = vdp_device_create_x11(display, 0, &dev, &vdp_get_proc_address);
   assert(ret == VDP_STATUS_OK);

   load_vdpau(dev);
   vdp_generate_csc_matrix(&camp, VDP_COLOR_STANDARD_SMPTE_240M, &mat);

#define ok(a) do { ret = a; if (ret != VDP_STATUS_OK) { fprintf(stderr, "%s\n", vdp_get_error_string(ret)); return 1; } } while (0)

   ok(vdp_presentation_queue_target_create_x11(dev, window, &target));
   for (i = 0; i < ARRAY_SIZE(surf); ++i) {
      ok(vdp_video_surface_create(dev, VDP_CHROMA_TYPE_420, input_width, input_height, &surf[i]));
      ok(vdp_output_surface_create(dev, VDP_COLOR_TABLE_FORMAT_B8G8R8X8, output_width, output_height, &osurf[i]));
   }
   ok(vdp_video_mixer_create(dev, ARRAY_SIZE(mixer_features), mixer_features,
                             ARRAY_SIZE(mixer_parameters), mixer_parameters, mixer_values, &mixer));
   ok(vdp_decoder_create(dev, VDP_DECODER_PROFILE_MPEG1, input_width, input_height, 2, &dec));
   vdp_decoder_destroy(dec);
   ok(vdp_decoder_create(dev, VDP_DECODER_PROFILE_MPEG2_MAIN, input_width, input_height, 2, &dec));

   {
      VdpBitstreamBuffer buffer = {
         .struct_version = VDP_BITSTREAM_BUFFER_VERSION,
         .bitstream = swan,
         .bitstream_bytes = sizeof(swan)
      };
      vdp_decoder_render(dec, surf[0], &info, 1, &buffer);
   }
   while (1) {
      XEvent ev;
      XNextEvent(display, &ev);
      switch (ev.type) {
         case Expose: /* Draw */ break;
         case KeyPress: goto out;
      }
   }

out:
   vdp_decoder_destroy(dec);
   vdp_video_mixer_destroy(mixer);
   for (i = ARRAY_SIZE(surf); i > 0; i--) {
      vdp_output_surface_destroy(osurf[i - 1]);
      vdp_video_surface_destroy(surf[i - 1]);
   }
   vdp_presentation_queue_target_destroy(target);
   vdp_device_destroy(dev);
   XDestroyWindow(display, window);
   XCloseDisplay(display);
   return 0;
}

