#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int i, j, altura, largura, filtro;
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
    unsigned char (*g)[largura] = malloc(altura * sizeof(*g));;
    
    if(type == 2){
    for(i = 0; i < altura; i++)
      for(j = 0; j < largura; j++)
        fscanf(arquivo, "%hhu", &g[i][j]);
    fclose(arquivo);
    } else{
        printf("Formato inválido!");
        fclose(arquivo);
        exit(0);}
    
    // A mesma coisa na hora de alocar as matrizes ORIGEM e NOVA, doideira isso daé
    int (*imagemORIGEM)[largura+2] = malloc((altura+2) * sizeof(*imagemORIGEM));
    int (*imagemNOVA)[largura+2]   = malloc((altura+2) * sizeof(*imagemNOVA));
    
    for (i = 0; i < altura+2; i++)
        for (j = 0; j < largura+2; j++){
                imagemORIGEM[i][j] = 255;
                imagemNOVA[i][j] = 255;}
    
    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
                imagemORIGEM[i+1][j+1] = g[i][j];
    
    // Bloco para cobrir a moldura branca da imagem expandida
    // Cantos
    imagemORIGEM[0][0]                = g[0][0];
    imagemORIGEM[0][largura+1]        = g[0][largura-1];
    imagemORIGEM[altura+1][0]         = g[altura-1][0];
    imagemORIGEM[altura+1][largura+1] = g[altura-1][largura-1];
    
    // Bordas horizontais
    for (j = 0; j < largura; j++){
            imagemORIGEM[0][j+1]        = g[0][j];
            imagemORIGEM[altura+1][j+1] = g[altura-1][j];}

    // Bordas verticais
    for (i = 0; i < altura; i++){
            imagemORIGEM[i+1][0]        = g[i][0];
            imagemORIGEM[i+1][largura+1] = g[i][largura-1];}
    
    // Etapa do borrão
    for (i = 1; i <= altura; i++)
        for (j = 1; j <= largura; j++){
                filtro = (imagemORIGEM[i-1][j-1] + imagemORIGEM[i][j-1] + imagemORIGEM[i+1][j-1] + 
                          imagemORIGEM[i-1][j]   + imagemORIGEM[i][j]   + imagemORIGEM[i+1][j]   + 
                          imagemORIGEM[i-1][j+1] + imagemORIGEM[i][j+1] + imagemORIGEM[i+1][j+1]) / 9;
                imagemNOVA[i][j]     = filtro;}
    
    unsigned char (*imagemBORDA)[largura] = malloc(altura * sizeof(*imagemBORDA));

    for (i = 1; i <= altura; i++)
        for (j = 1; j <= largura; j++) {
            int diferenca = imagemORIGEM[i][j] - imagemNOVA[i][j];

            if (diferenca < 0) diferenca = -diferenca; // Def de absoluto

            diferenca = diferenca * 10;
            if (diferenca > 255) diferenca = 255;
            imagemBORDA[i-1][j-1] = (unsigned char)diferenca;}
    
    arquivo = fopen("imagemDetectada.pgm", "w");
    
    fprintf(arquivo, "P2\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura, altura, cmax);

    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
            fprintf(arquivo, "%hhu ", imagemBORDA[i][j]);
    fclose(arquivo);

    free(g);
    free(imagemORIGEM);
    free(imagemNOVA);
    free(imagemBORDA);}