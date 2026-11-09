#include <stdio.h>
#include <stdlib.h>

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
    
    novaA = altura*2;
    novaL = largura*2;
    int imagemNOVA[novaA][novaL][3];
    
    centroX = (novaA - altura) / 2;
    centroY = (novaL - largura) / 2;

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i][j][k] = 255;
    
    for (i = 0; i < altura; i++)
        for (j = 0; j < largura; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i+centroX][j+centroY][k] = g[i][j][k];
    
    arquivo = fopen("imagemExpandida.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", novaL, novaA, cmax);

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
}