#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "video_gr.h"
#include "read_xpm.h"
#include "vbe.h"
#include "keyboard.h"
#include "macros.h"
#include "i8254.h"
#include "i8042.h"
#include "video_test.h"
//#include "timer.h"

static int hook=0;
void clear_screen(unsigned char *ptr)
{
	int i;
	for(i=0;i< H_RES_105*V_RES_105;i++,ptr++)
	{
		*ptr=0;
	}
}
void desenhar_xpm(unsigned char *ptr,char *xpm[],int x,int y)
{
	int tx,ty,nobit=0,posx=0,posy=0;
	char *bitmap=malloc(sizeof(char));
	bitmap=read_xpm(xpm,&tx,&ty);
	unsigned char *inicio=ptr + x + y*(H_RES_105) ;
	for(posx=0; posx < tx || posy < ty ; posx++,ptr++)
	{
		if(posx>=tx)
		{
			posy++;
			ptr=inicio+H_RES_105*posy;
			posx=0;
		}
		*ptr=*(bitmap+nobit);
		nobit++;
	}
}
void putpixel(int x, int y,unsigned char *ptr,unsigned long color)
{
	ptr+= x + y*(H_RES_105) ;
	*ptr=color;
}
//https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void line(int x0, int y0, int x1, int y1,unsigned char *ptr,unsigned long color) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    putpixel(x0,y0,ptr,color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}




int timer_subscribe_int(void) {
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook)!=0)return -1;
	if(sys_irqenable(&hook)!=0)return -1;
	return 0;//valor inicial do hook que corresponde a bit order in interrupt mask;
}

int timer_unsubscribe_int() {
	if(sys_irqdisable(&hook)!=0)return 1;
	return 0;
}




void *video_test_init(unsigned short mode, unsigned short delay) {
	char *video_mem;
	video_mem=vg_init(mode);
	tickdelay(micros_to_ticks(delay*1000000));
	vg_exit();
	return video_mem;
}

int video_test_square(unsigned short x, unsigned short y, unsigned short size,unsigned long color) {
	void *video_mem;
	video_mem = vg_init(MODE_105);
	int i;
	int xx = 0, yy = 0,erro;
	erro=size/2;
	if(size%2!=0)erro--;
	unsigned char *ptr;
	ptr = video_mem;
	for (i = 0; i < (1024 * 768); i++, xx++, ptr++) {
		if (xx >= 1024) {
			xx = 0;
			yy++;
		}
		if (xx > (x-erro) + (1024 / 2) && yy > (y-erro) + (768 / 2) && xx <= (size-erro) + (1024 / 2)
				&& yy <= (size-erro) + (768 / 2)) {
			ptr=draw_line_horizontal(size,ptr,color);
			xx+=size;
		}
	}
	video_dump_fb();
	keyboard();
	vg_exit();
	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	void *video_mem;
	video_mem = vg_init(MODE_105);
	unsigned char *ptr;
	ptr = video_mem;

	int x1 = xi, y1 = yi, x2 = xf, y2 = yf;

	line(x1,y1,x2,y2,ptr,color);
	video_dump_fb();
	keyboard();
	vg_exit();
	return 0;
}

int video_test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	void *video_mem;
	video_mem = vg_init(MODE_105);
	desenhar_xpm(video_mem,xpm,xi,yi);
	video_dump_fb();
	keyboard();
	vg_exit();
	return 0;

}

int video_test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	void *video_mem;
	video_mem = vg_init(MODE_105);

	unsigned char *ptr;
	ptr=video_mem;

	int x0 = xi, y0 = yi, x1 = xf, y1 = yf;

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	int ipc_status, r, irq_set, obter,i=1;
	message msg;
	obter = timer_subscribe_int();
	irq_set = BIT(obter);
	int counter = 0;

	int irq_set_kbd, hook_kb_id;

	unsigned long stat, code;
	hook_kb_id = kbd_subscribe(); //Faz subscribe aos interrupts do teclado.
	irq_set_kbd = BIT(hook_kb_id);



	while (i>=1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if ((msg.NOTIFY_ARG & irq_set) && i==1) {
					counter++;
					if(counter==121) counter=1;
					int k=0;
					if ((counter % (60/f)) == 0) {
						for (; k <= s; k++) {
							if (x0 == x1 && y0 == y1) {
								clear_screen(video_mem);
								desenhar_xpm(ptr, xpm, x0, y0);
								i = 2;
								break;
							}
							e2 = err;
							if (e2 > -dx) {
								err -= dy;
								x0 += sx;
							}
							if (e2 < dy) {
								err += dx;
								y0 += sy;
							}
						}
						clear_screen(video_mem);
						desenhar_xpm(ptr,xpm,x0, y0);
					}
				}
				if (msg.NOTIFY_ARG & irq_set_kbd)
				{
					int receber;
					receber = kbd_handler(); //chamar o handler, no primeiro caso com j=1 e caso seja um scancode de 2 bytes isto volta a ser chamado mas com j=2
					if (receber == 1 || receber == -1) //caso erro
							{
						printf("Ocorreu um erro com o handler.\n");
						return 1;
					} else if (receber == 2) //caso esc foi pressionado
							{
						printf("Pressionou o ESC.\n");
						i = 0;
						counter_system(KBD_STATUS_REG, &stat);
						while (stat & KBD_OBF) {
							if (kbd_get_code(&code) == -1)
								return -1;
							counter_system(KBD_STATUS_REG, &stat);
						}
					}
				}
				break;
			default: 	//não faz nada
				break;
			}
		} else {
			//não faz nada
		}

	}
	video_dump_fb();
	timer_unsubscribe_int();
	kbd_unsubscribe();
	vg_exit();
	return 0;
}

int video_test_controller() {

	/* To be completed */
	return 0;

}

