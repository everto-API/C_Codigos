#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv){
  int i, j, l, h, canal;
  float fatorT;
  unsigned char type, cmax, caractere, valor;
  FILE *fp;
  
  fp = fopen(argv[1], "r");
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);
  
  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);
  unsigned char imagemA[h][l][3];

  if(type==3){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%hhu %hhu %hhu", &imagemA[i][j][0],&imagemA[i][j][1],&imagemA[i][j][2]);
    fclose(fp);
  }
  else if(type==6){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%c%c%c", &imagemA[i][j][0],&imagemA[i][j][1],&imagemA[i][j][2]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }
  // AAAAAAAAAAAAAAA

  fp = fopen(argv[2], "r");
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);
  
  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);
  unsigned char imagemB[h][l][3];

  if(type==3){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%hhu %hhu %hhu", &imagemB[i][j][0],&imagemB[i][j][1],&imagemB[i][j][2]);
    fclose(fp);
  }
  else if(type==6){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%c%c%c", &imagemB[i][j][0],&imagemB[i][j][1],&imagemB[i][j][2]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }

  // AAAAAAAAAAAAAAA
  fp = fopen(argv[3], "w");

  fprintf(fp, "P3\n");
  fprintf(fp, "# Criado no curso de Introdução à Computação Gráfica\n");
  fprintf(fp, "# Departamento de Matemática - UFS\n");
  fprintf(fp, "# Prof Evilson\n");
  fprintf(fp, "%d %d\n%hhu\n", l, h, cmax);
  for (i=0; i<h; i++) {
    for (j=0; j<l; j++) {
      if (l > 1)
        fatorT = 1.0f - (float)j / (l - 1);
      else
        fatorT = 1.0f;

      for (canal = 0; canal < 3; canal++) {
        valor = (unsigned char)(
          fatorT * imagemA[i][j][canal] + (1.0f - fatorT) * imagemB[i][j][canal]
        );
        fprintf(fp, "%hhu ", valor);
      }
      fprintf(fp, "\n");
    }
  }
  fclose(fp);
}
