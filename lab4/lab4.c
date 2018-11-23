#include <minix/syslib.h>
#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "test4.h"

static int imprimir_uso(char **argv)
{
	printf("Nao foram especificados nenhuns argumentos. Porfavor use a seguintes notacoes:\n");
	printf("service run %s -args \"packet <quantidade de packets a ler>\"\n",argv[0]);
	printf("service run %s -args \"remote <periodo de tempo em milisegundos> <quantidade de packets a ler>\"\n",argv[0]);
	printf("service run %s -args \"async <tempo sem packets>\"\n",argv[0]);
	printf("service run %s -args \"gesture <tamanho da linha para acabar>\"\n",argv[0]);
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
	unsigned long packet,time,gesto;
	if(strncmp( argv[1] , "packet" , strlen("packet")) == 0 )
	{
		if(argc > 3) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 2 argumentos separados por espaco");
			return 1;
		}
		packet=transform_long(argv[2],10);
		if(packet==ULONG_MAX)return 1;
		printf("mouse:mouse_test_packet(%lu)\n",packet);
		return mouse_test_packet(packet);
	}
	else if(strncmp( argv[1] , "remote" , strlen("remote")) == 0 )
	{
		if(argc > 4) //foram inseridos mais que 3 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 3 argumento.\n");
			return 1;
		}
		packet=transform_long(argv[3],10);
		if(packet==ULONG_MAX)return 1;
		time=transform_long(argv[2],10);
		if(time==ULONG_MAX)return 1;
		printf("mouse:mouse_test_remote()\n");
		return mouse_test_remote(time,packet);
	}
	else if(strncmp( argv[1] , "async" , strlen("async")) == 0 )
	{
		if(argc > 3) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 2 argumentos separados por espaco");
			return 1;
		}
		time=transform_long(argv[2],10);
		if(time==ULONG_MAX)return 1;
		printf("mouse:mouse_test_async(%lu)\n",time);
		return mouse_test_async(time);
	}
	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0)
	{
		if (argc > 3) //foram inseridos mais que 2 argumentos e portanto dá erro
		{
			printf("Foi inserido um numero incorreto de argumentos.\n");
			printf("Insira 2 argumentos separados por espaco");
			return 1;
		}
		gesto = transform_long(argv[2], 10);
		if (gesto == ULONG_MAX)
			return 1;
		printf("mouse:mouse_test_gesture(%d)\n", gesto);
		return mouse_test_gesture(gesto);
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
	if (argc == 1)return imprimir_uso(argv); // a função não tem argumentos
	else return escolher_funcao(argc,argv);
}
