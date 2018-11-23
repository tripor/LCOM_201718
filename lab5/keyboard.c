#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "i8042.h"
#include "keyboard.h"
int hook_kb=1;

//--------------------------------------Funções que ajudam o handler--------------------

//Recebe a informação do port e guarda-a na variável byte, e incrementa o counter cada vez que a função for chamada.
//Retorna 0 se tudo correr como esperado e 1 se houver algum erro.
int counter_system(unsigned long port,unsigned long *byte)
{
	if(sys_inb(port, byte)!=0)return 1;
	return 0;
}
//Obtem o code para o scan e guarda no pointer. Retorna -1 se ocorrer um erro ou 0 caso contrário.
int kbd_get_code(unsigned long *code2)
{
	unsigned long stat,code;
	if(counter_system(KBD_STATUS_REG, &stat)!=0)return -1;
	if(counter_system(KBD_OUT_BUF,&code)!=0)return -1;//guarda o scancode em code
	if ((stat &(KBD_PAR_ERR | KBD_TO_ERR)) == 0 )// ve se há algo de errado com o stat
	{
		*code2=code;
		return 0;
	}
	else return -1;
}
//--------------------------------------------------------------------------------------

//--------------------------------------Subscribe,unsubscribe and handler---------------

//Permite que os interrupts do kernal sejam recebidos.Return ao valor original de hook_kb
int kbd_subscribe(void)
{
	if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_kb)!=0)return 1;
	if(sys_irqenable(&hook_kb)!=0)return 1;
	return 1;
}
//Volta ao estado anterior ao subscribe.
int kbd_unsubscribe()
{
	if(sys_irqdisable(&hook_kb)!=0)return 1;
	if(sys_irqrmpolicy(&hook_kb)!=0)return 1;
	return 0;
}
//Função que é chamada quando há um interrupt do teclado.
int kbd_handler() {
	unsigned long code;
	if (kbd_get_code(&code) == -1)
		return -1;
	if (ESC_BREAK_CODE == code)
		return 2;
	return 0;
}
//--------------------------------------------------------------------------------------

int keyboard() {
	hook_kb=1;
	int ipc_status, r, irq_set_kbd, hook_kb_id, i = 0;
	message msg;

	unsigned long stat,code;
	hook_kb_id = kbd_subscribe(); //Faz subscribe aos interrupts do teclado.
	if (hook_kb_id != 2)
		irq_set_kbd = BIT(hook_kb_id); //por a 1 a posicao definidada por hook_kb
	else //Caso ocorra um erro.
	{
		printf("Ocorreu um erro no kbd_subscribe");
		return 1;
	}
	while (i == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive falho com: %d", r);
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_kbd) //caso seja interrupt do kbd
						{
					//Aconteceu um i do keyboard.completar
					int receber;
					receber = kbd_handler(); //chamar o handler, no primeiro caso com j=1 e caso seja um scancode de 2 bytes isto volta a ser chamado mas com j=2
					if (receber == 1 || receber == -1) //caso erro
							{
						printf("Ocorreu um erro com o handler.\n");
						return 1;
					} else if (receber == 2) //caso esc foi pressionado
							{
						printf("Pressionou o ESC.\n");
						i = 1;
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
		tickdelay(micros_to_ticks(DELAY_US));
	}
	if (kbd_unsubscribe() != 0) //Fazer unsub
			{
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
		return 1;
	}
	return 0;

}
