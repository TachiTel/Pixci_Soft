/*
 *
 *	xclibel1.c	External	16-Feb-2023
 *
 *	Copyright (C)  2002-2023  EPIX, Inc.  All rights reserved.
 *
 *	Example program for XCLIB C Library, SCF Level functions.
 *	Example assumes Linux 'terminal' environment.
 *
 */


/*
 *  INSTRUCTIONS:
 *
 *  1)	Set 'define' options below according to the intended camera
 *	and video format.
 *
 *	For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV5B, SV5L, and SV6 frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, or PAL/YC.
 *	(The SV5A and SV5B do not support NTSC/YC or PAL/YC).
 *	For PIXCI(R) SV7 frame grabbers
 *	common choices are RS-170, NSTC, CCIR, or PAL.
 *	For PIXCI(R) SV8 frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, PAL/YC.
 *
 *	For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32, D2X, D3X, D3XE,
 *	E1, E1DB, E4, E4DB, E4G2, E4TX2, E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-PoCL,
 *	EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280, EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB, ELS2, SI, SI1, SI2, and SI4
 *	frame grabbers, use "default" to select the default format for the camera
 *	for which the PIXCI(R) frame grabber is intended.
 *	For non default formats, use XCAP to save the video set-up to a
 *	file, and set FORMATFILE to the saved file's name.
 *	For camera's with RS-232 control, note that the saved
 *	video set-up only resets the PIXCI(R) frame grabber's
 *	settings, but XCLIB does not reset the camera's settings.
 *
 *	Alternately, this could be modified to use any
 *	other convention chosen by the programmer to allow
 *	run time selection of the video format and resolution.
 *
 */

#if !defined(FORMAT) && !defined(FORMATFILE)
				  // For PIXCI(R) SV2, SV3, SV4, SV5, SV5B, SV5L, SV6
  //#define FORMAT  "RS-170"	  // RS-170 on input 2
  //#define FORMAT  "NTSC"	  // NTSC on input 2
  //#define FORMAT  "NTSC/YC"	  // NSTC S-Video on input 1		(N/A on SV5A,SV5B)
  //#define FORMAT  "CCIR"	  // CCIR on input 2
  //#define FORMAT  "PAL"	  // PAL (B,D,G,H,I) on input 2
  //#define FORMAT  "PAL/YC"	  // PAL (B,D,G,H,I) S-Video on input 1 (N/A on SV5A,SV5B)
  //#define FORMAT  "default"	  // NSTC S-Video on input 1

				  // For PIXCI(R) SV7
  //#define FORMAT  "RS-170"	  // RS-170
  //#define FORMAT  "NTSC"	  // NTSC
  //#define FORMAT  "CCIR"	  // CCIR
  //#define FORMAT  "PAL"	  // PAL
  //#define FORMAT  "default"	  // NSTC

				  // For PIXCI(R) SV8
  //#define FORMAT  "RS-170"	  // RS-170 on BNC 0
  //#define FORMAT  "NTSC"	  // NTSC on BNC 0
  //#define FORMAT  "NTSC/YC"	  // NSTC S-Video
  //#define FORMAT  "CCIR"	  // CCIR on BNC 0
  //#define FORMAT  "PAL"	  // PAL on BNC 0
  //#define FORMAT  "PAL/YC"	  // PAL (B,D,G,H,I) S-Video
  //#define FORMAT  "default"	  // NSTC on BNC 0

				  // For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32,
				  // D2X, D3X, D3XE, E1, E1DB, E4, E4DB, E4G2, E4TX2,
				  // E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-PoCL,
				  // EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280,
				  // EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB,
				  // ELS2, SI, SI1, SI2, SI4
  //#define FORMAT  "default"	  // as per board's intended camera

					  // For any PIXCI(R) frame grabber:
    #define FORMAT	"default"	  // as per board's default camera or video format
  //#define FORMATFILE	"xcvidset.fmt"	  // using format file saved by XCAP
#endif


