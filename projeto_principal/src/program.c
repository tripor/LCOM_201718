#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>

#include "lowProgram.h"
#include "bitmap.h"
#include "program.h"
#include "macros.h"
#include "data_storage.h"
#include "caminhos.h"
#include "round.h"
#include "ser.h"

static Bitmap* loadedImages;
static Bitmap* loadedBaloes;
static Bitmap* loadedSuper;
static Bitmap* loadedDardo;
static Bitmap* loadedMonkey;
static Bitmap* loadedPop;
static Bitmap* loadedNumero;
static int* velocidades;
static int money,vida;
static caminho* inicio;
static unsigned char pop_tipo;

static mouse_over over=OVERNADA;
static int posicaoX,posicaoY;

static int tempo=1;
static conjunto_ronda *loadedRonda;
static baloes_ronda *baloes_da_ronda;

int aconteceu_colisao(balao_type *balao,projetil *dardo)
{
	if(pop_tipo>=3)pop_tipo=0;
	if(dardo->pierce>balao->hp)//se o dardo tiver mais capacidade de rebentar que o hp do balão
	{
		add_pop(&loadedPop[pop_tipo],balao->posX-15,balao->posY-10);
		pop_tipo++;
		dardo->pertence->pops+=balao->hp;
		delete_balao(balao);
		money+=balao->hp;
		dardo->pierce-=balao->hp;
		return 1;
	}
	else if(dardo->pierce<balao->hp)//caso o hp do balão seja maior que o pierce do projetil
	{
		add_pop(&loadedPop[pop_tipo],balao->posX-15,balao->posY-10);
		pop_tipo++;
		dardo->pertence->pops+=dardo->pierce;
		balao->hp-=dardo->pierce;
		money+=dardo->pierce;
		balao->speed=velocidades[balao->hp];
		balao->mapa=&(loadedBaloes[balao->hp]);
		return 2;
	}
	else//caso o hp e pierce sejam iguais tem de ser apagar o balão e o projetil
	{
		money+=balao->hp;
		add_pop(&loadedPop[pop_tipo],balao->posX-15,balao->posY-10);
		pop_tipo++;
		dardo->pertence->pops+=balao->hp;
		delete_balao(balao);
		return 2;
	}
	return 2;// 2 para apagar o projetil
}

