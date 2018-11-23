#include "i8042.h"

.data

.text

.global  _handler_asm,_stat_reg,_get_code_mouse

_handler_asm:   //esp recebe parametros, 4(%esp)tem o primeiro elemento
	xor %eax,%eax
	movl 4(%esp),%edx//guarda o 1º parametro, code
	movl 8(%esp),%esi//guarda o 2º parametro, code2
	movl 12(%esp),%ecx//guarda o 3º parametro, j
	movl (%ecx),%ebx//tira o valor de j do pointer
	cmp  $1,%ebx//comparar o valor de j com 1
	je um //se j for 1 vai para "um"
	// Se for um scancode de 2 bytes, j==3
	inb $KBD_STATUS_REG//vai buscar o stat do status reg
	movl %eax,%ebx// guarda este stat em ebx
	inb $KBD_OUT_BUF// vai buscar o scancode
	andl $(KBD_PAR_ERR | KBD_TO_ERR), %ebx // vai comparar o stat com os erros
	cmp $0x00,%ebx//compara ebx com 0x00
	jne error//caso aconteca um erro vai para "error"
	movl %eax,(%esi) //vai guarda o segundo code em *code2
	movl $2,(%ecx)//define j para 2
	jmp fim//vai para o fim
um:
	//Caso se um scancode de 1 byte
	inb $KBD_STATUS_REG//obtem o stat do status reg
	movl %eax,%ebx//guarda o stat em ebx
	inb $KBD_OUT_BUF//obtem o scancode
	andl $(KBD_PAR_ERR | KBD_TO_ERR), %ebx//and de stat com os erros
	cmp $0x00,%ebx//compara ebx com 0x00
	jne error//caso aconteça um erro vai para "error"
	movl %eax,(%edx)//senão guarda o code em *code
	//Vou verificar se nao há mais code para ler
	inb $KBD_STATUS_REG//ler stat de status reg
	andl $KBD_OBF, %eax//and do stat com output buffer full
	cmp $0x00,%eax//comparar eax com 0x00
	je fim//caso não haja mais para ler vai para o "fim"
	movl $3,(%ecx)//senão j=3
	jmp fim//vai para o fim
error:
	movl $-1,%eax//devolve -1 em caso de error
fim:
	ret


_stat_reg:
	inb $KBD_STATUS_REG
	ret

_get_code_mouse:
	inb $KBD_OUT_BUF
	ret




