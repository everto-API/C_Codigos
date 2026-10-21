//------------------------
// Universidade Federal de Sergipe
// Departamento de Matemática
// Biblioteta simples para o curso de Introdução à Computação Gráfica
// Para converter imagens ppm para pgm
// Sintaxe: ./ppmtopgm image.ppm image.pgm
// Prof Evilson Vieira
// Aracaju-SE 06/12/2023
//------------------------
#include <stdio.h>
#include <stdlib.h>
void main(int argc, char **argv){
  int i, j, l, h;
  unsigned char type, cmax, caractere;
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
  unsigned char p[h][l][3];

  if(type==3){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%hhu %hhu %hhu", &p[i][j][0],&p[i][j][1],&p[i][j][2]);
    fclose(fp);
  }
  else if(type==6){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%c%c%c", &p[i][j][0],&p[i][j][1],&p[i][j][2]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }
  fp = fopen(argv[2], "w");

  fprintf(fp, "P3\n");
  fprintf(fp, "# Criado no curso de Introdução à Computação Gráfica\n");
  fprintf(fp, "# Departamento de Matemática - UFS\n");
  fprintf(fp, "# Prof Evilson\n");
  fprintf(fp, "%d %d\n%hhu\n", l, h, cmax);
  for (i=0; i<h; i++)
    for (j=0; j<l; j++)
      fprintf(fp, "%hhu %hhu %hhu\n", p[i][j][0], p[i][j][1], p[i][j][2]);
  fclose(fp);
}
