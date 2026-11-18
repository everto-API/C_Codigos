#include <stdio.h>
#include <stdlib.h>

/*
    O objetivo do programa é esticar uma ponta de uma imagem dada. Isto se da por
    meio de Transformações Projetivas, o conceito será seguido através do PDF entregue
    pelo professor durante o curso de Introdução à Computação Gráfica (2026.2)

    Envolve coordenadas homogêneas:
        Um ponto (x, y) vira um vetor (x, y, 1) e aplicamos uma transformação T nela
        T * (x, y, 1) = (x', y', w') e depois (x'/w', y'/w')

    Pegamos quatro pontos (u1, u2, u3, u4) e mais quatro pontos (v1, v2, v3, v4)
    Daí:
        T * u1 = v1;
        (...)
        T * u4 = v4;
    
    Onde:
        u4 = a1 * u1 + a2 * u2 + a3 * u3
        v4 = b1 * v1 + b2 * v2 + b3 * v3

    A ideia é...
        1) A = [u1, u2, u3] e B = [v1, v2, v3]; (vetores em colunas)
        2) a = A⁻¹ * u4     e b = B⁻¹ * v4
        3) C = [(b1/a1) * v1, (b2/a2) * v2, (b3/a3) * v3]
        4) T = C * A⁻¹
*/

// Esta função calcula o determinante de uma matriz 3 x 3
// (Qual o objetivo?) | A fórmula da matriz inversa divide pelo determinante
double detTres(double m[3][3]){
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
         - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
         + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);}

// Esta função serve para calcular a matriz inversa
// Matriz inversa = (matrizAdjunta) / determinante
// Este trambolho já faz o trabalho de descobrir a matriz dos Cofatores e a matriz adjunta
// E ainda entrega a matriz inversa já, o negócio é brabo
void inversaTres(double m[3][3], double inv[3][3]){
    double d = detTres(m);
    double invDet = 1 / d;

    inv[0][0] =  (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    inv[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet;
    inv[0][2] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
    inv[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * invDet;
    inv[1][1] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    inv[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet;
    inv[2][0] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    inv[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet;
    inv[2][2] =  (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;}

// Calcula r = M * v
// matriz * vetor
void MatrizPorVetor(double m[3][3], double v[3], double r[3]){
    double t[3] = {0};

    for (int i = 0; i < 3; i++)     // para cada linha da matriz...
        for (int j = 0; j < 3; j++) // ...soma linha[i] * vetor[j]
            t[i] += m[i][j] * v[j];
    
    for (int i = 0; i < 3; i++)
        r[i] = t[i];}

// Calcula A * B
void multiplicarMatrizes(double A[3][3], double B[3][3], double resultado[3][3]) {
    double matrizTemp[3][3] = {0};

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                matrizTemp[i][j] += A[i][k] * B[k][j];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            resultado[i][j] = matrizTemp[i][j];}

/* 
    Aqui o negócio já fica extremamente confuso - Implementa o método do PDF

    Recebe os 4 pontos (u) e (v) e devolve em T
    (T) u1 -> v1, u2 -> v2, (...)


*/
void matrizProjetiva(double u[4][2], double v[4][2], double T[3][3]){
    double A[3][3], B[3][3], invA[3][3], invB[3][3], C[3][3];

    double u4[3] = {u[3][0], u[3][1], 1};
    double v4[3] = {v[3][0], v[3][1], 1};

    double a[3], b[3];
    
    // 1º Passo: montar A = [u1, u2, u3] e B = [v1, v2, v3]
    for (int i = 0; i < 3; i++) {
        A[0][i] = u[i][0];  
        A[1][i] = u[i][1];  
        A[2][i] = 1;

        B[0][i] = v[i][0];  
        B[1][i] = v[i][1];  
        B[2][i] = 1;}
    
    // 2º Passo: inverter A e B
    inversaTres(A, invA);
    inversaTres(B, invB);
    
    // 3º Ṕasso: Descobrir o quarto ponto (u4 e v4) usando os 3 primeiros pontos
    // Como A*a = u4, temos que a = A⁻¹ * u4
    MatrizPorVetor(invA, u4, a);
    MatrizPorVetor(invB, v4, b);
    
    // 4º Passo: montar C, cuja coluna i é (bi / ai) * (coluna i de B)
    for (int i = 0; i < 3; i++)
        for (int r = 0; r < 3; r++)
            C[r][i] = (b[i] / a[i]) * B[r][i];
    
    // 5º Passo: C * A⁻¹ = T
    multiplicarMatrizes(C, invA, T);}
    



int main(int argc, char **argv){
    int i, j, k, altura, largura, novaA, novaL;
    unsigned char cmax = 255, caractere, type;
    FILE *arquivo;

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

    unsigned char (*g)[largura][3] = malloc(altura * sizeof(*g));

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
    
    // Bloco construído por mim heheheeheh
    novaA = altura*2;
    novaL = largura*2;
    
    double origem[4][2] = {
        {0, 0},          {largura - 1, 0},
        {0, altura - 1}, {largura - 1, altura - 1}};
    
    double destino[4][2] = {
        {0, 0},          {largura - 1, 0},
        {0, altura - 1}, {(largura - 1) * 1.5, (altura - 1) * 1.5}};
    
    int (*imagemNOVA)[novaL][3] = malloc(novaA * sizeof(*imagemNOVA));
    

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                imagemNOVA[i][j][k] = 255;
    
    double matrizF[3][3];
    matrizProjetiva(destino, origem, matrizF);

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
                    imagemNOVA[i][j][k] = g[yi][xi][k];}
    
    arquivo = fopen("imagemExpandida.ppm", "w");
    
    fprintf(arquivo, "P3\n");
    fprintf(arquivo, "%u %u\n%hhu\n", novaL, novaA, cmax);

    for (i = 0; i < novaA; i++)
        for (j = 0; j < novaL; j++)
            for (k = 0; k < 3; k++)
                fprintf(arquivo, "%hhu ", imagemNOVA[i][j][k]);
    fclose(arquivo);

    free(g);
    free(imagemNOVA);}