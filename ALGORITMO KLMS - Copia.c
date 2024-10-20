#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "biblioteca_kernel.h"
#define PI 3.1415926536

// FUN��O PARA VERIFICAR A CONSTRU��O DO VETOR
void imprime_teste_2(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%0.7lf",vetor[i]);
        printf("\n");
    }
}

// FUN��O PARA VERIFICAR A CONSTRU��O DO VETOR
void imprime_teste_1(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("valor [%d]: ",i);
        printf("%0.8lf",vetor[i]);
        printf("\n");
    }

    printf("\n\nUltimos valores : \n\n");
}

// FUN��O PARA VERIFICAR A CONSTRU��O DO VETOR
void imprime_teste(int *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%d",vetor[i]);
        printf("\n");
    }

    printf("\n\n");
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Gerador de valores aleat�rios com distribui��o gaussiana
double AWGN_generator()
    {/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

        double temp1;
        double temp2;
        double result;
        int p;

        p = 1;

        while( p > 0 )
        {
            temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
                                                           integer between 0 and  RAND_MAX,
                                                           which is defined in stdlib.h.
                                                       */
            if ( temp2 == 0 )
                {// temp2 is >= (RAND_MAX / 2)
                p = 1;
                }// end if
            else
                {// temp2 is < (RAND_MAX / 2)
                    p = -1;
                }// end else

        }// end while()

        temp1 = cos( ( 2.0 * (double)PI ) * rand() / ( (double)RAND_MAX ) );
        result = sqrt( -2.0 * log( temp2 ) ) * temp1;

        return result;	// return the generated random sample to the caller
}

double *atualizar_dicionario(double *Un, double *dicionario, int n, int M)
{
    for(int i=M-1; i>0; i--)
    {
        dicionario[i] = dicionario[i-1];
    }

    dicionario[0] = Un[n];

    //printf("DICIONARIO:\n");
    //imprime_teste_1(dicionario,M);
    //printf("\n");

    //printf("ENTRADA (UN):\n");
    //imprime_teste_1(Un,10);
    //printf("\n");

    //system("pause");


    return dicionario;
}

double* calculo_sse(double *sse, double *e,int N)
{
    double aux;
    int interacoes = 100;

    for(int i=0;i<N;i++)
    {
        aux = (pow(e[i],2)) / interacoes;
        sse[i] = aux + sse[i];
    }

    return sse;
}


//FUN��O QUE CALCULA UM NOVO VALOR PARA O COEFICIENTE Wn
double *calculo_Wn(double *Wn,double passo,double *kw,double e,int M)
{
    double aux, resultado;

    for(int i=0; i<M; i++)
    {
        aux = 2 * passo * kw[i] * e;
        resultado = Wn[i] - aux;
        Wn[i] = resultado;
    }

    return Wn;
}

// FUN��O QUE CALCULA O ESCALAR d
double calculo_d(double *Yn, double *Zn, int n)
{
    double d = 0;

    d = Yn[n] + Zn[n];

    return d;
}

// FUN��O QUE CALCULA O ESCALAR dw
double calculo_dw(double *Wn, double *kw, int tamanho)
{
    int j,i;
    double dw = 0;
    double multiplicacao = 0;

    // REALIZANDO UM PRODUTO DE VETORES QUE RESULTA NUM ESCALAR Dw
    for(j=0; j<tamanho; j++)
    {
        for(i=0; i<tamanho; i++)
        {
            multiplicacao = kw[j] * Wn[i];
            dw = dw + multiplicacao;
        }
    }

    return dw;
}


// FUN��O QUE CALCULA O kw(n)
double *calculo_kw(double *kw, double *vetor_Un, double *vetor_Dn, int n,int M)
{
    double eta = 0.04;

    for(int i=0; i<M; i++)
    {
        kw[i]= kernel_gaussiano(vetor_Un[n],vetor_Dn[i],eta);
    }

    return kw;
}

// FUN��O QUE GERA O VETOR INICIAL DE COEFICIENTES COM 0 NAS POSI��ES
double * cria_vetor_coeficiente(double *vetor, int tamanho)
{
    vetor = (double *)malloc(tamanho * sizeof(double));

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i]= 0;
    }

    return vetor;

}

// GERANDO O VETOR DE RU�DO COM VALORES ALEAT�RIOS E PEQUENOS
double * cria_vetor_ruido(double *vetor, int seed, int tamanho)
{
    double valor;

    // Alocando vetor
    vetor = (double *)malloc(tamanho * sizeof(double));

    // Gernando valores aleat�rios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = AWGN_generator();
        vetor[i]= valor/100000.0;
    }

    return vetor;

}

// FUN��O QUE ATRIBUI OS VALORES DE y(n)
double * sinal_desejado(double *vetor_Un, double *vetor_Yn, int tamanho)
{
    // Alocando Dinamicamente
    vetor_Yn = (double *)malloc(tamanho * sizeof(double));

    // Elevando o Euler (e) ao valor de vetor de entrada U(n) e "jogando" em y(n)
    for (int i = 0; i < tamanho; i++)
    {
        vetor_Yn[i] = exp(vetor_Un[i]);
    }

    return vetor_Yn;
}

