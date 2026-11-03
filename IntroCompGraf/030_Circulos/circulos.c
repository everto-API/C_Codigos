//------------------------
// Universidade Federal de Sergipe
// Departamento de Matemática
// Biblioteta simples para o curso de Introdução à Computação Gráfica
// Desenha círculos aleatórios em uma imagem ppm
// Sintaxe: ./circulos
// Prof Evilson Vieira
// Aracaju-SE 12/12/2023
//------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Essa função desenha um círculo com centro, raio e cor aleatórios
void circ(int l, int h, int r1, int r2, unsigned char (**im)[3]){
    int i, j, I, J, K, L;
    int O[2]={rand()%l, rand()%h}, R=rand()%(r2-r1)+r1; // centro e raio
    unsigned char cor[3]={rand(),rand(),rand()};        // cor

    // Estabelecendo a caixa delimitadora do círculo
    I = (O[0]>R) ? O[0]-R : 0;
    if (O[0]+R<l) l=O[0]+R;

    J = (O[1]>R) ? O[1]-R : 0;
    if (O[1]+R<h) h=O[1]+R;
    // O círculo está na caixa (I,J)--(l,h)

    R*=R;//quadrado do raio
    for(i=I; i<l; i++){
        K=(i-O[0])*(i-O[0]);
        for(j=J; j<h; j++){
            L=K+((j-O[1])*(j-O[1]));
            if(L<R){
                im[i][j][0]=cor[0];
                im[i][j][1]=cor[1];
                im[i][j][2]=cor[2];
            }
        }
    }
}

void main(int argc, char **argv){
    int i, j, k, l, h;
    FILE *file;

    l=600; //largura: número de colunas
    h=400; //altura : número de linhas

    //-----------------------------------------------------
    // Carregando a imagem
    //-----------------------------------------------------
    // Alocanco memória para a imagem
    unsigned char (**imagem)[3];      // ponteiro para um vetor de ponteiros para vetores de tamanho 3 (parecido com a estrutura de matriz 3D)

    j=l*sizeof(char*);
    imagem = malloc(j);               // alocando ponteiros para as colunas: x

    j=h*3;
    for (i=0; i<l; i++)               // alocando ponteiros para cada coordenada da coluna: y
        imagem[i] = malloc(j);
    // Nessa estrutura o píxel imagem[i][j] têm coordenadas (i,j)

    // pintando de branco
    for(i=0; i<l; i++)
        for(j=0; j<h; j++)
            imagem[i][j][0]=imagem[i][j][1]=imagem[i][j][2]=255;
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Desenhando os circulos
    //-----------------------------------------------------
    srand(time(NULL));          // inicializando o gerador randômico
    for(i=0;i<100;i++)          // quantidade de círculos
        circ(l,h,10,50,imagem); // largura, altura, raio menor, raio maior, imagem
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Gravando o arquivo de imagem no formato ppm binário
    //-----------------------------------------------------
    file = fopen("circulos.ppm", "w");
    fprintf(file, "P6\n# Reta no plano - Prof Evilson\n");
    fprintf(file, "%u %u\n255\n", l, h);
    for (j=0;j<h;j++)
        for (i=0;i<l;i++)
            fprintf(file,"%c%c%c", imagem[i][j][0],imagem[i][j][1],imagem[i][j][2]);
    fclose(file);
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Liberando a memória alocada
    //-----------------------------------------------------
    for (i=0; i<l; i++)
        free(imagem[i]);
    free(imagem);
}
