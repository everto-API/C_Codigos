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

// argc informa quantos argumentos foram passados; argv guarda esses argumentos.
// Este exemplo espera: nome-do-programa, arquivo.ppm e arquivo.pgm.
// Atenção: main deve retornar int (e aqui argc deveria ser validado antes de
// acessar argv[1] e argv[2]). A forma abaixo foi mantida como no material.
void main(int argc, char **argv){
  // i e j percorrem linhas e colunas; l e h representam largura e altura.
  // type guarda o número do formato (3 ou 6), cmax o maior valor de canal,
  // e caractere é usado para examinar os bytes do cabeçalho.
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  // FILE* é o identificador usado pelas funções de leitura e escrita de arquivos.
  FILE *fp;
  
  // Abre a imagem de entrada. Para arquivos binários, o modo recomendado é "rb".
  // Também seria importante verificar se fopen conseguiu abrir o arquivo.
  fp = fopen(argv[1], "r");

  // No cabeçalho PPM, linhas iniciadas por # são comentários.
  // Este laço pula um comentário inicial, se houver, e devolve o primeiro
  // caractere que não pertence a ele para que fscanf possa lê-lo.
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  // Lê o identificador do formato: o P é literal e %hhu lê o número como
  // unsigned char. Assim, type recebe 3 (PPM textual) ou 6 (PPM binário).
  fscanf(fp, "P%hhu\n", &type);

  // Tenta pular comentário(s) que apareçam depois do identificador do formato.
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);
  
  // Lê largura, altura e valor máximo permitido para cada canal RGB.
  // Observação: %u espera ponteiros para unsigned int, mas l e h são int;
  // essa incompatibilidade deve ser corrigida em uma versão revisada.
  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);

  // Matriz de pixels: h linhas, l colunas e 3 componentes por pixel
  // (índice 0 = vermelho, 1 = verde, 2 = azul). É um array de tamanho variável
  // alocado na pilha; imagens grandes podem exceder o espaço disponível.
  unsigned char p[h][l][3];

  // P3 armazena os canais como números em texto, por exemplo: "255 0 0".
  if(type==3){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        // %hhu converte cada número textual para unsigned char.
        fscanf(fp, "%hhu %hhu %hhu", &p[i][j][0],&p[i][j][1],&p[i][j][2]);
    fclose(fp);
  }
  // P6 armazena os valores dos canais como bytes, sem convertê-los de texto.
  else if(type==6){
    for(i=0; i<h; i++)
      for(j=0; j<l; j++)
        // %c lê um caractere/byte por canal. Esta leitura só cobre amostras
        // de 1 byte (maxval até 255); o modo "rb" e fread seriam mais adequados.
        fscanf(fp, "%c%c%c", &p[i][j][0],&p[i][j][1],&p[i][j][2]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    // exit(0) sinaliza sucesso ao sistema; um erro deveria usar status diferente de 0.
    exit(0);
  }

  // A imagem em tons de cinza precisa de apenas um valor por pixel.
  unsigned char g[h][l];
  for (i=0; i<h; i++)
    for (j=0; j<l; j++)
      // Combina vermelho, verde e azul com pesos de luminância aproximados.
      // O verde pesa mais porque a visão humana é mais sensível a essa componente.
      // Como os pesos somam 1, o resultado fica na faixa dos canais; a conversão
      // para unsigned char descarta a parte fracionária.
      g[i][j]=0.2126*p[i][j][0]+0.7152*p[i][j][1]+0.0722*p[i][j][2];

  // Abre o arquivo de saída. P5 contém pixels binários, então o modo recomendado
  // é "wb"; também convém testar se a abertura foi bem-sucedida.
  fp = fopen(argv[2], "w");

  // Cabeçalho PGM: P5 identifica o formato binário; depois vêm comentários,
  // largura, altura e o valor máximo dos pixels.
  fprintf(fp, "P5\n");
  fprintf(fp, "# Criado no curso de Introdução à Computação Gráfica\n");
  fprintf(fp, "# Departamento de Matemática - UFS\n");
  fprintf(fp, "# Prof Evilson\n");
  fprintf(fp, "%u %u\n%hhu\n", l, h, cmax);
  // Escreve um byte de cinza por pixel, percorrendo a imagem linha a linha.
  // fwrite seria a opção apropriada para gravar dados binários.
  for (i=0; i<h; i++)
    for (j=0; j<l; j++)
      fprintf(fp, "%c", g[i][j]);
  fclose(fp);
}
