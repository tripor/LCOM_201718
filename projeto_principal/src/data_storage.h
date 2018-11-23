#ifndef _DATA_STORAGE_H_
#define _DATA_STORAGE_H_

/** @defgroup Data_storage Data_storage
 * @details Formato da maneira de como os dados vão ser guardados
 */

#include "bitmap.h"

typedef enum {RIGHTCLICK,RIGHTDOWN,RIGHTUP} right_state;
typedef enum {LEFTCLICK,LEFTDOWN,LEFTUP} left_state;
typedef enum {NADA,DART,SUPER} monkey_type;
typedef enum {SIM,NAO} selecionado;
typedef enum {INICIAL,JOGO,JOGOMULTI,SAIR} menu;
typedef enum {WAITING,PRONTO1,PRONTO2,LOADED,LOADING} menu_multi;

typedef enum {UN,SEL} menu_inicial;

typedef enum {OVERNADA,OVERDART,OVERSUPER} mouse_over;

typedef enum {RED,BLUE,GREEN,YELLOW,PINK} tipo_balao;


///lista ligada que guarda os balões que vão ser lançados e quando o intervalo entre eles
typedef struct br{
	tipo_balao tipo;
	int tempo_espera;
	struct br* next;
}baloes_ronda;

///lista ligada que guarda as roundas
typedef struct ron{
	int indice;
	int rbe;/**< @brief quantidades de pops necessário a rounda inteira*/
	baloes_ronda *ronda;
	struct ron* next;
}conjunto_ronda;

///lista ligada com o caminho dos balões
typedef struct li{
	short posX;
	short posY;
	unsigned long indice;
	struct li *next;
}caminho;

///lista duplamente ligada que guarda a animação de rebentar um balão
typedef struct p{
	int posX;
	int posY;
	struct p* next;
	struct p* before;
	Bitmap *mapa;
}pop;


///lista duplamente ligada que guarda os macacos ou outras coisas como o mapa e os overlays
typedef struct im {
	int posX;
	int posY;
	unsigned long range;/**< @brief  range do macaco*/
	unsigned long speed_lancar;/**< @brief velocidade de lançamento*/
	unsigned long pierce;/**< @brief quantidade de balões que os projeteis que lançam podem rebentar*/
	unsigned long tempo;/**< @brief guarda o tempo para o macaco*/
	unsigned long aa;/**< @brief determina ao fim de quanto tempo ele vai auto-atacar*/
	unsigned long pops;/**< @brief guarda quantos balões ele já rebentou*/
	monkey_type tipo;/**< @brief tipo de macaco*/
	selecionado select;/**< @brief saber se o utilizador selecionou o macaco*/
	struct im **linha_endereco;/**< @brief guarda uma linha da largura do macaco com os seus endereços*/
	struct im* next;
	struct im* before;
	Bitmap *mapa;/**< @brief bitmapa do macaco*/
	Bitmap *projetil_tipo;/**< @brief bitmapa do projetil*/
}imagem;

///guarda as informações do rato
typedef struct {

	unsigned long posX;
	unsigned long posY;
	imagem *selected_monkey;
	selecionado select;/**< @brief se selecionou algum macaco*/
	monkey_type tipo;/**< @brief tipo de macaco selecionado*/
	right_state rightBotton;/**< @brief estados do botão direito*/
	left_state leftBotton;/**< @brief estados do botão esquerdo*/

}mouse_data;

///lista duplamente ligada com os balões
typedef struct balao {
	int posX;
	int posY;
	unsigned long hp;/**< @brief vida do balão*/
	unsigned long speed;/**< @brief velocidade do balão*/
	struct balao ** linha_endereco;/**< @brief guarda uma linha da largura do balão com os seus endereços*/
	caminho* percurso;/**< @brief guarda o percurso do balão*/
	struct balao* next;
	struct balao* before;
	Bitmap *mapa;
}balao_type;

///lista duplamente ligada com os projeteis
typedef struct dardo {
	int posX;
	int posY;
	unsigned long pierce;/**< @brief quantidade de balões que pode rebentar*/
	unsigned long speedX;/**< @brief velocidade horizontal*/
	unsigned long speedY;/**< @brief velocidade vertical*/
	struct dardo* next;
	struct dardo* before;
	imagem *pertence;//a que macaco pertece o projetil
	Bitmap *mapa;
}projetil;

///guarda o primeiro macaco e o ultimo
typedef struct{
	imagem *primeiraImagem;
	imagem *ultimaImagem;
	int size;
}conjunto;

///guarda o primeiro pop e o ultimo
typedef struct{
	pop *primeiraPop;
	pop *ultimaPop;
	int size;
}conjunto_pops;

///guarda o primeiro balão e o ultimo
typedef struct{
	balao_type *primeiraBalao;
	balao_type *ultimaBalao;
	int size;
}conjunto_baloes;

///guarda o primeiro porjetil e o ultimo
typedef struct{
	projetil *primeiraProjeteis;
	projetil *ultimaProjeteis;
	int size;
}conjunto_projeteis;

/**@}*/


#endif