/*
 *  2.1) Set number of expected PIXCI(R) frame grabbers.
 *  This example expects that the boards are
 *  identical and operated at the same resolution.
 *
 *  For PIXCI(R) frame grabbers with multiple, functional units,
 *  the XCLIB presents the two halves of the PIXCI(R) E1DB, E4DB, E4G2-2F, E4TX2-2F,
 *  E8CAM, E8DB, e104x4-2f, ECB2, EL1DB, ELS2, miniH2B, SI2, or SV7 frame grabbers,
 *  or the three parts of the PIXCI(R) E4G2-F2B, E4TX2-F2B, or e104x4-f2b frame grabber,
 *  or the four quarters of the PIXCI(R) E4G2-4B, E4TX2-4B, e104x4-4b, or SI4 frame grabbers,
 *  as two, three, or four independent PIXCI(R) frame grabbers, respectively.
 */
#if !defined(UNITS)
    #define UNITS	1
#endif
#define UNITSMAP    ((1<<UNITS)-1)  // shorthand - bitmap of all units


/*
 *  2.2) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  selected PIXCI(R) frame grabbers; to simplify
 *  configuration, code (below) will add the UNIT/UNITSMAP
 *  selection (above) to the driver configuration parameters.
 *
 *  Note: Under Linux, the image frame buffer memory can't be set as
 *  a run time option. It MUST be set via insmod so the memory can
 *  be reserved during Linux's initialization.
 */
#if !defined(DRIVERPARMS)
  //#define DRIVERPARMS "-QU 0"       // don't use interrupts
  //#define DRIVERPARMS "-IM 8192"    // request 8192 mbyte of frame buffer memory
    #define DRIVERPARMS ""	      // default
#endif


/*
 *  3)	Choose whether the optional PXIPL Image Processing Library
 *	is available.
 */
#if !defined(USE_PXIPL)
    #define USE_PXIPL	0
#endif


/*
 *  4) Select directory for saving of images.
 *  This example will not overwrite existing images files;
 *  so directory selection is not critical.
 */
#if !defined(IMAGEFILE_DIR)
    #define IMAGEFILE_DIR    "."
#endif


/*
 *  5a) Compile with GCC w/out PXIPL for 32 bit Linux on Intel i386 as:
 *
 *	    gcc -DC_GNU32=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/xclib_i386.a -lm
 *
 *	Compile with GCC with PXIPL for 32 bit Linux on Intel i386 as:
 *
 *	    gcc -DC_GNU32=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/pxipl_i386.a ../../lib/xclib_i386.a -lm
 *
 *	Compile with GCC w/out PXIPL for 64 bit Linux on Intel x86_64 as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/xclib_x86_64.a -lm
 *
 *	Compile with GCC with PXIPL for 64 bit Linux on Intel x86_64  as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/pxipl_x86_64.a ../../lib/xclib_x86_64.a -lm
 *
 *	Compile with GCC w/out PXIPL for 64 bit Linux on nVidia/ARM TX1/TX2/Xavier/Nano/XavierNX as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/xclib_aarch64.a -lm
 *
 *	Compile with GCC with PXIPL for 64 bit Linux on nVidia/ARM TX1/TX2/Xavier/Nano/XavierNX as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/pxipl_aarch64.a ../../lib/xclib_aarch64.a -lm
 *
 *	Compile with GCC w/out PXIPL for 32 bit Linux on nVidia/ARM TK1
 *	or Boundary Devices/ARM NITROGEN6 as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/xclib_armv7l.a -lm
 *
 *	Compile with GCC with PXIPL for 32 bit Linux on nVidia/ARM TK1
 *	or Boundary Devices/ARM NITROGEN6 as:
 *
 *	    gcc -DC_GNU64=400 -DOS_LINUX -I../../inc -no-pie xclibel1.c ../../lib/pxipl_armv7l.a ../../lib/xclib_armv7l.a -lm
 *
 *	Run as:
 *	    ./a.out
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <stdio.h>		// c library
#include <signal.h>		// c library
#include <string.h>		// c library
#include <stdlib.h>		// c library
#include <stdarg.h>		// c library
#include <unistd.h>		// c library
#include <limits.h>		// c library
#include <sys/time.h>		// c library
#include "xcliball.h"		// function prototypes
#if USE_PXIPL
  #include "pxipl.h"		// function prototypes
#endif


/*
 *  SUPPORT STUFF:
 *
 *  Catch CTRL+C and floating point exceptions so that
 *  once opened, the PIXCI(R) driver and frame grabber
 *  are always closed before exit.
 *  In most environments, this isn't critical; the operating system
 *  advises the PIXCI(R) driver that the program has terminated.
 *  In other environments, such as DOS, the operating system
 *  isn't as helpful and the driver & frame grabber would remain open.
 */
