#pragma once

#include <types.h>

#define FBIOGET_VSCREENINFO     0x4600
#define FBIOPUT_VSCREENINFO     0x4601
#define FBIOGET_FSCREENINFO     0x4602

typedef struct linux_fb_bitfield {
  u32 offset;                   // beginning of bitfield
  u32 length;                   // length of bitfield
  u32 msb_right;                // != 0 : Most significant bit is right
} fb_bitfield;

typedef struct linux_fb_var_screeninfo {
  u32 xres;                     // visible resolution
  u32 yres;
  u32 xres_virtual;             // virtual resolution
  u32 yres_virtual;
  u32 xoffset;                  // offset from virtual to visible
  u32 yoffset;                  // resolution
  u32 bits_per_pixel;           // guess what
  u32 grayscale;                // 0 = color, 1 = grayscale, >1 = FOURCC
  fb_bitfield red;              // bitfield in fb mem if true color,
  fb_bitfield green;            // else only length is significant
  fb_bitfield blue;
  fb_bitfield transp;           // transparency
  u32 nonstd;                   // != 0 Non standard pixel format
  u32 activate;                 // see FB_ACTIVATE_*
  u32 height;                   // height of picture in mm
  u32 width;                    // width of picture in mm
  u32 accel_flags;              // (OBSOLETE) see fb_info.flags
  // Timing: All values in pixclocks, except pixclock (of course)
  u32 pixclock;                 // pixel clock in ps (pico seconds)
  u32 left_margin;              // time from sync to picture
  u32 right_margin;             // time from picture to sync
  u32 upper_margin;             // time from sync to picture
  u32 lower_margin;
  u32 hsync_len;                // length of horizontal sync
  u32 vsync_len;                // length of vertical sync
  u32 sync;                     // see FB_SYNC_*
  u32 vmode;                    // see FB_VMODE_*
  u32 rotate;                   // angle we rotate counter clockwise
  u32 colorspace;               // colorspace for FOURCC-based modes
  u32 reserved[4];              // Reserved for future compatibility
} fb_var_screeninfo;

typedef struct linux_fb_fix_screeninfo {
  char id[16];                    // identification string eg "TT Builtin"
  u64 smem_start;                 // Start of frame buffer mem (physical address)
  u32 smem_len;                   // Length of frame buffer mem
  u32 type;                       // see FB_TYPE_*
  u32 type_aux;                   // Interleave for interleaved Planes
  u32 visual;                     // see FB_VISUAL_*
  u16 xpanstep;                   // zero if no hardware panning
  u16 ypanstep;                   // zero if no hardware panning
  u16 ywrapstep;                  // zero if no hardware ywrap
  u32 line_length;                // length of a line in bytes
  u64 mmio_start;                 // Start of Memory Mapped I/O (physical address)
  u32 mmio_len;                   // Length of Memory Mapped I/O
  u32 accel;                      // Indicate to driver which specific chip/card we have
  u16 capabilities;               // see FB_CAP_*
  u16 reserved[2];                // Reserved for future compatibility
} fb_fix_screeninfo;
