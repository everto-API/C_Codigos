#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
    int i, j, k, altura, largura, novaA, novaL, novoI, novoJ, centroX, centroY;
    double rad, c, s, xNOVO, yNOVO, ci, cj, iCentrado, jCentrado;
    unsigned char cmax = 255, caractere, type;
    FILE *arquivo;

    // Ler arquivo em ASCII ou BINÁRIO
    // Bloco do professor
    arquivo = fopen(argv[1], "r");
    while ((caractere = getc(arquivo)) == '#')
        while((caractere = getc(arquivo)) != '\n');
    ungetc(caractere, arquivo);

    fscanf(arquivo, "P%hhu\n", &type);
    while ((caractere = getc(arquivo)) == '#')
        while((caractere = getc(arquivo)) != '\n');
    ungetc(caractere, arquivo);
  
    fscanf(arquivo, "%u %u\n%hhu\n", &largura, &altura, &cmax);
    unsigned char g[altura][largura][3];
    
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
    
    // Gerar a imagem nova
    // Meu bloco
    novaA = altura*2;
    novaL = largura*2;
    int imagemNOVA[novaA][novaL][3];
    
    centroX = novaA / 2.0;
    centroY = novaL / 2.0;
    ci      = altura / 2.0;
    cj      = largura / 2.0;

    rad = 30 * M_PI / 180;
    c = cos(rad);
    s = sin(rad);

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i][j][k] = 255;
    
    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
            for (k = 0; k < 3; k++) {
                iCentrado = i - ci;
                jCentrado = j - cj;
                
                novoI = round(iCentrado * c - jCentrado * s);
                novoJ = round(iCentrado * s + jCentrado * c);
                imagemNOVA[novoI + centroX][novoJ + centroY][k] = g[i][j][k];}
    
    arquivo = fopen("imagemRotacionada.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", novaL, novaA, cmax);

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
}