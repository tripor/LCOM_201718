#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "i8042.h"
#include "test3.h"
static int hook=0,counter=0,timer_counter=0,hook_timer=1;
unsigned long handler_asm();
int counter_system(unsigned long port,unsigned long *byte);
//--------------------------------------POLL--------------------------------------------

//Escreve cmd para o keyboard comand register.
int kbd_write_cmd(unsigned long cmd)
{
	unsigned long stat;
	while(1) //Repete-se enquanto não se escrever no comand register.
	{
		counter_system(KBD_STATUS_REG , &stat);
		if((stat & KBD_IBF) == 0 ) {
			sys_outb(KBC_CMD_REG, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}
//Devolve o command byte para futura escrita no keyboard no poll
unsigned long kbd_read_cmd()
{
	unsigned long cmd;
	kbd_write_cmd(KBC_READ_CB);
	counter_system(KBD_OUT_BUF,&cmd);
	return cmd;
}
//Manda o codigo 0x47 para o command byte do keyboard e faz disable. Similar ao programa dado para esta alinea
int kbd_disable_poll(void)
{
	unsigned long byte;
	byte=GET_CMD & kbd_read_cmd();
	kbd_write_cmd(KBC_WRITE_CB);
	if(sys_outb(KBC_CMD_REG_ARGS,byte | ENA_IN_MOUSE)!=0)return 1;
	return 0;
}
//Manda o codigo que faz enable ao teclado na função poll
int kbd_enable_poll(void)
{
	unsigned long byte;
	byte=GET_CMD & kbd_read_cmd();
	kbd_write_cmd(KBC_WRITE_CB);
	if(sys_outb(KBC_CMD_REG_ARGS,byte | ENA_IN_MOUSE |ENA_IN_KBD)!=0)return 1;
	return 0;
}
//Imprime o scancode para a função poll. Retorna 2 se for pressionado esc e 0 caso contrario
int kbd_handler_poll(unsigned long code)
{
	unsigned long code2,stat;
	counter_system(KBD_STATUS_REG, &stat);
	if( stat & KBD_OBF ) //Se o output buffer estiver enabled, imprimem-se scancodes de 2 bytes.
	{
		counter_system(KBD_OUT_BUF, &code2);//ler segundo scancode.
		if(BREAK_CODE & code2)
		{
			printf("BREAKCODE: 0x%x 0x%x\n",code,code2);
		}
		else
		{
		 	printf("MAKECODE: 0x%x 0x%x\n",code,code2);
		}
	}
	else//Imprime os scancodes de 1 só byte.
	{
		if(BREAK_CODE & code)
		{
			printf("BREAKCODE: 0x%x\n",code);
		}
		else
		{
		 	printf("MAKECODE: 0x%x\n",code);
		}
		if(ESC_BREAK_CODE == code)return 2;//Quando o esc for largado.
	}
	return 0;
}
//--------------------------------------------------------------------------------------

//--------------------------------------Funções que ajudam o handler--------------------

//Recebe a informação do port e guarda-a na variável byte, e incrementa o counter cada vez que a função for chamada.
//Retorna 0 se tudo correr como esperado e 1 se houver algum erro.
int counter_system(unsigned long port,unsigned long *byte)
{
	if(sys_inb(port, byte)!=0)return 1;
	counter++;
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
//Se j for 1 printf de scancode de 1 byte else printf de scancode de 2 bytes
int display_key(unsigned long *code,unsigned long *code2,int *j)
{
	if(*j == 1)
	{
		if(BREAK_CODE & *code)
		{
			printf("BREAKCODE: 0x%x\n",*code);
		}
		else
		{
		 	printf("MAKECODE: 0x%x\n",*code);
		}
		if(ESC_BREAK_CODE == *code)return 2;
	}
	else if(*j==2)
	{
		if(BREAK_CODE & *code2)
		{
			printf("BREAKCODE: 0x%x 0x%x\n",*code,*code2);
		}
		else
		{
		 	printf("MAKECODE: 0x%x 0x%x\n",*code,*code2);
		}

		*j=1;
	}

	return 0;
}
//--------------------------------------------------------------------------------------

//--------------------------------------Subscribe,unsubscribe and handler---------------

//Permite que os interrupts do kernal sejam recebidos.Return ao valor original de hook
int kbd_subscribe(void)
{
	if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook)!=0)return 1;
	if(sys_irqenable(&hook)!=0)return 1;
	return 0;
}
//Volta ao estado anterior ao subscribe.
int kbd_unsubscribe()
{
	if(sys_irqdisable(&hook)!=0)return 1;
	if(sys_irqrmpolicy(&hook)!=0)return 1;
	return 0;
}
//Função que é chamada quando há um interrupt do teclado.
int kbd_handler(unsigned long *code,unsigned long *code2,int *j)
{
	unsigned long stat;
	if(*j==2)//Se j == 2 lê a segunda parte do scancode e imprime.
	{
		if(kbd_get_code(code2) == -1) return -1;
		return display_key(code,code2,j);
	}
	else//Se j for 1 vai verificar se o output buffer foi esvaziado e imprime. Se não j = 2 e acaba.
	{
		if(kbd_get_code(code) == -1) return -1;
		counter_system(KBD_STATUS_REG, &stat);
		if(stat & KBD_OBF)
		{
			*j=2;
			return 0;
		}
		else *j=1;
		return display_key(code,code2,j);
	}
}
//Subscrever aos interrupts do timer.Return ao valor original do hook_timer
int timer_subscribe_int(void) {
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_timer)!=0)return -1;
	if(sys_irqenable(&hook_timer)!=0)return -1;
	return 1;//valor inicial do hook que corresponde a bit order in interrupt mask;
}
//Unsubscrive aos interrupts do timer.
int timer_unsubscribe_int() {
	if(sys_irqdisable(&hook_timer)!=0)return 1;
	return 0;
}
//Incrementador do timer_counter sempre que há um interrupt do timer.
void timer_int_handler() {
	timer_counter++;
}

