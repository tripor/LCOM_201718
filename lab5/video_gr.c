#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "macros.h"

/* Constants for VBE 0x105 mode */

/* Private global variables */

static void *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *draw_line_horizontal(int size, unsigned char *ptr,unsigned long color) {
	int i;
	for (i = 0; i < size; i++,ptr++) {
		*ptr=color;
	}
	return ptr;
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void *vg_init(unsigned short mode) {
	struct reg86u reg;
	reg.u.b.ah = VBE_CALL;
	reg.u.b.al = FUNCTION_SET_VBE_MODE;
	reg.u.w.bx = BIT(14) | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = INT_NU;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 0;
	}

	vbe_mode_info_t t;
	vbe_get_mode_info(mode,&t);
	h_res=t.XResolution;
	v_res=t.YResolution;
	bits_per_pixel=t.BitsPerPixel;

	int r;
	struct mem_range mr;
	unsigned int vram_base=t.PhysBasePtr; /* VRAM’s physical addresss */
	unsigned int vram_size=(h_res*v_res*bits_per_pixel); /* VRAM’s size, but you can use the frame-buffer size, instead */
	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	if (video_mem == MAP_FAILED)
		panic("couldn’t map video memory");
	return video_mem;
}