void carregar_imagens()
{
	loadedImages = malloc(sizeof(Bitmap) * 5);
	loadedImages[0] = *(load_imagem("super"));
	loadedImages[1] = *(load_imagem("ball"));
	loadedImages[2] = *(load_imagem("red"));
	loadedImages[3] = *(load_imagem("blue"));
	loadedImages[4] = *(load_imagem("mouse"));

	loadedBaloes = malloc(sizeof(Bitmap) * 6);
	loadedBaloes[1]=*(load_imagem("red"));
	loadedBaloes[2]=*(load_imagem("blue"));
	loadedBaloes[3]=*(load_imagem("green"));
	loadedBaloes[4]=*(load_imagem("yellow"));
	loadedBaloes[5]=*(load_imagem("pink"));

	velocidades=malloc(sizeof(int)*6);
	velocidades[1]=1;
	velocidades[2]=2;
	velocidades[3]=3;
	velocidades[4]=5;
	velocidades[5]=7;


	loadedSuper=malloc(sizeof(Bitmap) * 8);
	loadedSuper[0]=*(load_imagem("super3"));
	loadedSuper[1]=*(load_imagem("super4"));
	loadedSuper[2]=*(load_imagem("super5"));
	loadedSuper[3]=*(load_imagem("super6"));
	loadedSuper[4]=*(load_imagem("super7"));
	loadedSuper[5]=*(load_imagem("super8"));
	loadedSuper[6]=*(load_imagem("super1"));
	loadedSuper[7]=*(load_imagem("super2"));

	loadedDardo=malloc(sizeof(Bitmap) * 8);
	loadedDardo[0]=*(load_imagem("dart1"));
	loadedDardo[1]=*(load_imagem("dart2"));
	loadedDardo[2]=*(load_imagem("dart3"));
	loadedDardo[3]=*(load_imagem("dart4"));
	loadedDardo[4]=*(load_imagem("dart5"));
	loadedDardo[5]=*(load_imagem("dart6"));
	loadedDardo[6]=*(load_imagem("dart7"));
	loadedDardo[7]=*(load_imagem("dart8"));

	loadedMonkey=malloc(sizeof(Bitmap) * 8);
	loadedMonkey[0]=*(load_imagem("monkey1"));
	loadedMonkey[1]=*(load_imagem("monkey2"));
	loadedMonkey[2]=*(load_imagem("monkey3"));
	loadedMonkey[3]=*(load_imagem("monkey4"));
	loadedMonkey[4]=*(load_imagem("monkey5"));
	loadedMonkey[5]=*(load_imagem("monkey6"));
	loadedMonkey[6]=*(load_imagem("monkey7"));
	loadedMonkey[7]=*(load_imagem("monkey8"));

	loadedPop=malloc(sizeof(Bitmap) * 3);
	loadedPop[0]=*(load_imagem("pop1"));
	loadedPop[1]=*(load_imagem("pop2"));
	loadedPop[2]=*(load_imagem("pop3"));

	loadedNumero=malloc(sizeof(Bitmap) * 10);
	loadedNumero[0]=*(load_imagem("numero0"));
	loadedNumero[1]=*(load_imagem("numero1"));
	loadedNumero[2]=*(load_imagem("numero2"));
	loadedNumero[3]=*(load_imagem("numero3"));
	loadedNumero[4]=*(load_imagem("numero4"));
	loadedNumero[5]=*(load_imagem("numero5"));
	loadedNumero[6]=*(load_imagem("numero6"));
	loadedNumero[7]=*(load_imagem("numero7"));
	loadedNumero[8]=*(load_imagem("numero8"));
	loadedNumero[9]=*(load_imagem("numero9"));
}

void orientar_pos(imagem *monkey,int pos)
{
	if(monkey->tipo==SUPER)
	{
		monkey->mapa=&loadedSuper[pos];
		monkey->projetil_tipo=&loadedDardo[pos];
	}
	else if(monkey->tipo==DART)
	{
		monkey->mapa=&loadedMonkey[pos];
		monkey->projetil_tipo=&loadedDardo[pos];
	}
}

void comeca(imagem **board)
{
	inicio=malloc(sizeof(caminho));
	inicio->next=NULL;
	criar_caminho1(inicio,board);//carrega o caminho para a memoria

	loadedRonda=criar_roundas();//cria as rondas do jogo
	baloes_da_ronda=loadedRonda->ronda;

	carregar_imagens();//carrega todas as imagens necessarias para o jogo
	pop_tipo=0;
	money=STARTING_MONEY;
	vida=STARTING_VIDA;
}

void orientar_monkey(imagem *monkey,int speedX,int speedY)
{
	if(speedX!=0 && speedY!=0)
	{
		int er=(speedY*100)/speedX,er2=(speedX*100)/speedY;
		int pos=0;
		if(speedX>0 && speedY>0)//quadrante de esquerda para cima
		{
			if(er<=50)
			{
				pos=4;
			}
			else if(er<=100)
			{
				pos=5;
			}
			else if (er2<=50)
			{
				pos=6;
			}
			else
			{
				pos=5;
			}

		}
		else if(speedX<0 && speedY>0)//quadrante direita para cima
		{
			er=-er;
			er2=-er2;
			if (er <= 50) {
				pos = 0;
			} else if (er <= 100) {
				pos = 7;
			} else if (er2 <= 50) {
				pos = 6;
			} else {
				pos = 7;
			}
		}
		else if(speedX<0 && speedY<0)//direita para baixo
		{
			if (er <= 50) {
				pos = 0;
			} else if (er <= 100) {
				pos = 1;
			} else if (er2 <= 50) {
				pos = 2;
			} else {
				pos = 1;
			}
		}
		else if(speedX>0 && speedY<0)//esquerda para baixo
		{
			er=-er;
			er2=-er2;
			if (er <= 50) {
				pos = 4;
			} else if (er <= 100) {
				pos = 3;
			} else if (er2 <= 50) {
				pos = 2;
			} else {
				pos = 3;
			}
		}
		orientar_pos(monkey,pos);
	}
	else//caso uma das velocidades seja nula
	{
		if(speedX==0)//caso seja nula a horizontal
		{
			if(speedY>0)
			{
				orientar_pos(monkey,6);
			}
			else if(speedY<0)
			{
				orientar_pos(monkey,2);
			}
		}
		else if(speedY==0)//caso seja nula a vertical
		{
			if(speedX>0)
			{
				orientar_pos(monkey,4);
			}
			else if(speedY<0)
			{
				orientar_pos(monkey,0);
			}
		}
	}
}

