#include <stdio.h>
#include <stdlib.h>

// Este programa cria uma imagem de altura e largura definida e após isto ela lê a matriz da imagem criada
// e adiciona uma moldura de 1 único pixel para a altura e largura da imagem.
// A imagem criada: [largura, altura]
// A imagem nova:   [largura+2, altura+2]

int main(char **argv){
    int i, j, k;
    unsigned char cmax = 255;
    FILE *arquivo;
    int altura = 10, largura = 10;
    int g[largura][altura][3];

    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++) {
            if (i == j) {
                g[i][j][0] = 0;
                g[i][j][1] = 0;
                g[i][j][2] = 0;
            } else {
                g[i][j][0] = 255;
                g[i][j][1] = 255;
                g[i][j][2] = 255;}}

    arquivo = fopen("imagem.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura, altura, cmax);

    for (i = 0; i < largura; i++)
        for (j = 0; j < altura; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", g[i][j][k]);
    fclose(arquivo);

    int imagemNOVA[largura+2][altura+2][3];
    
    for (i = 0; i < altura+2; i++)
        for (j = 0; j < largura+2; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i][j][k] = 255;
    
    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i+1][j+1][k] = g[i][j][k];
    
    arquivo = fopen("imagemMoldurada.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura+2, altura+2, cmax);

    for (i = 0; i < largura+2; i++)
        for (j = 0; j < altura+2; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
}