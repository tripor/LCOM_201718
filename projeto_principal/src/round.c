#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "round.h"

baloes_ronda* ronda1()
{
	baloes_ronda *primeiro=malloc(sizeof(baloes_ronda));
	primeiro->tipo=RED;
	primeiro->tempo_espera=30;

	baloes_ronda *guardar=primeiro;

	int i;
	for(i=1;i<=20;i++)
	{
		baloes_ronda *novo=malloc(sizeof(baloes_ronda));
		primeiro->next=novo;
		novo->tipo=RED;
		novo->tempo_espera=30;
		novo->next=NULL;
		primeiro=novo;
	}
	return guardar;
}

baloes_ronda* ronda2()
{
	baloes_ronda *primeiro=malloc(sizeof(baloes_ronda));
	primeiro->tipo=RED;
	primeiro->tempo_espera=30;

	baloes_ronda *guardar=primeiro;

	int i;
	for(i=1;i<=30;i++)
	{
		baloes_ronda *novo=malloc(sizeof(baloes_ronda));
		primeiro->next=novo;
		novo->tipo=RED;
		novo->tempo_espera=20;
		novo->next=NULL;
		primeiro=novo;
	}
	return guardar;
}
baloes_ronda* ronda3()
{
	baloes_ronda *primeiro=malloc(sizeof(baloes_ronda));
	primeiro->tipo=RED;
	primeiro->tempo_espera=30;

	baloes_ronda *guardar=primeiro;

	int i;
	for(i=1;i<=20;i++)
	{
		baloes_ronda *novo=malloc(sizeof(baloes_ronda));
		primeiro->next=novo;
		novo->tipo=RED;
		novo->tempo_espera=15;
		novo->next=NULL;
		primeiro=novo;
	}
	for (i = 0; i <= 5; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 20;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}

baloes_ronda* ronda4() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 20;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}

baloes_ronda* ronda5() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 5; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 25; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda6() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 20;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda7() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 20; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 25; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda8() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 10; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 20; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 14; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda9() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = GREEN;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 30; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda10() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = BLUE;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 102; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda11() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 10; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 10; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 12; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 2; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 20;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda12() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = BLUE;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 10; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 5; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda13() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 100; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 2;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 23; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 4; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda14() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 50; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 2;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 15; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = BLUE;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 10; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 9; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda15() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = RED;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 20; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = RED;
		novo->tempo_espera = 2;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 12; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 5; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 3; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = PINK;
		novo->tempo_espera = 15;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}
baloes_ronda* ronda16() {
	baloes_ronda *primeiro = malloc(sizeof(baloes_ronda));
	primeiro->tipo = GREEN;
	primeiro->tempo_espera = 30;

	baloes_ronda *guardar = primeiro;

	int i;
	for (i = 1; i <= 20; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = GREEN;
		novo->tempo_espera = 5;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 8; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = YELLOW;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	for (i = 0; i <= 4; i++) {
		baloes_ronda *novo = malloc(sizeof(baloes_ronda));
		primeiro->next = novo;
		novo->tipo = PINK;
		novo->tempo_espera = 10;
		novo->next = NULL;
		primeiro = novo;
	}
	return guardar;
}

conjunto_ronda* criar_roundas()
{
	conjunto_ronda *inicio=malloc(sizeof(conjunto_ronda));
	inicio->indice=1;
	inicio->rbe=20;
	inicio->ronda=ronda1();//cria a ronda 1

	conjunto_ronda *guardar=inicio;

	int i;

	for(i=2;i<=16;i++) //cria as rondas 2-16
	{
		conjunto_ronda *novo=malloc(sizeof(conjunto_ronda));
		inicio->next=novo;
		novo->indice=i;
		if(i==2)
		{
			novo->rbe = 30;
			novo->ronda = ronda2();

		}
		else if(i==3)
		{
			novo->rbe = 30;
			novo->ronda = ronda3();

		}
		else if(i==4)
		{
			novo->rbe = 60;
			novo->ronda = ronda4();

		}
		else if(i==5)
		{
			novo->rbe = 55;
			novo->ronda = ronda5();

		}
		else if(i==6)
		{
			novo->rbe = 57;
			novo->ronda = ronda6();

		}
		else if(i==7)
		{
			novo->rbe = 85;
			novo->ronda = ronda7();

		}
		else if(i==8)
		{
			novo->rbe = 92;
			novo->ronda = ronda8();

		}
		else if(i==9)
		{
			novo->rbe = 90;
			novo->ronda = ronda9();

		}
		else if(i==10)
		{
			novo->rbe = 204;
			novo->ronda = ronda10();

		}
		else if(i==11)
		{
			novo->rbe = 74;
			novo->ronda = ronda11();

		}
		else if(i==12)
		{
			novo->rbe = 80;
			novo->ronda = ronda12();

		}
		else if(i==13)
		{
			novo->rbe = 185;
			novo->ronda = ronda13();

		}
		else if(i==14)
		{
			novo->rbe = 145;
			novo->ronda = ronda14();

		}
		else if(i==15)
		{
			novo->rbe = 91;
			novo->ronda = ronda15();

		}
		else if(i==16)
		{
			novo->rbe = 112;
			novo->ronda = ronda16();

		}

		novo->next=NULL;
		inicio=novo;
	}
	return guardar;

}
