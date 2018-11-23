#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "macros.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t * vmi_p) {

	phys_bytes buf;
	struct reg86u r;
	mmap_t mapa;
	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t), &mapa);
	buf=mapa.phys;
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
	(*vmi_p)=(*(vbe_mode_info_t*)mapa.virtual);
	return 0;
}