void perde_vida(unsigned long quantidade)
{
	vida-=quantidade;
	if(vida<0)vida=0;
}

void desenhar_numero(int posX,int posY,int numero,unsigned char buffer[])
{
	int resto=numero;
	do {
		imagem *desenho = malloc(sizeof(imagem));
		desenho->posX = posX;
		desenho->posY = posY;
		desenho->mapa = &loadedNumero[resto % 10];
		posX -= 9;
		resto = resto / 10;
		desenhar_imagem(buffer, desenho);
		free(desenho);

	} while (resto != 0);

}

void add_dart(int posX,int posY)
{
	add_monkey(&loadedMonkey[6],&loadedDardo[6], posX, posY,RANGE_DART,VELO_DART,PIERCE_DART,AA_DART,DART);
	money-=PRECO_DART;
}

void add_super(int posX,int posY)
{
	add_monkey(&loadedSuper[6],&loadedDardo[6], posX, posY,RANGE_SUPER,VELO_SUPER,PIERCE_SUPER,AA_SUPER,SUPER);
	money-=PRECO_SUPER;
}


void add_red() {
	add_balao(&loadedBaloes[1], 1, velocidades[1], inicio);
}


void add_blue() {
	add_balao(&loadedBaloes[2], 2, velocidades[2], inicio);
}


void add_green() {
	add_balao(&loadedBaloes[3], 3, velocidades[3], inicio);
}


void add_yellow() {
	add_balao(&loadedBaloes[4], 4, velocidades[4], inicio);
}


void add_pink() {
	add_balao(&loadedBaloes[5], 5, velocidades[5], inicio);
}

