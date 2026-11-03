#include <stdio.h>
#include <stdlib.h>

int main(char **argv){
    int i, j, k;
    unsigned char cmax = 255;
    FILE *arquivo;
    int altura = 500, largura = 500;
    int g[largura][altura][3];

    // Pontos para as retas
    int ponto1[2] = {250, 100};
    int ponto2[2] = {120, 350};
    int ponto3[2] = {400, 380};

    // Primeira Reta
    int x[2] = {ponto1[1] - ponto2[1], ponto2[0] - ponto1[0]};  // vetorPerpendicular
    int x1 = x[0], x2 = x[1];
    
    // Segunda Reta
    int y[2] = {ponto2[1] - ponto3[1], ponto3[0] - ponto2[0]};
    int y1 = y[0], y2 = y[1];
    
    // Terceira Reta
    int z[2] = {ponto3[1] - ponto1[1], ponto1[0] - ponto3[0]};
    int z1 = z[0], z2 = z[1];

    for (i = 0; i < largura; i++)
        for (j = 0; j < altura; j++) {
            int AB = (x1*(i - ponto1[0]) + x2*(j - ponto1[1]));
            int BC = (y1*(i - ponto2[0]) + y2*(j - ponto2[1]));
            int CA = (z1*(i - ponto3[0]) + z2*(j - ponto3[1]));
            
            if (AB > 0 && BC > 0 && CA > 0 || AB < 0 && BC < 0 && CA < 0) {
                g[i][j][0] = 0;
                g[i][j][1] = 0;
                g[i][j][2] = 255;
            } else {
                g[i][j][0] = 255;
                g[i][j][1] = 255;
                g[i][j][2] = 255;
            }
        }

    arquivo = fopen("gustavo.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", largura, altura, cmax);

    for (i = 0; i < largura; i++)
        for (j = 0; j < altura; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", g[i][j][k]);
    fclose(arquivo);
}