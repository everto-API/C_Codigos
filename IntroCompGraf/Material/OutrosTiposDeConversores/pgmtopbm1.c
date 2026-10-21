//------------------------
// Universidade Federal de Sergipe
// Departamento de Matemática
// Biblioteta simples para o curso de Introdução à Computação Gráfica
// Para converter imagens pgm para pbm tipo P1
// Sintaxe: ./ppmtopgm image.pgm image.pbm
// Prof Evilson Vieira
// Aracaju-SE 23/12/2023
//------------------------

#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv){
  int i, j, k, l, h;
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
  unsigned char g[h][l];

  if(type==2){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%hhd ", &g[i][j]);
    fclose(fp);
  }
  else if(type==5){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        fscanf(fp, "%c", &g[i][j]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }

  k=0;
  for(i=0; i<h;){
    for(j=0; j<l; j++){
      g[i][j]=cmax-g[i][j];
      k+=g[i][j];
      if(k>=cmax){
        g[i][j]=1;
        k-=cmax;
      }
      else g[i][j]=0;
    }
    i++;
    if(i<h){
      j--;
      for(;j>=0; j--){
        g[i][j]=cmax-g[i][j];
        k+=g[i][j];
        if(k>=cmax){
          g[i][j]=1;
          k-=cmax;
        }
        else g[i][j]=0;
      }
      i++;
    }
  }

  fp = fopen(argv[2], "w");

  fprintf(fp, "P1\n");
  fprintf(fp, "# Criado no curso de Introdução à Computação Gráfica\n");
  fprintf(fp, "# Departamento de Matemática - UFS\n");
  fprintf(fp, "# Prof Evilson\n");
  fprintf(fp, "%u %u\n", l, h);

  for (i=0; i<h; i++)
    for (j=0; j<l; j++)
      fprintf(fp, "%hhu ",g[i][j]);
  
  fclose(fp);
}