menu update(unsigned char buffer[],menu estado)
{
	if(vida<=0)return SAIR;
	if(tempo==451)tempo=450;
	tempo++;
	int resto_money=money,posX_money=980,posY_money=7,resto_vida=vida,posX_vida=980,posY_vida=38;
	do{//desenha o dinheiro no ecra
		imagem *desenho=malloc(sizeof(imagem));
		desenho->posX=posX_money;
		desenho->posY=posY_money;
		desenho->mapa=&loadedNumero[resto_money%10];
		posX_money-=9;
		resto_money=resto_money/10;
		desenhar_imagem(buffer,desenho);
		free(desenho);

	} while(resto_money!=0);

	do {//desenha as vidas no ecra
		imagem *desenho = malloc(sizeof(imagem));
		desenho->posX = posX_vida;
		desenho->posY = posY_vida;
		desenho->mapa = &loadedNumero[resto_vida % 10];
		posX_vida -= 9;
		resto_vida = resto_vida / 10;
		desenhar_imagem(buffer, desenho);
		free(desenho);

	} while (resto_vida != 0);

	if(over==OVERDART)//caso o rato esteja sobre um macaco, mostra o preço
	{
		desenhar_numero(posicaoX-15,posicaoY,PRECO_DART,buffer);
	}
	else if(over==OVERSUPER)
	{
		desenhar_numero(posicaoX-15,posicaoY,PRECO_SUPER,buffer);
	}

	if(baloes_da_ronda->next!=NULL)//função que trata dos balhões que devem aparecer na ronda segundo as informações dadas
	{
		if(baloes_da_ronda->tempo_espera<=tempo)
		{
			if(baloes_da_ronda->tipo==RED)
			{
				add_red();
			}
			else if(baloes_da_ronda->tipo == BLUE)
			{
				add_blue();
			}
			else if(baloes_da_ronda->tipo == GREEN)
			{
				add_green();
			}
			else if(baloes_da_ronda->tipo == YELLOW)
			{
				add_yellow();
			}
			else if(baloes_da_ronda->tipo == PINK)
			{
				add_pink();
			}
			baloes_da_ronda=baloes_da_ronda->next;
			tempo=0;
		}
	}
	else if(loadedRonda->next==NULL)//caso todas as roundas tenham acabado e não haja mais balões no ecra, o jogador ganhou
	{
		if(verificar_baloes())
			return SAIR;
	}
	else if(tempo>=450)//se for o fim da ronda ele espera uns segundos antes de começar uma ronda nova
	{
		loadedRonda=loadedRonda->next;
		baloes_da_ronda=loadedRonda->ronda;
	}
	// 140 745 |  525 745
	desenhar_numero(120-15,743,loadedRonda->indice,buffer);//desenha a ronda no ecra
	desenhar_numero(505-15,743,loadedRonda->rbe,buffer);//desenha o rbe no ecra
	return estado;
}

void mouse_controlador(mouse_data *dados,imagem *mouse,imagem **board,imagem **board_empty)
{
	posicaoX=dados->posX;
	posicaoY=dados->posY;
	if(dados->posX>=900 && dados->posX<=960 && dados->posY>=105 && dados->posY<=165)//caso o rato esteja sobre o macaco dardo
	{
		over=OVERDART;
	}
	else if(dados->posX>=900 && dados->posX<=960 && dados->posY>=(105+60*3) && dados->posY<=(165+60*3))//caso o rato esteja sobre o super macaco
	{
		over=OVERSUPER;
	}
	else
	{
		over=OVERNADA;
	}

	if(dados->rightBotton == RIGHTCLICK)//retira o selecionamento do rato
	{
		unselect_monkey();
		mouse->mapa=&loadedImages[4];
		dados->tipo=NADA;
	}
	if(dados->leftBotton == LEFTCLICK)
	{
		if(dados->tipo == SUPER)//caso for para colocar um super macaco
		{
			reescrever_board(board,board_empty);
			if(testar_board( dados->posX,dados->posY,board,&loadedImages[0]))//verificar se a posição que quer colocar não tem nada lá escrita
			{
				send_macaco(dados->posX, dados->posY,dados->tipo);//transmite a informação para o outro jogador
				add_monkey(&loadedSuper[6],&loadedDardo[6], dados->posX, dados->posY,mouse->range,VELO_SUPER,PIERCE_SUPER,AA_SUPER,dados->tipo);
				money-=PRECO_SUPER;//retira o preço
			}
			reescrever_board(board,board_empty);
			mouse->mapa=&loadedImages[4];
			dados->tipo=NADA;
		}
		else if(dados->tipo == DART)//caso for para colocar um macaco normal
		{
			reescrever_board(board,board_empty);
			if(testar_board( dados->posX,dados->posY,board,&loadedImages[0]))//verificar se a posição que quer colocar não tem nada lá escrita
			{
				send_macaco(dados->posX, dados->posY,dados->tipo);//transmite a informação para o outro jogador
				add_monkey(&loadedMonkey[6],&loadedDardo[6], dados->posX, dados->posY,mouse->range,VELO_DART,PIERCE_DART,AA_DART,dados->tipo);
				money-=PRECO_DART;//retira  o preço
			}
			reescrever_board(board,board_empty);
			mouse->mapa=&loadedImages[4];
			dados->tipo=NADA;
		}
		if(dados->tipo == NADA)
		{
			if(board[dados->posX+dados->posY*H_RES] != NULL)//seleciona um macado do mapa
			{
				unselect_monkey();
				board[dados->posX+dados->posY*H_RES]->select=SIM;
				dados->select=SIM;
				dados->selected_monkey=board[dados->posX+dados->posY*H_RES];
			}
			else//retira a seleção caso exista
			{
				unselect_monkey();
				dados->select=NAO;
				dados->selected_monkey=NULL;
			}
		}

		//no ecra direito com as opção das torres verifica se tem dinheiro suficiente e faz as alterações necessarios caso seja suficiente
		if(dados->posX>=900 && dados->posX<=960 && dados->posY>=105 && dados->posY<=165)// 800 105 | 960 155 | 1024 ; x-900x960-1024 y-105+60
		{
			if(money>=PRECO_DART)
			{
				unselect_monkey();
				dados->select = NAO;
				dados->selected_monkey = NULL;
				mouse->mapa = &loadedMonkey[6];
				mouse->range = RANGE_DART;
				dados->tipo = DART;
			}
		}
		else if(dados->posX>=900 && dados->posX<=960 && dados->posY>=(105+60*3) && dados->posY<=(165+60*3))
		{
			if(money>=PRECO_SUPER)
			{
				unselect_monkey();
				dados->select = NAO;
				dados->selected_monkey = NULL;
				mouse->mapa=&loadedSuper[6];
				mouse->range=RANGE_SUPER;
				dados->tipo=SUPER;
			}

		}
	}
}

