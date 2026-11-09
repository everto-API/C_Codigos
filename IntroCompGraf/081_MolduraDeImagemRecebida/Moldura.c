#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int i, j, k, altura, largura;
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

    int imagemNOVA[altura+2][largura+2][3];
    
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

    for (i = 0; i < altura+2; i++)
        for (j = 0; j < largura+2; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);
}