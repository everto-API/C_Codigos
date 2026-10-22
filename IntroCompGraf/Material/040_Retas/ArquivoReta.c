#include <stdio.h>
#include <stdlib.h>

int main(char **argv){
    int i, j, k;
    unsigned char cmax = 255;
    FILE *arquivo;
    int altura = 500, largura = 500;
    int g[largura][altura][3];

    int ponto1[2] = {250, 150};
    int ponto2[2] = {200, 70};
    int x[2] = {ponto1[1] - ponto2[1], ponto2[0] - ponto1[0]};  // vetorPerpendicular
    int a = x[0], b = x[1];

    for (i = 0; i < largura; i++)
        for (j = 0; j < altura; j++)
            if ((a*(j - ponto1[0]) + b*(i - ponto1[1])) > 0) {
                g[i][j][0] = 255;
                g[i][j][1] = 255;
                g[i][j][2] = 255;
            } else {
                g[i][j][0] = 0;
                g[i][j][1] = 0;
                g[i][j][2] = 0;}

    arquivo = fopen("lima.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura, altura, cmax);

    for (i = 0; i < largura; i++)
        for (j = 0; j < altura; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", g[i][j][k]);
    fclose(arquivo);
}