//--------------------------------------------------------------------------------------

int kbd_test_scan(unsigned short assembly) {
	int ipc_status,r,irq_set_kbd,hook_id,i=0,j=1;
	unsigned long code=0,code2=0,test;
	message msg;
	hook_id=kbd_subscribe();//Faz subscribe aos interrupts do teclado.
	if(hook_id != 1 ) irq_set_kbd=BIT(hook_id);//por a 1 a posicao definidada por hook
	else//Caso ocorra um erro.
	{
		printf("Ocorreu um erro no kbd_subscribe");
		return 1;
	}
	while(i==0)
	{
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive falho com: %d", r);
		}
		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
				case HARDWARE:
					if(msg.NOTIFY_ARG & irq_set_kbd) //caso seja interrupt do kbd
					{
						if(assembly==0)//caso seja em c
						{
							//Aconteceu um i do keyboard.completar
							int receber;
							receber=kbd_handler(&code,&code2,&j);//chamar o handler, no primeiro caso com j=1 e caso seja um scancode de 2 bytes isto volta a ser chamado mas com j=2
							if(receber==1 || receber == -1)//caso erro
							{
									printf("Ocorreu um erro com o handler.\n");
									return 1;
							}
							else if(receber==2)//caso esc foi pressionado
							{
								printf("Pressionou o ESC, o scan vai parar.\n");
								i=1;
							}
						}
						else//caso seja em assembly
						{
							test = handler_asm(&code,&code2,&j); //chamar assembly e guarda os codes em pointers
							if (test==-1)
							{
								printf("Ocorreu um erro.\n");
								return 1;
							}
							int receber;
							receber=display_key(&code,&code2,&j);//não faz display se for j==3
							if(receber==1 || receber == -1)
							{
								printf("Ocorreu um erro com o handler.\n");
								return 1;
							}
							else if(receber==2)
							{
								printf("Pressionou o ESC, o scan vai parar.\n");
								i=1;
							}
						}

					}
					break;
				default: 	//não faz nada
					break;
			}
		}
		else
		{
					//não faz nada
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	if(kbd_unsubscribe()!=0) //Fazer unsub
	{
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
		return 1;
	}

	if(assembly==0)printf("Numero de vezes que sys_inb foi chamado: %d\n",counter);
	return 0;

}

int kbd_test_poll() {
	unsigned long stat,code;
	int i=0;
	/*if(kbd_disable_poll()!=0)
	{
	    printf("Ocorreu ao fazer disable ao teclado\n");
	    return 1;
	}*/
    while(i==0)
    {
    	counter_system(KBD_STATUS_REG, &stat);
    	if( stat & KBD_OBF )//Se houver scancode para ler
    	{
    		counter_system(KBD_OUT_BUF, &code);
    		if((stat &(KBD_PAR_ERR | KBD_TO_ERR)) == 0 ) //caso não ocorra erros
    		{
    			i=kbd_handler_poll(code);//chama o handler para o poll
    		}
    		else return 1;
    	}
    	tickdelay(micros_to_ticks(DELAY_US));
    }
    printf("Foi pressionado o ESC. O polling vai parar\n");
    if(kbd_enable_poll()!=0)//fazer enable ao teclado
    {
        printf("Ocorreu ao fazer enable ao teclado\n");
        return 1;
    }
    printf("Numero de vezes que o sys_inb foi chamado: %d",counter);
	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	int ipc_status,r,irq_set_kbd,irq_set_timer,hook_id,hook_id_timer;
	int i=0,j=1;
	unsigned long code=0,code2=0;
	message msg;
	hook_id_timer=timer_subscribe_int();//sub ao timer
	hook_id=kbd_subscribe();//faz sub ao teclado
	if(hook_id != 1 && hook_id_timer != -1)
	{
		irq_set_kbd=BIT(hook_id);
		irq_set_timer=BIT(hook_id_timer);
	}
	else
	{
		printf("Ocorreu um erro no subscribe");
		return 1;
	}
	while(i==0 && (n*60)>=timer_counter)
	{
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive falho com: %d", r);
		}
		if(is_ipc_notify(ipc_status))
		{
			switch(_ENDPOINT_P(msg.m_source))
			{
				case HARDWARE:
					if(msg.NOTIFY_ARG & irq_set_kbd)//Se for interrupt do teclado
					{
						int receber;
						timer_counter=0;
						receber=kbd_handler(&code,&code2,&j);
						if(receber==1)
						{
							printf("Ocorreu um erro com o handler.\n");
							return 1;
						}
						else if(receber==2)
						{
							printf("Pressionou o ESC, o scan vai parar.\n");
							i=1;
						}
						else if(receber == -1) return 1;
						tickdelay(micros_to_ticks(DELAY_US));
					}
					else if(msg.NOTIFY_ARG & irq_set_timer) //Interrupt do timer
					{
						timer_int_handler();
					}
					break;
				default: 	//não faz nada
					break;
			}
		}
		else
		{
					//não faz nada
		}
	}
	if((n*60)<=timer_counter) printf("Passaram %d segundos desde a ultima vez que carregou numa tecla, a leitura acabou.\n",n);
	if(kbd_unsubscribe()!=0)
	{
		printf("Ocorreu um erro no kbd_unsubscribe.\n");
		return 1;
	}
	return 0;
}