void keypressed(unsigned long scancode, mouse_data *dados,imagem *mouse)
{
	if (scancode == Q_MAKE_CODE) {
		if(money>=PRECO_DART)
		{
			mouse->mapa = &loadedMonkey[6];
			mouse->range = RANGE_DART;
			dados->tipo = DART;
		}
	} else if (scancode == W_MAKE_CODE) {
		if(money>=PRECO_SUPER)
		{
			mouse->mapa=&loadedSuper[6];
			mouse->range=RANGE_SUPER;
			dados->tipo=SUPER;
		}
	} else if (scancode == E_MAKE_CODE) {

	} else if (scancode == R_MAKE_CODE) {
		//add_monkey(&loadedImages[3],&loadedImages[4], dados.posX, dados.posY);
	} else if (scancode == T_MAKE_CODE) {
		//add_projetil(&loadedImages[4], dados.posX, dados.posY,1,2,10);
	} else if (scancode == DELETE_MAKE_CODE){
		if(dados->select==SIM)
		{
			if(dados->selected_monkey->tipo==DART)
			{
				money+=PRECO_DART*3/4;
			}
			else if(dados->selected_monkey->tipo==SUPER)
			{
				money+=PRECO_SUPER*3/4;
			}
			delete_monkey(dados->selected_monkey);
		}
	}
}

void keypressed_jogador1(unsigned long scancode, mouse_data *dados,imagem *mouse)
{
	if (scancode == Q_MAKE_CODE) {
		if(money>=PRECO_DART)
		{
			mouse->mapa = &loadedMonkey[6];
			mouse->range = RANGE_DART;
			dados->tipo = DART;
		}
	} else if (scancode == W_MAKE_CODE) {
		if(money>=PRECO_SUPER)
		{
			mouse->mapa=&loadedSuper[6];
			mouse->range=RANGE_SUPER;
			dados->tipo=SUPER;
		}
	}
}

void keypressed_jogador2(unsigned long scancode)
{
	if (scancode == Q_MAKE_CODE) {
		add_red();
		send_bloon('R');
	} else if (scancode == W_MAKE_CODE) {
		add_blue();
		send_bloon('B');
	} else if (scancode == E_MAKE_CODE) {
		add_green();
		send_bloon('G');
	} else if (scancode == R_MAKE_CODE) {
		add_yellow();
		send_bloon('Y');
	} else if (scancode == T_MAKE_CODE) {
		add_pink();
		send_bloon('P');
	}
}