static void sigintfunc(int sig)
{
    /*
     * Some languages/environments don't allow
     * use of printf from a signal catcher.
     * Printing the message isn't important anyhow.
     *
    if (sig == SIGINT)
	printf("Break\n");
    if (sig == SIGFPE)
	printf("Float\n");
    */

    pxd_PIXCIclose();
    exit(1);
}

/*
 *  SUPPORT STUFF:
 *
 *  Slow down execution speed so
 *  each step/function can be observed.
 */
static void user(const char *mesg)
{
    if (mesg && *mesg)
	printf("%s\n", mesg);
    fprintf(stderr, "\n\nContinue (Key ENTER) ?");
    while (getchar() != '\n') ;
    fprintf(stderr, "\n");
}

/*
 * Video 'interrupt' callback function.
 */
static int fieldirqcount = 0;
static void videoirqfunc(int sig)
{
    fieldirqcount++;
}

/*
 * Open the XCLIB C Library for use.
 */
static int do_open(void)
{
    int i;

    printf("Opening EPIX(R) PIXCI(R) Frame Grabber,\n");
    printf("using configuration parameters '%s',\n", DRIVERPARMS? DRIVERPARMS: "default");

    //
    // Either FORMAT or FORMATFILE should have been
    // selected above.
    //
    #if defined(FORMAT)
	printf("and using predefined format '%s'.\n\n", FORMAT);
	i = pxd_PIXCIopen(DRIVERPARMS, FORMAT, "");
    #elif defined(FORMATFILE)
	printf("and using format file '%s'.\n\n", FORMATFILE);
	i = pxd_PIXCIopen(DRIVERPARMS, "", FORMATFILE);
    #else
	#error "Select a video format or a video format file"
    #endif
    if (i < 0) {
	printf("Open Error %s(%d)\a\a\n", pxd_mesgErrorCode(i), i);
	pxd_mesgFault(UNITSMAP);
	user("");
	return(i);
    }
    user("Open OK");
    //
    // For the sake of demonstrating optional interrupt hooks.
    //
    signal(SIGUSR1, videoirqfunc);
    if (pxd_eventCapturedFieldCreate(1, SIGUSR1, NULL) >= 0)
	user("Hooked signal upon captured field");
    //
    return(i);
}

/*
 * Report image frame buffer memory size
 */
static void do_imsize(void)
{
    printf("Frame buffer memory size       : %.3f Kbytes\n", (double)pxd_infoMemsize(UNITSMAP)/1024);
    printf("Image frame buffers (per board): %d\n", pxd_imageZdim());
    printf("Number of boards               : %d\n", pxd_infoUnits());
    user("");
}

/*
 * Report image resolution.
 */
static void do_vidsize(void)
{
    printf("Image resolution:\n");
    printf("xdim           = %d\n", pxd_imageXdim());
    printf("ydim           = %d\n", pxd_imageYdim());
    printf("colors         = %d\n", pxd_imageCdim());
    printf("bits per pixel = %d\n", pxd_imageCdim()*pxd_imageBdim());
    user("");
}


/*
 * Capture
 */
