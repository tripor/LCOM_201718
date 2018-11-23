#ifndef _LOWPROGRAM_H_
#define _LOWPROGRAM_H_
/** @defgroup lowProgram lowProgram
 * @{
 *
 * Fun��es relacionadas com o funcionamento do programa
 */
#include "data_storage.h"
/**
 * @brief guardar o path do programa para as imagens
 *
 * @param path path do programa
 */
void guardar_path(char *path);
/**
 * @brief verifica se h� bal�es no ecr�
 *
 * @return 1 se houver bal�es, 0 caso n�o haja
 */
int verificar_baloes();
/**
 * @brief coloca 2 pixeis a preto no ecra, na posi��o indicada
 *
 * @param posX posicao do pixel
 * @param posY posicao do pixel
 * @param buffer buffer do jogo
 */
void putpixel(int posX,int posY,unsigned char buffer[]);
/**
 * @brief https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 * @param x0 centro em x0
 * @param y0 centro em y0
 * @param buffer buffer do jogo
 */
void drawcircle(int x0, int y0, int radius,unsigned char buffer[]);
/**
 * @brief https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
 *
 * @param x0 coordenada inicial
 * @param y0 coordenada inicial
 * @param x1 coordenada final
 * @param y1 coordenada final
 */
void line(int x0, int y0, int x1, int y1);
/**
 * @brief fun��o que avan�a os bal�es
 *
 */
void avancar();
/**
 * @brief fun��o que faz acan�ar os projeteis
 *
 * @param buffer buffer do jogo
 * @param colisao mapa das colisoes
 * @param vazio mapa de colisoes sem nada
 */
void avancar_projeteis(unsigned char buffer[], balao_type **colisao,balao_type **vazio);
/**
 * @brief apaga um pop, desliga-o do anterior e liga-o ao proximo e depois faz free
 *
 * @param delete pop para fazer delete
 */
void delete_pop(pop* delete);
/**
 * @brief apaga um macaco, desliga-o do anterior e liga-o ao proximo e depois faz free
 *
 * @param delete macaco para fazer delete
 */
void delete_monkey(imagem* deleted);
/**
 * @brief apaga um balao, desliga-o do anterior e liga-o ao proximo e depois faz free
 *
 * @param delete balao para fazer delete
 */
void delete_balao(balao_type* deleted);
/**
 * @brief apaga um projetil, desliga-o do anterior e liga-o ao proximo e depois faz free
 *
 * @param delete projetil para fazer delete
 */
void delete_projetil(projetil* deleted);
/**
 * @brief adiciona duas strings a uma
 *
 * @param s1 string 1
 * @param s2 string 2
 * @return as duas strings juntas
 */
char* add_strings(const char* s1, const char* s2);
/**
 * @brief adiciona um pop a lista
 *
 * @param nom dados da imagem
 * @param posX posi��o do pop
 * @param posY posi��o do pop
 */
void add_pop(Bitmap* nom,int posX, int posY);
/**
 * @brief adiciona um bal�o a lista ,ligando-o ao ultimo
 *
 * @param nom dados da imagem
 * @param hp vida do bal�o
 * @param speed velociadade do bal�o
 * @param inicio percurso que o balao vai percurrer, primeiro ponto onde vai come�ar
 */
void add_balao(Bitmap* nom, unsigned long hp, unsigned long speed,caminho *inicio);
/**
 * @brief adiciona um macaco a lista ,ligando-o ao ultimo
 *
 * @param nom dados com a informa��o da imagem
 * @param tipo dados com a informa��o do projetil que vai lan�ar
 * @param posX posicao do macaco
 * @param posY posicao do macaco
 * @param range distancia de visao do macaco
 * @param speed_lancar velocidade de lancamento do projetil
 * @param pierce quantidade de baloes que o projetil vai poder rebentar
 * @param aa tempo entre cada ataque
 * @param type tipo de macaco
 */
void add_monkey(Bitmap* nom, Bitmap* tipo,int posX, int posY,unsigned long range,unsigned long speed_lancar,unsigned long pierce,unsigned long aa,monkey_type type);
/**
 * @brief adiciona um projetil a lista ,ligando-o ao ultimo
 *
 * @param nom dados com a informacao da imagem
 * @param posX posicao do projetil
 * @param posY posicao do projetil
 * @param pierce quantidades de baloes que o projetil pode rebentar
 * @param speedX velocidade horizontal
 * @param speedY velocidade vertical
 * @param pertence macaco a qual pertence o projetil
 */
void add_projetil(Bitmap* nom , int posX, int posY, unsigned long pierce,long speedX, long speedY,imagem *pertence);
/**
 * @brief faz load a um bmp. numa linha do bmp ignora o branco e guarda tudo o que seja diferente de branco. guarda a posi��o de come�o do que n�o � branco e guarda o tamanho de quanto imprimir e os valores e faz isto para todas as linhas
 *
 * @param tipo_objeto da listas de imagens para fazer load
 * @return struct bitmap com as informa��es necessarias
 */
