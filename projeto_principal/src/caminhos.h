#ifndef _CAMINHOS_H_
#define _CAMINHOS_H_

/** @defgroup Caminho Caminho
 * @{
 * Fun��es para criar o percurso dos bal�es
 */
/**
 * @brief Preenche no mapa das colis�o a posi��o ocupada pelo bal�o para n�o poderem ser colocadas torres nessa posi��o
 *
 * @param board apontador para o board com as colis�es
 * @param posX posi��o onde come�ar a preencher
 * @param posY posi��o onde come�ar a preencher
 * @param nada endere�o de um macaco que n�o existe para ser diferente de NULL
 */
void preencher(imagem **board,int posX,int posY,imagem *nada);
/**
 * @brief cria o caminho para o primeiro mapa
 *
 * @param inicio mapa que vou querer inicialiar
 * @param board para onde vou querer guardar o inicial
 */
void criar_caminho1(caminho *inicio,imagem **board);

/**@}*/
#endif
