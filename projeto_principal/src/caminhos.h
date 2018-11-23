#ifndef _CAMINHOS_H_
#define _CAMINHOS_H_

/** @defgroup Caminho Caminho
 * @{
 * Funções para criar o percurso dos balões
 */
/**
 * @brief Preenche no mapa das colisão a posição ocupada pelo balão para não poderem ser colocadas torres nessa posição
 *
 * @param board apontador para o board com as colisões
 * @param posX posição onde começar a preencher
 * @param posY posição onde começar a preencher
 * @param nada endereço de um macaco que não existe para ser diferente de NULL
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