// FUN��O QUE CRIA VETOR DE ENTRADA COM VALORES ALEAT�RIOS
int* cria_vetor_interio(int *vetor, int seed, int tamanho)
{
    vetor = (int *)malloc(tamanho * sizeof(int));

    // Gernando valores aleat�rios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i]= rand() % 31; // Garantindo que ser�o valores aleatorios sejam de -1 a 1
    }

    return vetor;
}

// FUN��O QUE CRIA VETOR DE ENTRADA COM VALORES ALEAT�RIOS
double* cria_vetor_entrada(double *vetor, int seed, int tamanho)
{
    double valor;
    FILE *arquivo = fopen("vetor_entrada.csv", "w");

    vetor = (double *)malloc(tamanho * sizeof(double));

    // Gernando valores aleat�rios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = AWGN_generator();
        valor = valor / 1000;
        vetor[i]= valor;
    }

    for (int i = 0; i < tamanho; i++)
    {
        fprintf(arquivo, "%lf\n", vetor[i]);             // Escrevendo valores SSE no arquivo
    }

    return vetor;
}

// FUN��O PRINCIPAL
int main ()
{
    // Declarando Vari�veis

    int M = 10;       // Tamanho de Vetor
    int N = 10000;       // Numero de amostras - testando em um valor baixo

    double *dicionario;  // Vetor Dicion�rio
    double *Un;          // Vetor de entrada
    double *Yn; // Vetor Exponencial
    double *Zn; // Vetor dos Ru�dos
    double *Wn; // Vetor Coeficientes
    double *kw; // Vetor Kernel
    double dw;  // Valor escalar da minha sa�da estimada
    double d;   // Valor escalar desejado num instante n
    double e = 0;
    double passo = 0.01;
    double *sse;
    double MSE;
    double *salva_resultado;



    sse = cria_vetor_coeficiente(sse,N);

    FILE *arquivo = fopen("valores.csv", "w");

    for(int m=0; m<100; m++)
    {
        Un = cria_vetor_entrada(Un,10,N);                   // Criando o vetor de entrada com valores aleat�rios
        Yn = sinal_desejado(Un, Yn, N);               // Realizando a oper��o Exponencial y(n)
        Zn = cria_vetor_ruido(Zn,1,N);                      // Criando o vetor com os dados de ruido 10^(-5)
        dicionario = cria_vetor_entrada(dicionario,2,M);    // Criando o Vetor Dicion�rio
        Wn = cria_vetor_coeficiente(Wn,M);                  // Criando Vetor de coeficientes inicializados com 0
        salva_resultado = cria_vetor_coeficiente(salva_resultado,N);


        for(int n=0; n<N; n++)
        {
            kw = cria_vetor_coeficiente(kw,M);      // Criando o vetor Kernel inicialmente zerado
            kw = calculo_kw(kw,Un,dicionario,n,M);  // Calculando todos os valores de Kernel atrav�s da intera��o dos valores de entrada e dicion�rio
            dw = calculo_dw(Wn, kw, M);             // Calculando o valor desejado Dw atrav�s de um produto escalar entre vetores
            d = calculo_d(Yn,Zn,n);                 // Calculando o valor de d
            e = dw - d;                             // Calculando o erro
            salva_resultado[n] = e;
            Wn = calculo_Wn(Wn,passo,kw,e,M);       // Atualizando os valores do vetor coeficiente
            dicionario = atualizar_dicionario(Un,dicionario,n,M); // Atualizando os valores do dicionario com os valores de entrada
            free(kw);

            /*
            if((m == 0 && n==0) || (n==9999 && m == 1))
            {
                printf("COEFICIENTE:\n");
                imprime_teste_1(Wn,M);
                printf("\n");

                printf("e2:\n");
                //printf("%lf",sse);
                printf("\n\n");
            }
            */
        }

        sse = calculo_sse(sse,salva_resultado,N);

        for (int i = 0; i < N; i++)
        {
            fprintf(arquivo, "%lf\n", sse[i]);             // Escrevendo valores SSE no arquivo
        }
    }

    fclose(arquivo);

    printf("ENTRADA (UN):\n");
    //imprime_teste_1(Un,N);
    printf("\n");

    printf("Y(N):\n");
    //imprime_teste_1(Yn,N);
    printf("\n");

    printf("RUIDO:\n");
    //imprime_teste_2(Zn,N);
    printf("\n");

    printf("DICIONARIO:\n");
    //imprime_teste_1(dicionario,M);
    printf("\n");

    printf("COEFICIENTE:\n");
    //imprime_teste_1(Wn,M);
    printf("\n");

    printf("KERNEL:\n");
    imprime_teste_1(kw,M);
    printf("\n");

    printf("Dw:\n");
    //printf("%lf",dw);
    printf("\n");

    printf("D:\n");
    //printf("%0.7lf",d);
    printf("\n");

    //printf("e:\n");
    //printf("%lf",e);
    //printf("\n");

    //printf("e2:\n");
    //printf("%lf",e2);
    //printf("\n");

    printf("TODOS OS VALORES DE E:\n");
    //imprime_teste_1(salva_resultado,N);
    printf("\n");

    printf("VETOR SSE:\n");
    //imprime_teste_1(sse,N);
    printf("\n");

    free(Un);
    free(Yn);
    free(Zn);
    free(dicionario);
    free(Wn);
    free(salva_resultado);
}