static void do_video1(void)
{
    int err;
    //
    // Capture image into buffer 1.
    //
    printf("Field count before snap=%ld\n", (long)pxd_videoFieldCount(1));
    err = pxd_doSnap(UNITSMAP, 1, 0);
    printf("pxd_doSnap: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    printf("Field count after  snap=%ld\n", (long)pxd_videoFieldCount(1));
    printf("Captured field count from IRQ hook=%d\n", fieldirqcount);
    //
    // Check for faults, such as erratic sync
    // or insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image snapped into buffer 1");
}

/*
 * General purpose output will pulse, running for several seconds.
 */
static void do_extout1(void)
{
    pxd_getGPOut(UNITSMAP, 0);
    user("Ready to watch general purpose output signal?");
}
static void do_extout2(void)
{
    int     j;

    printf("Toggling general purpose output ..\n");
    for (j = 60*3; j--; ) {
	pxd_getGPOut(UNITSMAP, 0x1);
	pxd_getGPOut(UNITSMAP, 0x0);
    }
    user("general purpose output pulses done");
}

/*
 * The general purpose input is often used as a flag to start an
 * activity, with the application code looping until
 * the general purpose input changes.
 * No signal may be present during this demo, so as to avoid
 * indefinite loops the count is simply printed twice,
 * giving the user an opportunity to toggle the input.
 * This (silently) sets the GPOut, to provide a convenient
 * signal source for testing.
 */
static void do_extin1(void)
{
    int err = 0;
    //
    // On PIXCI(R) D, D24, D32 frame grabbers the input, if available,
    // is latched, until explicitly reset.
    // This has no effect on other PIXCI(R) frame grabbers
    // in which the inputs are level sensitive.
    //
    err = pxd_setGPIn(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPIn: %s\n", pxd_mesgErrorCode(err));
    err = pxd_setGPOut(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPOut: %s\n", pxd_mesgErrorCode(err));
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}
static void do_extin2(void)
{
    int err = 0;
    err = pxd_setGPIn(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPIn: %s\n", pxd_mesgErrorCode(err));
    err = pxd_setGPOut(UNITSMAP, 0x1);
    if (err < 0)
	printf("pxd_setGPOut: %s\n", pxd_mesgErrorCode(err));
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}

/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 */
#define AOI_XDIM    3
#define AOI_YDIM    10
#define COLORS	    3
static uchar   colorimage_buf[AOI_XDIM*AOI_YDIM*COLORS];

static void color_display1(void)
{
    int     i, u;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    //
    // Transfer the color data from a selected AOI of
    // the image buffer into the PC buffer in
    // YCrCb (Intensity, Red Chroma and Blue Chroma)
    // format.	The number of bytes to be transfered
    // is 3 times the number of pixels.
    //
    for (u = 0; u < UNITS; u++) {
	if ((i = pxd_readuchar(1<<u, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "YCRCB")) != AOI_XDIM*AOI_YDIM*3) {
	    if (UNITS > 1)
		printf("Unit %d: ", u);
	    if (i < 0)
		printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	    else
		printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	    user("");
	    return;
	}
    }
    user("Image area of interest transfered for YCrCb");
}
static void color_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf(" Y=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("Cr=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("Cb=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
static void color_display3(void)
{
    int     i, u;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    //
    // Transfer the color data from a selected AOI
    // in the image buffer into the PC buffer in
    // RGB format.
    //
    for (u = 0; u < UNITS; u++) {
	if ((i = pxd_readuchar(1<<u, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "RGB")) != AOI_XDIM*AOI_YDIM*3) {
	    if (UNITS > 1)
		printf("Unit %d: ", u);
	    if (i < 0)
		printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	    else
		printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	    user("");
	    return;
	}
    }
    user("Image area of interest transfered for RGB");
}
static void color_display4(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf("R=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("G=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("B=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
static void color_display5(void)
{
    int     i, u;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    //
    // Transfer the color data from a selected AOI
    // in the the image buffer into the PC buffer in
    // BSH format.
    //
    for (u = 0; u < UNITS; u++) {
	if ((i = pxd_readuchar(1<<u, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "BSH")) != AOI_XDIM*AOI_YDIM*3) {
	    if (UNITS > 1)
		printf("Unit %d: ", u);
	    if (i < 0)
		printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	    else
		printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	    user("");
	    return;
	}
    }
    user("Image area of interest transfered for BSH");
}
static void color_display6(void)
{
    int     x, y;
    //
    // Display data from the the PC buffer.
    // The HSB values, ranging from 0 to 255, are rescaled
    // as displayed into the more typical 0 to 1 for S and B,
    // and 0 to 360 for H.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf("B=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*3+0]/255.);
	    printf("S=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*3+1]/255.);
	    printf("H=%3.0f ", colorimage_buf[(y*AOI_XDIM+x)*3+2]*360./256.);
	}
	printf("\n");
    }
    user("");
}
#undef	AOI_XDIM
#undef	AOI_YDIM
#undef	COLORS



/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 */
#define AOI_XDIM    9
#define AOI_YDIM    10
static uchar   monoimage_buf8[AOI_XDIM*AOI_YDIM];
static ushort  monoimage_buf16[AOI_XDIM*AOI_YDIM];

static void bw_display1(void)
{
    int     i, u;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    //
    // Transfer the monochrome data from a selected AOI of
    // the image buffer into the PC buffer, as 8 bit pixels.
    // Or,
    // Transfer the monochrome data from a selected AOI of
    // the image buffer into the PC buffer, as 8 to 16 bit pixels.
    //
    // The ushort array could be used for both for 8 bit pixels, but
    // users of 8 bit pixels commonly assume pixel<=>byte,
    // and is more efficient.
    //
    for (u = 0; u < UNITS; u++) {
	if (pxd_imageBdim() <= 8) {
	    if ((i = pxd_readuchar(1<<u, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, monoimage_buf8, sizeof(monoimage_buf8)/sizeof(uchar), "Grey")) != AOI_XDIM*AOI_YDIM) {
		if (UNITS > 1)
		    printf("Unit %d: ", u);
		if (i < 0)
		    printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
		else
		    printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
		user("");
		return;
	    }
	} else {
	    if (i = pxd_readushort(1<<u, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, monoimage_buf16, sizeof(monoimage_buf16)/sizeof(ushort), "Grey") != AOI_XDIM*AOI_YDIM) {
		if (UNITS > 1)
		    printf("Unit %d: ", u);
		if (i < 0)
		    printf("pxd_readushort: %s\n", pxd_mesgErrorCode(i));
		else
		    printf("pxd_readushort error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
		user("");
		return;
	    }
	}
    }
    user("Image area of interest transfered");
}
static void bw_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	printf("I = ");
	for (x = 0; x < AOI_XDIM; x++)
	    if (pxd_imageBdim() <= 8)
		printf("%4d ", monoimage_buf8[y*AOI_XDIM+x]);
	    else
		printf("%4d ", monoimage_buf16[y*AOI_XDIM+x]);
	printf("\n");
    }
    user("");
}


/*
 * Be nice and careful: For sake of this example which uses a
 * file name not selected by the user, and which might already exist,
 * don't overwrite the file if it already exists.
 * This check for a pre-existant file is the only reason
 * that fopen() need be done; this section of code
 * isn't normally needed when saving images.
 */
static int checkexist(const char *name)
{
    FILE    *fp;
    if ((fp = fopen(name, "rb")) != NULL) {
	fclose(fp);
	printf("Image not saved, file %s already exists\n", name);
	user("");
	return(1);
    }
    return(0);
}

/*
 * Save image in industry standard .tif format.
 */
static void do_savetif()
{
    int     u, err = 0;
    char    name[] = IMAGEFILE_DIR "/" "image0.tif";

    for (u = 0; u < UNITS; u++) {
	name[strlen(name)-5] = '0'+u;

	//
	// Don't overwrite existing file.
	//
	if (checkexist(name))
	    return;

	//
	// Do save of entire image in TIFF format.
	//
	if (UNITS == 1)
	    printf("Image buffer 1 being saved to file %s\n", name);
	else
	    printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	err = pxd_saveTiff(1<<u, name, 1, 0, 0, -1, -1, 0, 0);
	if (err < 0) {
	    printf("pxd_saveTiff: %s\n", pxd_mesgErrorCode(err));
	    user("");
	    return;
	}
	user("Image buffer saved");
    }
}

/*
 * Save image in the Windows .bmp format.
 * This feature is available under Linux as well.
 */
static void do_savebmp()
{
    int     u, err = 0;
    char    name[] = IMAGEFILE_DIR "/" "image0.bmp";

    for (u = 0; u < UNITS; u++) {
	name[strlen(name)-5] = '0'+u;

	//
	// Don't overwrite existing file.
	//
	if (checkexist(name))
	    return;

	//
	// Do save of entire image in Bitmap format.
	// Monochrome image buffers are saved as an 8 bit monochrome image,
	// color image buffers are saved as an 24 bit RGB color image.
	//
	if (UNITS == 1)
	    printf("Image buffer 1 being saved to file %s\n", name);
	else
	    printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	err = pxd_saveBmp(1<<u, name, 1, 0, 0, -1, -1, 0, 0);
	if (err < 0) {
	    printf("pxd_saveBmp: %s\n", pxd_mesgErrorCode(err));
	    user("");
	    return;
	}
	user("Image buffer saved");
    }
}

/*
 * Save image in the JPEG format.
 * This requires the PXIPL library.
 */
static void do_savejpeg()
{
    #if USE_PXIPL
	int	u, err = 0;
	char	name[] = IMAGEFILE_DIR "/" "image0.jpg";

	for (u = 0; u < UNITS; u++) {
	    name[strlen(name)-5] = '0'+u;

	    //
	    // Don't overwrite existing file.
	    //
	    if (checkexist(name))
		return;

	    //
	    // Do save of entire image in JPEG format.
	    //
	    if (UNITS == 1)
		printf("Image buffer 1 being saved to file %s\n", name);
	    else
		printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	    err = pxio8_jpegwrite(NULL, pxd_defineImage(u,1,0,0,-1,-1,"RGB"), NULL, name, 8, NULL);
	    if (err < 0) {
		printf("pxio8_jpegwrite: %s\n", pxd_mesgErrorCode(err));
		user("");
		return;
	    }
	    user("Image buffer saved");
	}
    #endif
}

/*
 * Display image as text characters.
 *
 * Not expected to produce a nice looking image,
 * as it is limited to a text mode display;
 * but avoids interfacing to the Linux screen manager.
 * Nor does it bother to try to do so efficiently; normally
 * the pxd_readuchar() would be called once for the entire
 * image, or perhaps once per line.
 */
static void do_display()
{
    uchar   charcodes[] = { ' ', '.', ':', '-',
			    '+', '*', 'e', 'm',
			    'I', 'H', 'B', 'M',
			  /* 0xB0, 0xB1, 0xB2, 0xDB */ };
    int     dx, dy, err = 0, u;
    #define DISPX   72
    #define DISPY   20

    for (u = 0; u < UNITS; u++) {
	if (UNITS > 1)
	    printf("Unit %d:\n", u);
	for (dy = 0; dy < DISPY; dy++) {
	    for (dx = 0; dx < DISPX; dx++) {
		uchar	value[1];
		int	ix = (dx*pxd_imageXdim())/DISPX;
		int	iy = (dy*pxd_imageYdim())/DISPY;
		err = pxd_readuchar(1<<u, 1, ix, iy, ix+1, iy+1, value, 1, "Grey");
		value[0] = (value[0]*(sizeof(charcodes)-1))/255;
		printf("%c", charcodes[value[0]]);
	    }
	    printf("\n");
	}
    }
    if (err < 0)
	printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(err));
    user("Image buffer 'displayed'");
}


/*
 * Capture sequence
 */
static void do_videosequence(void)
{
    int     err;
    //
    // Capture sequence of images into all frame buffers
    // This function returns immediately; we must wait till done.
    //
    // Unfortunately, a very slow camera might cause us to wait
    // a long time; and a very, very slow camera, or a camera
    // waiting for en external trigger, might cause us to wait forever.
    // Thus, a timeout.
    err = pxd_goLiveSeq(UNITSMAP, 1, pxd_imageZdim(), 1, pxd_imageZdim(), pxd_imageIdim());
    printf("pxd_goLiveSeq: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    if (err >= 0) {
	struct timeval timeval, timeval2;
	gettimeofday(&timeval, NULL);
	//
	printf("Capturing images ...");
	while (pxd_goneLive(UNITSMAP, 0)) {
	    printf("\r                    \r");
	    usleep(5000);
	    printf("Capturing images ...");
	    gettimeofday(&timeval2, NULL);
	    if (timeval2.tv_sec > timeval.tv_sec+30) {
		pxd_goAbortLive(UNITSMAP);
		user("Image seqeunce capture aborted after 30 seconds");
		return;
	    }
	    usleep(5000);
	}

	printf("\n");
    }
    //
    // Check for faults, such as erratic sync
    // or insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image seqeunce captured");
}

/*
 * Save sequence
 */
static void do_savesequence(void)
{
    int     u, i, err = 0;
    char    name[PATH_MAX];

    for (u = 0; u < UNITS; u++) {
	printf("Saving images ...");
	for (i = 1; i <= pxd_imageZdim(); i++) {
	    snprintf(name, sizeof(name)-1, "%s/image%d_%.6d.tif", IMAGEFILE_DIR, u, i);
	    name[sizeof(name)-1] = 0;

	    //
	    // Don't overwrite existing file.
	    //
	    if (checkexist(name))
		continue;

	    //
	    // Do save of entire image in TIFF format.
	    //
	    err = pxd_saveTiff(1<<u, name, i, 0, 0, -1, -1, 0, 0);
	    if (err < 0) {
		printf("pxd_saveTiff: %s\n", pxd_mesgErrorCode(err));
		user("");
		return;
	    }
	    printf(".");
	}
	printf("\n");
	user("Image buffers saved");
    }
}

/*
 * Close the PIXCI(R) frame grabber
 */
static void do_close(void)
{
    pxd_PIXCIclose();
    user("PIXCI(R) frame grabber closed");
}

/*
 * Here and elsewhere: we declare as 'static'
 * to avoid compiler's 'missing prototype' warnings.
 */
static void hello(void)
{
    printf("\n\n");
    printf("PIXCI(R) Frame Grabbers -  XCLIB 'C' Library\n");
    printf("XCLIBEL1.C - Example program\n");
    printf("Copyright (C)  2004-2017  EPIX, Inc.  All rights reserved.\n");
    printf("\n");
    printf("This program is best used by executing this program\n");
    printf("one step at a time, while simultaneously reading\n");
    printf("the XCLIB documentation and program listing.\n");
    user("");
}

/*
 *  MAIN:
 *
 *  Each library function is demonstrated in its own subroutine,
 *  the main calls each subroutine to produce the interactive demonstration.
 *
 *  It is suggested that this program source be read at the same time
 *  as the compiled program is executed.
 *
 */
int main(void)
{
    //
    // Say Hello
    //
    hello();

    //
    // Catch signals.
    //
    signal(SIGINT, sigintfunc);
    signal(SIGFPE, sigintfunc);

    //
    // Open and set video format.
    //
    if (do_open() < 0)
	return(1);

    //
    // Basic video operations
    // Report info, snap image, snap
    // another image.
    do_imsize();
    do_vidsize();
    do_video1();
    do_video1();

    //
    // Show pixel values
    //
    if (pxd_imageCdim() == 1) {
	bw_display1();
	bw_display2();
    } else {
	color_display1();
	color_display2();
	color_display3();
	color_display4();
	color_display5();
	color_display6();
    }

    //
    // Save image
    //
    do_savetif();
    do_savebmp();
    #if USE_PXIPL
	do_savejpeg();
    #endif

    //
    // Display image.
    //
    do_display();

    //
    // Do General Purpose Input/Output signals
    //
    do_extin1();
    do_extin2();
    do_extout1();
    do_extout2();

    //
    // Do sequence capture & save
    //
    do_videosequence();
    do_savesequence();


    //
    // All done
    //
    do_close();
    return(0);
}