Bitmap* load_imagem(char* tipo_objeto);
/**
 * @brief trata do disparo do macaco
 *
 * @param macaco macaco que vai ser tratado o disparo
 */
void disparo(imagem *macaco);
/**
 * @brief atualiza o ecra com o buffer e limpa o buffer
 *
 * @param video_mem endere�o com a posicao de memoria do ecra
 * @param buffer buffer com as imagens para passar para o ecra
 * @param inicial buffer com as imagens base
 */
void update_ecra(unsigned char *video_mem, unsigned char buffer[],unsigned char inicial[]);
/**
 * @brief verifica se n�o existe nada no mapa para poder ser colocado o macaco
 *
 * @param posX posicao onde quero fazer o teste
 * @param posY posicao onde quero fazer o teste
 * @param board mapa com as posicoes dos macacos e onde andam os baloes
 * @param nom dados com a imagem
 * @return Non NULL pointer to the image buffer
 */
int testar_board(int posX,int posY,imagem **board,Bitmap* nom);
/**
 * @brief volta a reescrever o mapa
 *
 * @param board para onde eu vou querer atualizar o mapa
 * @param board_empty mapa sem nada
 */
void reescrever_board(imagem **board,imagem **board_empty);
/**
 * @brief fun��o que desenha uma imagem no buffer
 *
 * @param buffer buffer para onde eu quero desenhar as imagens
 * @param desenho imagem que quero desenhar
 */
void desenhar_imagem(unsigned char buffer[],imagem *desenho);
/**
 * @brief imprime um macaco no ecra mas os limites correspondem aos limites do mapa e n�o do ecra. � praticamente igual � de cima
 *
 * @param buffer buffer para onde eu quero desenhar as imagens
 * @param desenho imagem que quero desenhar
 */
void desenhar_macaco(unsigned char buffer[],imagem *desenho);
/**
 * @brief desenha a mesa do mapa com os endere�os do bal�o para depois calcular a colis�o
 *
 * @param colisao mapa de colis�es
 * @return Non NULL pointer to the image buffer
 */
void desenhar_balao_colisao(balao_type **colisao);
/**
 * @brief tal como o desenhar macaco, desenha o bal�o  mas ao mesmo tempo atualiza o mapa das colis�es
 *
 * @param buffer buffer para onde eu quero desenhar as imagens
 * @param desenho imagem que quero desenhar
 * @param colisao mapa de colisoes
 */
void desenhar_balao(unsigned char buffer[], balao_type *desenho,balao_type **colisao);
/**
 * @brief tal como o desenhar macaco, mas neste caso para desenhar projeteis
 *
 * @param buffer buffer para onde eu quero desenhar as imagens
 * @param desenho imagem que quero desenhar
 */
void desenhar_projetil(unsigned char buffer[], projetil *desenho);
/**
 * @brief tal como o desenhar macaco mas restringe a existencia dele a um unica frame sendo feito delete
 *
 * @param buffer buffer para onde eu quero desenhar as imagens
 * @param desenho imagem que quero desenhar
 */
void desenhar_pop(unsigned char buffer[], pop *desenho);
/**
 * @brief func�o que chama a impress�o de cada bal�o
 *
 * @param buffer buffer para onde desenhar os baloes
 * @param colisao mapa de colisoes
 */
void desenhar_baloes(unsigned char buffer[],balao_type **colisao);
/**
 * @brief fun��o que chama a impress�o de todos os objetos no ecr�
 *
 * @param buffer buffer para onde desenhar os objetos
 * @param colisao mapa de colisoes
 */
void desenhar_objeto(unsigned char buffer[], balao_type **colisao);
/**
 * @brief fun��o que desenha o overlay do jogo
 *
 * @param buffer buffer para onde desenhar o overlay
 * @param novo imagem do overlay
 */
void desenhar_overlay(unsigned char buffer[], imagem *novo);
/**
 * @brief fun��o que desenha o rato no ecra
 *
 * @param buffer buffer para onde desenhar o rato
 * @param dados informacoes do rato
 * @param novo imagem do rato
 */
void desenhar_rato(unsigned char buffer[], mouse_data dados, imagem *novo);
/**
 * @brief retira a sele��o do macaco
 *
 */
void unselect_monkey();
/**
 * @brief inicializa os varios mapas a serem usados, bem como as variaveis que guardam os objetos
 *
 * @param buffer buffer para ser inicializado
 * @param inicial buffer para ser inicializado
 * @param vazio mapa de colisoes para ser inicializado
 * @param board mapa com os macacos para ser iniciaizado
 */
void init(unsigned char buffer[], unsigned char inicial[],balao_type **colisao,imagem **board);

/**@}*/
#endif
