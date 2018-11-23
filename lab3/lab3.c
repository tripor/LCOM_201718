#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "test3.h"
#include <limits.h>
#include <string.h>
#include <errno.h>

static int imprimir_uso(char **argv)
{
	printf("Nao foram especificados nenhuns argumentos. Porfavor use a seguintes notacoes:\n");
	printf("service run %s -args \"scan <0 em c e 1 em assembly>\"\n",argv[0]);
	printf("service run %s -args \"poll\"\n",argv[0]);
	printf("service run %s -args \"timedscan <tempo>\"\n",argv[0]);
	return 0;
}

static unsigned long transform_long(char *str, int base) //funcao retirada de lab2
{
	char *endptr;
		unsigned long val;

		/* Convert string to unsigned long */
		val = strtoul(str, &endptr, base);

		/* Check for conversion errors */
		if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
			perror("strtoul");
			return ULONG_MAX;
		}

		if (endptr == str) {
			printf("Nao foram usados digitos \"%s\"\n", str);
			return ULONG_MAX;
		}

		/* Successful conversion */
		return val;
}

int escolher_funcao(int argc, char **argv)
{
	unsigned long tipo,tempo;
	if(strncmp( argv[1] , "scan" , strlen("scan")) == 0 )
	{
		if(argc > 3) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 2 argumentos separados por espaco");
			return 1;
		}
		tipo=transform_long(argv[2],10);
		if(tipo==ULONG_MAX)return 1;
		printf("kbd:kbd_test_scan(%lu)\n",tipo);
		return kbd_test_scan(tipo);
	}
	else if(strncmp( argv[1] , "poll" , strlen("poll")) == 0 )
	{
		if(argc > 2) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 1 argumento.\n");
			return 1;
		}
		printf("kbd:kbd_test_poll()\n");
		return kbd_test_poll();
	}
	else if(strncmp( argv[1] , "timedscan" , strlen("timedscan")) == 0 )
	{
		if(argc > 3) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 2 argumentos separados por espaco");
			return 1;
		}
		tempo=transform_long(argv[2],10);
		if(tempo==ULONG_MAX)return 1;
		printf("kbd:kbd_test_timed_scan(%lu)\n",tempo);
		return kbd_test_timed_scan(tempo);
	}
	else
	{
		printf("A funcao escolhida \"%s\" nao existe.\n",argv[1]);
		return 1;
	}
	return 0;


}


int main(int argc, char **argv){

	sef_startup();
	sys_enable_iop(SELF);
	if (argc == 1)return imprimir_uso(argv); // a função não tem argumentos
	else return escolher_funcao(argc,argv);
}




