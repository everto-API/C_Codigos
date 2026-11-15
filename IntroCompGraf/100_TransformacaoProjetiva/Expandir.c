#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void multiplicarMatrizes(double A[3][3], double B[3][3], double resultado[3][3]) {
    double matrizTemp[3][3] = {0};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                matrizTemp[i][j] += A[i][k] * B[k][j];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            resultado[i][j] = matrizTemp[i][j];}

int main(int argc, char **argv){
    int i, j, k, altura, largura, novaA, novaL, centroX, centroY;
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

    // Aqui é onde o filho chora e a mãe também =D
    unsigned char (*g)[largura][3] = malloc(altura * sizeof(*g));
    novaA = altura*2;
    novaL = largura*2;
    
    centroX = (novaA - altura) / 2;
    centroY = (novaL - largura) / 2;

    // Bloco das matrizes para a Transformação Projetiva
    // 1. Matriz Translação
    // 2. Matriz Rotação
    // 3. Matriz Perspectiva    

    double matrizT[3][3] = {
        {1, 0, -centroX + 75},
        {0, 1, -centroY},
        {0, 0, 1}};
    
    double angulo = 15 * M_PI/180;
    double matrizRot[3][3] = {
        {cos(angulo),   -sin(angulo),   0},
        {sin(angulo),   cos(angulo),    0},
        {0,             0,              1}};

    double matrizPer[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {-0.005, -0.001, 1}};
    
    double matrizF[3][3];
    multiplicarMatrizes(matrizPer, matrizRot, matrizF);
    multiplicarMatrizes(matrizF, matrizT, matrizF);
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
    
    int (*imagemNOVA)[novaL][3] = malloc(novaA * sizeof(*imagemNOVA));
    
    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i][j][k] = 255;
    
    // Mapeamento reverso
    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++) {
            double x = j, y = i;
            double w  = matrizF[2][0]*x + matrizF[2][1]*y + matrizF[2][2];
            double xO = (matrizF[0][0]*x + matrizF[0][1]*y + matrizF[0][2]) / w;
            double yO = (matrizF[1][0]*x + matrizF[1][1]*y + matrizF[1][2]) / w;

            int xi = (int)xO;
            int yi = (int)yO;

            if (xi >= 0 && xi < largura && yi >= 0 && yi < altura)
                for (k = 0; k < 3; k++)
                    imagemNOVA[i][j][k] = g[yi][xi][k];
        }
    
    arquivo = fopen("imagemExpandida.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", novaL, novaA, cmax);

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
    free(g);
    free(imagemNOVA);
}