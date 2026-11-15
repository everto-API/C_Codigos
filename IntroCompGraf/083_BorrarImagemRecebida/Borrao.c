#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int i, j, k, altura, largura, filtro;
    unsigned char cmax = 255, caractere, type;
    FILE *arquivo;

    arquivo = fopen(argv[1], "r");
    while ((caractere = getc(arquivo)) == '#')
        while((caractere = getc(arquivo)) != '\n');
    ungetc(caractere, arquivo);

    fscanf(arquivo, "P%hhu\n", &type);
    while ((caractere = getc(arquivo)) == '#')
        while((caractere = getc(arquivo)) != '\n');
    ungetc(caractere, arquivo);
  
    fscanf(arquivo, "%u %u\n%hhu\n", &largura, &altura, &cmax);

    // Aparentemente o código envolvia muitas matrizes, daí da forma commum de se escrever a matriz
    // unsigned char g[largura][altura][3] ocupava muito espaço na memória; O ideal seria dessa forma usando o malloc
    unsigned char (*g)[largura][3] = malloc(altura * sizeof(*g));
    
    if(type == 3){
    for(i = 0; i < altura; i++)
      for(j = 0; j < largura; j++)
        fscanf(arquivo, "%hhu %hhu %hhu", &g[i][j][0],&g[i][j][1],&g[i][j][2]);
    fclose(arquivo);
    
    } else if(type == 6){
        for(i = 0; i < altura; i++)
            for(j = 0; j < largura; j++)
                fscanf(arquivo, "%c%c%c", &g[i][j][0],&g[i][j][1],&g[i][j][2]);
        fclose(arquivo);
    } else{
        printf("Formato inválido!");
        fclose(arquivo);
        exit(0);}
    
    // A mesma coisa na hora de alocar as matrizes ORIGEM e NOVA, doideira isso daé
    int (*imagemORIGEM)[largura+2][3] = malloc((altura+2) * sizeof(*imagemORIGEM));
    int (*imagemNOVA)[largura+2][3]   = malloc((altura+2) * sizeof(*imagemNOVA));
    
    for (i = 0; i < altura+2; i++)
        for (j = 0; j < largura+2; j++)
            for (k = 0; k < 3; k++) {
                imagemORIGEM[i][j][k] = 255;
                imagemNOVA[i][j][k] = 255;}
    
    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
            for (k = 0; k < 3; k++)
                imagemORIGEM[i+1][j+1][k] = g[i][j][k];
    
    // Bloco para cobrir a moldura branca da imagem expandida
    // Cantos
    for (k = 0; k < 3; k++) {
        imagemORIGEM[0][0][k]                = g[0][0][k];
        imagemORIGEM[0][largura+1][k]        = g[0][largura-1][k];
        imagemORIGEM[altura+1][0][k]         = g[altura-1][0][k];
        imagemORIGEM[altura+1][largura+1][k] = g[altura-1][largura-1][k];}
    
    // Bordas horizontais
    for (j = 0; j < largura; j++)
        for (k = 0; k < 3; k++) {
            imagemORIGEM[0][j+1][k]        = g[0][j][k];
            imagemORIGEM[altura+1][j+1][k] = g[altura-1][j][k];}

    // Bordas verticais
    for (i = 0; i < altura; i++)
        for (k = 0; k < 3; k++) {
            imagemORIGEM[i+1][0][k]        = g[i][0][k];
            imagemORIGEM[i+1][largura+1][k] = g[i][largura-1][k];}
    
    // Etapa do borrão
    for (i = 1; i <= altura; i++)
        for (j = 1; j <= largura; j++)
            for (k = 0; k < 3; k++) {
                filtro = (imagemORIGEM[i-1][j-1][k] + imagemORIGEM[i][j-1][k] + imagemORIGEM[i+1][j-1][k] + 
                          imagemORIGEM[i-1][j][k]   + imagemORIGEM[i][j][k]   + imagemORIGEM[i+1][j][k]   + 
                          imagemORIGEM[i-1][j+1][k] + imagemORIGEM[i][j+1][k] + imagemORIGEM[i+1][j+1][k]) / 9;
                imagemNOVA[i][j][k]     = filtro;}
    
    arquivo = fopen("imagemBorrada.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura, altura, cmax);

    for (i = 1; i <= altura; i++)
        for (j = 1; j <= largura; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
    free(g);
    free(imagemORIGEM);
    free(imagemNOVA);}