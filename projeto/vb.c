#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vb.h"
#include "vbe_macros.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

static unsigned char *video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t * vmi_p) {

	phys_bytes buf;
	struct reg86u r;
	mmap_t mapa;
	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t), &mapa);
	buf = mapa.phys;
	r.u.b.ah = VBE_CALL; /* VBE get mode info */
	r.u.b.al = FUNCTION_RETURN_MODE_INFO;
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf); /* set a segment base */
	r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = INT_NU;
	if (sys_int86(&r) != OK) {
		lm_free(&mapa);
		return 1;
	}
	(*vmi_p) = (*(vbe_mode_info_t*) mapa.virtual);
	return 0;
}



int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = INT_NU;

	reg86.u.b.ah = 0x00;
	reg86.u.b.al = FUNCTION_SET_TEXT_MODE;

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init() {
	struct reg86u reg;
	reg.u.b.ah = VBE_CALL;
	reg.u.b.al = FUNCTION_SET_VBE_MODE;
	reg.u.w.bx = BIT(14) | MODE_117;
	reg.u.b.intno = INT_NU;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 0;
	}

	vbe_mode_info_t t;
	vbe_get_mode_info(MODE_117, &t);
	h_res = t.XResolution;
	v_res = t.YResolution;
	bits_per_pixel = t.BitsPerPixel;

	int r;
	struct mem_range mr;
	unsigned int vram_base = t.PhysBasePtr;
	unsigned int vram_size = (h_res * v_res * bits_per_pixel);

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	if (video_mem == MAP_FAILED)
		panic("couldn’t map video memory");
	return video_mem;
}



