#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <string.h>
#include <math.h>

#include "lowProgram.h"
#include "macros.h"
#include "bitmap.h"
#include "program.h"

static conjunto sprites;
static conjunto_baloes baloes;
static conjunto_projeteis dardos;
static conjunto_pops pops;
static char *path;

void delete_pop(pop* delete);
void delete_monkey(imagem* delete);
void delete_balao(balao_type* delete);
void delete_projetil(projetil* delete);
void desenhar_baloes(unsigned char buffer[],balao_type **colisao);
void desenhar_balao_colisao(balao_type **colisao);

void guardar_path(char *program_path)
{
	path=program_path;
}

int verificar_baloes()
{
	if(baloes.size>0)
		return 0;
	else return 1;
}

void putpixel(int posX,int posY,unsigned char buffer[])
{
	if(posX*2>=LIMITE_ESQUERDA && posY>=LIMITE_CIMA && posX*2<=LIMITE_DIREITA && posY<=LIMITE_BAIXO)//verifica se o pixel a pintar não esta fora do mapa
	{
		buffer[posX*2+posY*H_RES]=PRETO;
		buffer[posX*2+posY*H_RES+1]=PRETO;
	}
}


void drawcircle(int x0, int y0, int radius,unsigned char buffer[])
 {
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y) {
		putpixel(x0 + x, y0 + y,buffer);
		putpixel(x0 + y, y0 + x,buffer);
		putpixel(x0 - y, y0 + x,buffer);
		putpixel(x0 - x, y0 + y,buffer);
		putpixel(x0 - x, y0 - y,buffer);
		putpixel(x0 - y, y0 - x,buffer);
		putpixel(x0 + y, y0 - x,buffer);
		putpixel(x0 + x, y0 - y,buffer);

		if (err <= 0) {
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0) {
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}

void line(int x0, int y0, int x1, int y1) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    //putpixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void avancar() {
	int i, j;
	balao_type *balao, *temp;
	balao = baloes.primeiraBalao->next;//primeiro balão
	for (i = 0; i < baloes.size; i++) {// percorre todos os balões
		for (j = 0; j < balao->speed; j++) {//faz com o que balão percorra o percurso dependendo da sua velocidade
			balao->percurso = balao->percurso->next;
			if (balao->percurso->next == NULL)//verifica se o balão já acabou o seu percurso
				j = balao->speed;
		}
		balao->posX = balao->percurso->posX;
		balao->posY = balao->percurso->posY;
		if (balao->percurso->next == NULL) {//se já acabou o percurso faz delete ao balão
			temp = balao->next;
			perde_vida(balao->hp);
			delete_balao(balao);
			balao = temp;
		} else//senão passa ao proximo
			balao = balao->next;
	}
}

void avancar_projeteis(unsigned char buffer[],balao_type **colisao,balao_type **vazio) {
	int i;
	projetil *dardo, *temp;
	dardo = dardos.primeiraProjeteis->next;//seleciona o primeiro projetil

	int dx,sx;
	int dy,sy;
	int err,e2;
	int x0,y0,x1,y1;

	int aconteceu=0,tamanho=dardos.size;

	for (i = 0; i < tamanho; i++) {//percorre todos os projeteis
		x0=dardo->posX;
		y0=dardo->posY;
		dardo->posX += dardo->speedX;
		dardo->posY += dardo->speedY;
		x1=dardo->posX;
		y1=dardo->posY;
		//algoritmo de desenhar uma linha e verificar se não existe nenhum balão nesse caminho
		dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		err = (dx > dy ? dx : -dy) / 2, e2;
		aconteceu=0;

		for (;;) {//avança na linha imaginaria
			while(1)//verifica enquanto houver balões nessa posição para rebentar
			{
				//verifica os 4 cantos do projetil
				if (*(colisao+ x0 + y0 * H_RES) != NULL) {
					aconteceu=aconteceu_colisao(*(colisao+x0 + y0 * H_RES),dardo);
					memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
					desenhar_baloes(buffer,colisao);
				}
				else if (*(colisao+x0 + dardo->mapa->bitmapInfoHeader.width*2 + y0 * H_RES) != NULL) {
					aconteceu=aconteceu_colisao(*(colisao+x0 + dardo->mapa->bitmapInfoHeader.width*2 + y0 * H_RES),dardo);
					memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
					desenhar_baloes(buffer,colisao);
				}
				else if (*(colisao+x0+ (y0 + dardo->mapa->bitmapInfoHeader.height)* H_RES) != NULL) {
					aconteceu=aconteceu_colisao(*(colisao+x0+ (y0 + dardo->mapa->bitmapInfoHeader.height)* H_RES),dardo);
					memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
					desenhar_baloes(buffer,colisao);
				}
				else if (*(colisao+x0 + dardo->mapa->bitmapInfoHeader.width*2+ (y0 + dardo->mapa->bitmapInfoHeader.height)* H_RES) != NULL) {
					aconteceu=aconteceu_colisao(*(colisao+x0 + dardo->mapa->bitmapInfoHeader.width*2+ (y0 + dardo->mapa->bitmapInfoHeader.height)* H_RES) ,dardo);
					memcpy(colisao,vazio,sizeof(balao_type*)*(H_RES*V_RES));
					desenhar_baloes(buffer,colisao);
				}
				else break;
				if(aconteceu==2)break;//caso o projetil tem de ser apagado
			}
			if(aconteceu==2) break;
			//algoritmo de desenhar a linha
			if (x0 == x1 && y0 == y1)
				break;
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
		//verifica se o projetil saiu do ecra e apaga ou se tem de ser apagado por causa de colisão
		if (dardo->posX > LIMITE_DIREITA/2 || dardo->posY > LIMITE_BAIXO || dardo->posY < LIMITE_CIMA-40 || dardo->posX < LIMITE_ESQUERDA/2-40 || aconteceu==2) {
			temp = dardo->next;
			delete_projetil(dardo);
			dardo = temp;
		} else
		{
			dardo = dardo->next;
		}


	}
}

void delete_pop(pop* delete)
{
	pop * temp;
	temp=delete->next;
	(delete->before)->next=delete->next;
	if(temp!=NULL)
	{
		temp->before=delete->before;
	}
	else
	{
		pops.ultimaPop=delete->before;
	}
	free(delete);
	pops.size--;
}

void delete_monkey(imagem* delete)
{
	imagem * temp;
	temp=delete->next;
	(delete->before)->next=delete->next;
	if(temp!=NULL)
	{
		temp->before=delete->before;
	}
	else {
		sprites.ultimaImagem = delete->before;
	}
	free(delete->linha_endereco);
	free(delete);
	sprites.size--;
}

void delete_balao(balao_type* delete)
{
	balao_type * temp;
	temp=delete->next;
	(delete->before)->next=delete->next;
	if(temp!=NULL)
	{
		temp->before=delete->before;
	}
	else
	{
		baloes.ultimaBalao=delete->before;
	}
	free(delete->linha_endereco);
	free(delete);
	baloes.size--;
}

void delete_projetil(projetil* delete)
{
	projetil * temp;
	temp=delete->next;
	(delete->before)->next=delete->next;
	if(temp!=NULL)
	{
		temp->before=delete->before;
	}
	else
	{
		dardos.ultimaProjeteis=delete->before;
	}
	free(delete);
	dardos.size--;
}

char* add_strings(const char* s1, const char* s2)
{
	char* result=malloc(strlen(s1)+strlen(s2)+1);
	strcpy(result,s1);
	strcat(result,s2);
	return result;
}

Bitmap* load_imagem(char* tipo_objeto)
{
	Bitmap* devolver;
	char* endereco = add_strings(path, "projeto_principal/src/images/");
	endereco = add_strings(endereco, tipo_objeto);
	endereco = add_strings(endereco, ".bmp");
	devolver = loadBitmap(endereco);
	int nobit = (devolver->bitmapInfoHeader.height-1)*(devolver->bitmapInfoHeader.width*2);
	int x,y,tamanho,tamanho2;
	devolver->linhas=malloc(sizeof(linha)*devolver->bitmapInfoHeader.height);
	for (y = 0; y < devolver->bitmapInfoHeader.height; y++) {//vai a todas as linhas do bmp
		tamanho=0;
		devolver->linhas[y].size=0;
		devolver->linhas[y].pos=malloc(sizeof(int));
		devolver->linhas[y].papel=malloc(sizeof(mapa));
		tamanho2=devolver->bitmapInfoHeader.width * 2;
		for (x=0; x < tamanho2; x++) { // vai a cada pixel
			if (*(devolver->bitmapData + nobit) !=BRANCO  ) {//verifica se é branco ou não
				if(tamanho==0)//se for a primeira vez que é branco
				{
					devolver->linhas[y].size++; // adiciona a quantidade de de espaços que tem de imprimir numa linha

					devolver->linhas[y].pos=realloc(devolver->linhas[y].pos,sizeof(int)*devolver->linhas[y].size);//adiciona espaço para guardar
					devolver->linhas[y].pos[devolver->linhas[y].size-1]=x;//adiciona a posição que tem de começar a imprimir

					devolver->linhas[y].papel=realloc(devolver->linhas[y].papel,sizeof(mapa)*devolver->linhas[y].size);//adiciona espaço de memoria para guardar
					devolver->linhas[y].papel[devolver->linhas[y].size-1].map=malloc(sizeof(char)*devolver->bitmapInfoHeader.width * 2);//aloca espaço para guardar o mapa
				}
				tamanho++;

				devolver->linhas[y].papel[devolver->linhas[y].size-1].sizet=tamanho;//guarda o tamanho da impressão por linha
				devolver->linhas[y].papel[devolver->linhas[y].size-1].map[tamanho-1]=*(devolver->bitmapData + nobit);//guarda os dados


			}
			else
			{
				tamanho=0;
			}
			nobit++;
		}
		nobit -= devolver->bitmapInfoHeader.width * 2;// anda do fim para o inicio porque senão imprime ao contrario
		nobit -= devolver->bitmapInfoHeader.width * 2;
	}

	return devolver;
}

void add_pop(Bitmap* nom,int posX, int posY)
{
	pop *novo=malloc(sizeof(pop));
	novo->posX = posX;
	novo->posY = posY;
	novo->before=pops.ultimaPop;
	novo->next=NULL;
	novo->mapa=nom;
	pops.ultimaPop->next=novo;
	pops.ultimaPop=novo;
	pops.size++;
}

void add_balao(Bitmap* nom,unsigned long hp,unsigned long speed,caminho *inicio)
{
	balao_type *novo=malloc(sizeof(balao_type));
	int x;
	novo->linha_endereco=malloc(sizeof(balao_type)*nom->bitmapInfoHeader.width*2);
	novo->posX = inicio->posX;
	novo->posY = inicio->posY;
	novo->hp=hp;
	novo->speed=speed;
	novo->percurso=inicio;
	novo->before=baloes.ultimaBalao;
	novo->next=NULL;
	novo->mapa=nom;
	for(x=0;x<nom->bitmapInfoHeader.width*2;x++)//guarda a linha de endereços
	{
		novo->linha_endereco[x]=novo;
	}
	baloes.ultimaBalao->next=novo;
	baloes.ultimaBalao=novo;
	baloes.size++;
}

void add_monkey(Bitmap* nom, Bitmap* tipo,int posX, int posY,unsigned long range,unsigned long speed_lancar,unsigned long pierce,unsigned long aa,monkey_type type)
{
	imagem *novo2=(imagem*)malloc(sizeof(imagem));
	int x;
	novo2->linha_endereco=malloc(sizeof(imagem)*nom->bitmapInfoHeader.width*2);
	novo2->tipo=type;
	novo2->mapa=nom;
	novo2->range=range;
	novo2->speed_lancar=speed_lancar;
	novo2->pierce=pierce;
	novo2->aa=aa;
	novo2->pops=0;
	novo2->tempo=0;
	novo2->projetil_tipo=tipo;
	novo2->posX = posX;
	novo2->posY=posY;
	novo2->before=sprites.ultimaImagem;
	novo2->next=NULL;
	novo2->select=NAO;
	for (x = 0; x < nom->bitmapInfoHeader.width * 2; x++)//guarda a linha de endereços
	{
		novo2->linha_endereco[x] = novo2;
	}

	sprites.ultimaImagem->next=novo2;
	sprites.ultimaImagem=novo2;
	sprites.size++;
}

void add_projetil(Bitmap* nom, int posX, int posY, unsigned long pierce,long speedX,long speedY,imagem *pertence) {
	projetil *novo2 = (projetil*) malloc(sizeof(projetil));
	novo2->pertence=pertence;
	novo2->mapa = nom;
	novo2->posX = posX;
	novo2->posY = posY;
	novo2->pierce = pierce;
	novo2->speedX = speedX;
	novo2->speedY = speedY;
	novo2->before = dardos.ultimaProjeteis;
	novo2->next = NULL;

	dardos.ultimaProjeteis->next = novo2;
	dardos.ultimaProjeteis = novo2;
	dardos.size++;
}

void disparo(imagem *macaco)
{
	if(macaco->tempo>=macaco->aa)
	{
		int i;
		balao_type *temp;
		balao_type *disparar;
		unsigned long max;
		long speedX,speedY;
		int distancia1,distancia2,distancia3,distancia4,range,distancia;
		range=macaco->range*macaco->range;
		temp = baloes.primeiraBalao->next;
		if(temp!=NULL)//se não houver nenhum balão
		{
			disparar = NULL;
			max=0;
			for (i = 0; i < baloes.size; i++) {//percorre todos os balões e vê qual é o primeiro que esta dentro do range da torre
				distancia1=(pow((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2) - temp->posX,2)                                     + pow((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)- temp->posY,2));
				distancia2=(pow((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2) -(temp->posX+temp->mapa->bitmapInfoHeader.width),2) + pow((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)- temp->posY,2));
				distancia3=(pow((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2) - temp->posX,2)                                     + pow((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)-(temp->posY+temp->mapa->bitmapInfoHeader.height),2));
				distancia4=(pow((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2) -(temp->posX+temp->mapa->bitmapInfoHeader.width),2) + pow((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)-(temp->posY+temp->mapa->bitmapInfoHeader.height),2));
				if(distancia1<=range || distancia2<=range || distancia3<=range || distancia4<=range)
				{
					if(temp->percurso->indice>=max)//guarda a melhor opção
					{
						distancia=(pow((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2) -(temp->posX+temp->mapa->bitmapInfoHeader.width/2),2) + pow((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)-(temp->posY+temp->mapa->bitmapInfoHeader.height/2),2));
						max=temp->percurso->indice;
						disparar=temp;
					}

				}
				temp = temp->next;
			}
			if(disparar!=NULL)//verificar se existe uma opção
			{
				//calcula a velocidade horizontal e vertical ,segundo o maximo que o macaco pode lançar
				distancia=sqrt(distancia);
				speedX=((macaco->posX+macaco->mapa->bitmapInfoHeader.width/2)-(disparar->posX+disparar->mapa->bitmapInfoHeader.width/2))*macaco->speed_lancar;
				speedY=((macaco->posY+macaco->mapa->bitmapInfoHeader.height/2)-(disparar->posY+disparar->mapa->bitmapInfoHeader.height/2))*macaco->speed_lancar;
				if(speedX<0)
				{
					speedX=-(abs(speedX)/distancia);
				}
				else
				{
					speedX=speedX/distancia;
				}
				if(speedY<0)
				{
					speedY=-(abs(speedY)/distancia);
				}
				else
				{
					speedY=speedY/distancia;
				}
				orientar_monkey(macaco,speedX,speedY);//orienta o macaco segundo o lançamento
				//adiciona o projetil
				add_projetil(macaco->projetil_tipo, (macaco->posX+macaco->mapa->bitmapInfoHeader.width/2), (macaco->posY+macaco->mapa->bitmapInfoHeader.height/2),macaco->pierce,-speedX,-speedY,macaco);
				macaco->tempo=0;
			}
		}
	}
	else
	{
		if(macaco->tempo<macaco->aa)//avança com o intervalo de tempo entre cada auto-ataque
		{
			macaco->tempo++;
		}
	}
}

int testar_board(int posX,int posY,imagem **board,Bitmap* nom)
{
	if((posX + nom->bitmapInfoHeader.width/2) >LIMITE_DIREITA/2 || (posY+nom->bitmapInfoHeader.height/2) >LIMITE_BAIXO)//verifica se metade do macaco não esta fora do mapa
	{
		return 0;
	}
	int x,y;
	for(x=posX+nom->bitmapInfoHeader.width/3;x<posX+nom->bitmapInfoHeader.width*2/3;x++)//verifica as posições horizontais de cima e em baixo
	{
		if(board[x + (posY+nom->bitmapInfoHeader.height/3)*H_RES] != NULL)
		{
			return 0;
		}
		if(board[x + (posY+nom->bitmapInfoHeader.height*2/3)*H_RES]!= NULL)
		{
			return 0;
		}

	}
	for(y=posY+nom->bitmapInfoHeader.height/3;y<posY+nom->bitmapInfoHeader.height*2/3;y++)//verifica as posições verticais da esquerda e direita
	{
		if(board[posX+nom->bitmapInfoHeader.width/3 + y*H_RES] != NULL)
		{
			return 0;
		}
		if(board[posX + nom->bitmapInfoHeader.width*2/3 + y*H_RES] != NULL)
		{
			return 0;
		}
	}
	return 1;//returna 1 se correr tudo bem
}

void reescrever_board(imagem **board,imagem **board_empty)
{
	int i,y;
	memcpy(board,board_empty,sizeof(imagem*)*(H_RES*V_RES));//copia o caminho para a base
	imagem *desenhar;
	desenhar=sprites.primeiraImagem->next;
	for (i = 0; i < sprites.size; i++) {//vai a cada macaco desenhar na mesa
		for (y = desenhar->mapa->bitmapInfoHeader.height/3; y < desenhar->mapa->bitmapInfoHeader.height*2/3; y++)
		{
			memcpy(board+desenhar->posX+(desenhar->posY+y)*H_RES+desenhar->mapa->bitmapInfoHeader.width/3,desenhar->linha_endereco,desenhar->mapa->bitmapInfoHeader.width*sizeof(imagem*)/3);
		}
		desenhar=desenhar->next;
	}

}

void desenhar_imagem(unsigned char buffer[],imagem *desenho)
{
	int x,y,tamanho;
	for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {

		if (y + desenho->posY < V_RES &&  y + desenho->posY >= 0) {//verificar os limites verticais

			for (x = 0; x < desenho->mapa->linhas[y].size; x++) {
				if((desenho->mapa->linhas[y].pos[x] + desenho->posX*2 + desenho->mapa->linhas[y].papel[x].sizet) < H_RES)//verifica se esta dentro do ecra, limite direito
				{

					if(desenho->mapa->linhas[y].pos[x] + desenho->posX*2>0)//dentro do limite esquerdo
					{
						memcpy(
							buffer + desenho->mapa->linhas[y].pos[x] + (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map,
							desenho->mapa->linhas[y].papel[x].sizet);

					}
					else
					{
						tamanho=-(desenho->mapa->linhas[y].pos[x] + desenho->posX*2);//imprime um tamanho para estar dentro dos limites
						if(desenho->mapa->linhas[y].papel[x].sizet-tamanho<0)tamanho=desenho->mapa->linhas[y].papel[x].sizet;
						memcpy(buffer + (y * H_RES) + desenho->posY * H_RES,
								desenho->mapa->linhas[y].papel[x].map + tamanho,
								desenho->mapa->linhas[y].papel[x].sizet
										- tamanho);
					}

				}
				else
				{
					//imprime um tamanho para estar dentro dos limites
					tamanho=desenho->mapa->linhas[y].papel[x].sizet -((desenho->mapa->linhas[y].pos[x] + desenho->posX*2 + desenho->mapa->linhas[y].papel[x].sizet)  - H_RES);
					if(tamanho<0)tamanho=0;
					memcpy(
							buffer + desenho->mapa->linhas[y].pos[x]
									+ (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map,
							tamanho);
				}


			}
		}
	}
}

void desenhar_macaco(unsigned char buffer[],imagem *desenho)
{
	int x,y,tamanho;
	for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {

		if (y + desenho->posY < LIMITE_BAIXO && y + desenho->posY >= LIMITE_CIMA) {

			for (x = 0; x < desenho->mapa->linhas[y].size; x++) {

				if ((desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
						+ desenho->mapa->linhas[y].papel[x].sizet)
						< LIMITE_DIREITA) {
					if (desenho->mapa->linhas[y].pos[x]
							+ desenho->posX * 2 > LIMITE_ESQUERDA) {
						memcpy(
								buffer + desenho->mapa->linhas[y].pos[x]
										+ (y * H_RES)
										+ desenho->posX * 2+desenho->posY*H_RES,
								desenho->mapa->linhas[y].papel[x].map,
								desenho->mapa->linhas[y].papel[x].sizet);

					} else {
						tamanho = -(desenho->mapa->linhas[y].pos[x]
								+ (desenho->posX * 2 - LIMITE_ESQUERDA));
						if (desenho->mapa->linhas[y].papel[x].sizet - tamanho
								< 0)
							tamanho = desenho->mapa->linhas[y].papel[x].sizet;
						memcpy(
								buffer + (y * H_RES) + desenho->posY * H_RES
										+ LIMITE_ESQUERDA,
								desenho->mapa->linhas[y].papel[x].map + tamanho,
								desenho->mapa->linhas[y].papel[x].sizet
										- tamanho);
					}

				} else {
					tamanho = desenho->mapa->linhas[y].papel[x].sizet
							- ((desenho->mapa->linhas[y].pos[x]
									+ desenho->posX * 2
									+ desenho->mapa->linhas[y].papel[x].sizet)
									- LIMITE_DIREITA);
					if (tamanho < 0)
						tamanho = 0;
					memcpy(
							buffer + desenho->mapa->linhas[y].pos[x]
									+ (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map, tamanho);
				}

			}
		}
	}
}

void desenhar_balao_colisao(balao_type **colisao)
{
	int i,y;
	balao_type *desenho;
	desenho = baloes.primeiraBalao->next;
	for (i = 0; i < baloes.size; i++) {
		for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {
			memcpy(colisao+desenho->posX*2+(desenho->posY+y)*H_RES,desenho->linha_endereco,desenho->mapa->bitmapInfoHeader.width*2);
		}
	}
}

void desenhar_balao(unsigned char buffer[], balao_type *desenho,balao_type **colisao) {
	int x, y, tamanho;
	for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {

		if (y + desenho->posY < LIMITE_BAIXO && y + desenho->posY >= LIMITE_CIMA) {

			for (x = 0; x < desenho->mapa->linhas[y].size; x++) {

				if ((desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
						+ desenho->mapa->linhas[y].papel[x].sizet) < LIMITE_DIREITA) {
					if (desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
							> LIMITE_ESQUERDA) {
						memcpy(
								buffer + desenho->mapa->linhas[y].pos[x]
										+ (y * H_RES)
										+ desenho->posX * 2+desenho->posY*H_RES,
								desenho->mapa->linhas[y].papel[x].map,
								desenho->mapa->linhas[y].papel[x].sizet);

					} else {
						tamanho = -(desenho->mapa->linhas[y].pos[x]
								+ (desenho->posX * 2-LIMITE_ESQUERDA));
						if (desenho->mapa->linhas[y].papel[x].sizet - tamanho
								< 0)
							tamanho = desenho->mapa->linhas[y].papel[x].sizet;
						memcpy(buffer + (y * H_RES) + desenho->posY * H_RES+LIMITE_ESQUERDA,
								desenho->mapa->linhas[y].papel[x].map + tamanho,
								desenho->mapa->linhas[y].papel[x].sizet
										- tamanho);
					}

				} else {
					tamanho = desenho->mapa->linhas[y].papel[x].sizet
							- ((desenho->mapa->linhas[y].pos[x]
									+ desenho->posX * 2
									+ desenho->mapa->linhas[y].papel[x].sizet)
									- LIMITE_DIREITA);
					if (tamanho < 0)
						tamanho = 0;
					memcpy(
							buffer + desenho->mapa->linhas[y].pos[x]
									+ (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map, tamanho);
				}

			}
		}

		memcpy(colisao+desenho->posX+(desenho->posY+y)*H_RES,desenho->linha_endereco,desenho->mapa->bitmapInfoHeader.width*2);//atualizar o mapa das colisões
	}
}

void desenhar_projetil(unsigned char buffer[], projetil *desenho) {
	int x, y, tamanho;
	for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {

		if (y + desenho->posY < LIMITE_BAIXO && y + desenho->posY >= LIMITE_CIMA) {

			for (x = 0; x < desenho->mapa->linhas[y].size; x++) {

				if ((desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
						+ desenho->mapa->linhas[y].papel[x].sizet) < LIMITE_DIREITA) {
					if (desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
							> LIMITE_ESQUERDA) {
						memcpy(
								buffer + desenho->mapa->linhas[y].pos[x]
										+ (y * H_RES)
										+ desenho->posX * 2+desenho->posY*H_RES,
								desenho->mapa->linhas[y].papel[x].map,
								desenho->mapa->linhas[y].papel[x].sizet);

					} else {
						tamanho = -(desenho->mapa->linhas[y].pos[x]
								+ (desenho->posX * 2-LIMITE_ESQUERDA));
						if (desenho->mapa->linhas[y].papel[x].sizet - tamanho
								< 0)
							tamanho = desenho->mapa->linhas[y].papel[x].sizet;
						memcpy(buffer + (y * H_RES) + desenho->posY * H_RES+LIMITE_ESQUERDA,
								desenho->mapa->linhas[y].papel[x].map + tamanho,
								desenho->mapa->linhas[y].papel[x].sizet
										- tamanho);
					}

				} else {
					tamanho = desenho->mapa->linhas[y].papel[x].sizet
							- ((desenho->mapa->linhas[y].pos[x]
									+ desenho->posX * 2
									+ desenho->mapa->linhas[y].papel[x].sizet)
									- LIMITE_DIREITA);
					if (tamanho < 0)
						tamanho = 0;
					memcpy(
							buffer + desenho->mapa->linhas[y].pos[x]
									+ (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map, tamanho);
				}

			}
		}
	}
}

void desenhar_pop(unsigned char buffer[], pop *desenho) {
	int x, y, tamanho;
	for (y = 0; y < desenho->mapa->bitmapInfoHeader.height; y++) {

		if (y + desenho->posY < LIMITE_BAIXO && y + desenho->posY >= LIMITE_CIMA) {

			for (x = 0; x < desenho->mapa->linhas[y].size; x++) {

				if ((desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
						+ desenho->mapa->linhas[y].papel[x].sizet) < LIMITE_DIREITA) {
					if (desenho->mapa->linhas[y].pos[x] + desenho->posX * 2
							> LIMITE_ESQUERDA) {
						memcpy(
								buffer + desenho->mapa->linhas[y].pos[x]
										+ (y * H_RES)
										+ desenho->posX * 2+desenho->posY*H_RES,
								desenho->mapa->linhas[y].papel[x].map,
								desenho->mapa->linhas[y].papel[x].sizet);

					} else {
						tamanho = -(desenho->mapa->linhas[y].pos[x]
								+ (desenho->posX * 2-LIMITE_ESQUERDA));
						if (desenho->mapa->linhas[y].papel[x].sizet - tamanho
								< 0)
							tamanho = desenho->mapa->linhas[y].papel[x].sizet;
						memcpy(buffer + (y * H_RES) + desenho->posY * H_RES+LIMITE_ESQUERDA,
								desenho->mapa->linhas[y].papel[x].map + tamanho,
								desenho->mapa->linhas[y].papel[x].sizet
										- tamanho);
					}

				} else {
					tamanho = desenho->mapa->linhas[y].papel[x].sizet
							- ((desenho->mapa->linhas[y].pos[x]
									+ desenho->posX * 2
									+ desenho->mapa->linhas[y].papel[x].sizet)
									- LIMITE_DIREITA);
					if (tamanho < 0)
						tamanho = 0;
					memcpy(
							buffer + desenho->mapa->linhas[y].pos[x]
									+ (y * H_RES)
									+ desenho->posX * 2+desenho->posY*H_RES,
							desenho->mapa->linhas[y].papel[x].map, tamanho);
				}

			}
		}
	}
	delete_pop(desenho);//delete do pop apos o framde que aparece
}

void update_ecra(unsigned char *video_mem,unsigned char buffer[],unsigned char inicial[])
{
	memcpy(video_mem,buffer,H_RES*V_RES);
	memcpy(buffer,inicial,H_RES*V_RES);
}

void desenhar_baloes(unsigned char buffer[],balao_type **colisao)
{
	int i;
	balao_type *desenhar2;
	desenhar2 = baloes.primeiraBalao->next;
	for (i = 0; i < baloes.size; i++) {
		desenhar_balao(buffer, desenhar2,colisao);
		desenhar2 = desenhar2->next;
	}
}

void desenhar_objeto(unsigned char buffer[],balao_type **colisao)
{
	int i;
	imagem *desenhar;
	desenhar=sprites.primeiraImagem->next;
	for (i = 0; i < sprites.size; i++) {
		desenhar_macaco(buffer, desenhar);
		disparo(desenhar);
		if(desenhar->select==SIM)
		{
			drawcircle(desenhar->posX+desenhar->mapa->bitmapInfoHeader.width/2,desenhar->posY+desenhar->mapa->bitmapInfoHeader.height/2,desenhar->range,buffer);
		}
		desenhar=desenhar->next;
	}
	desenhar_baloes(buffer,colisao);
	projetil *desenhar3;
	desenhar3 = dardos.primeiraProjeteis->next;
	for (i = 0; i < dardos.size; i++) {
		desenhar_projetil(buffer, desenhar3);
		desenhar3 = desenhar3->next;
	}
	pop *desenhar4;
	desenhar4 = pops.primeiraPop->next;
	int tamanho=pops.size;
	for (i = 0; i < tamanho; i++) {
		desenhar_pop(buffer, desenhar4);
		desenhar4 = desenhar4->next;
	}

}

void desenhar_overlay(unsigned char buffer[],imagem *novo)
{
	novo->posX = 0;
	novo->posY = 0;
	desenhar_imagem(buffer,novo);
}

void desenhar_rato(unsigned char buffer[], mouse_data dados,imagem *novo)
{
	novo->posX = dados.posX;
	novo->posY = dados.posY;
	desenhar_imagem(buffer,novo);
	if(dados.tipo != NADA)//desenha um circulo com o range do macaco que foi selecionado
	{
		drawcircle(novo->posX+novo->mapa->bitmapInfoHeader.width/2,novo->posY+novo->mapa->bitmapInfoHeader.height/2,novo->range,buffer);
	}
}

void unselect_monkey()
{
	int i;
	imagem *desenhar;
	desenhar = sprites.primeiraImagem->next;
	for (i = 0; i < sprites.size; i++)
	{
		if (desenhar->select == SIM)
		{
			desenhar->select=NAO;
		}
		desenhar = desenhar->next;
	}
}

void init(unsigned char buffer[],unsigned char inicial[],balao_type **vazio,imagem **board)
{
	int x;
	for (x = 0; x < V_RES * H_RES; x++) {
		buffer[x] = 0;
		inicial[x]=0;
		vazio[x]=NULL;
		board[x]=NULL;
	}
	Bitmap *ov;
	ov = load_imagem("mapa1");//faz load do mapa e guarda-o em inicial que é usado como base
	imagem *ajuda=malloc(sizeof(imagem));
	ajuda->mapa=ov;
	ajuda->posX=0;
	ajuda->posY=0;
	desenhar_imagem(inicial,ajuda);
	Bitmap *ov2;
	ov2 = load_imagem("overlay1");//faz load do overlay que é usado e imprime por cima do buffer inicial
	imagem *ajuda2 = malloc(sizeof(imagem));
	ajuda2->mapa = ov2;
	ajuda2->posX = 0;
	ajuda2->posY = 0;
	desenhar_imagem(inicial, ajuda2);

	imagem *novo=malloc(sizeof(imagem));//inicializa a variavel que guarda os macacos com um macaco imaginario para ter uma ligação e repete este procedimento para os outros casos
	novo->next=NULL;
	novo->before=NULL;
	sprites.primeiraImagem=novo;
	sprites.ultimaImagem=novo;
	sprites.size=0;

	balao_type *novo2 = malloc(sizeof(balao_type));
	novo2->next = NULL;
	novo2->hp = 0;
	novo2->speed = 0;
	novo2->before = NULL;
	novo2->percurso = NULL;
	baloes.primeiraBalao = novo2;
	baloes.ultimaBalao = novo2;
	baloes.size = 0;

	projetil *novo3 = malloc(sizeof(projetil));
	novo3->next = NULL;
	novo3->pierce = 0;
	novo3->speedX = 0;
	novo3->speedY = 0;
	novo3->before = NULL;
	dardos.primeiraProjeteis = novo3;
	dardos.ultimaProjeteis = novo3;
	dardos.size = 0;

	pop *novo4 = malloc(sizeof(pop));
	novo4->next = NULL;
	novo4->before = NULL;
	pops.primeiraPop = novo4;
	pops.ultimaPop = novo4;
	pops.size = 0;

}


