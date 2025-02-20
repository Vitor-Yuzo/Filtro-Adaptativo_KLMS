#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "biblioteca_kernel.h"
#define PI 3.1415926536

// FUNCAO PARA VERIFICAR A CONSTRU��O DO VETOR
void imprime_teste_2(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%0.7lf",vetor[i]);
        printf("\n");
    }
}

// FUNCAO PARA VERIFICAR A CONSTRU��O DO VETOR
void imprime_teste_1(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("valor [%d]: ",i);
        printf("%0.8lf",vetor[i]);
        printf("\n");
    }
}

// FUNCAO PARA VERIFICAR A CONSTRU��O DO VETOR
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

// Gerador de valores aleatorios com distribuicao gaussiana
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
    int realizacoes = 5000;

    for(int i=0;i<N;i++)
    {
        aux = (pow(e[i],2)) / realizacoes;
        sse[i] = aux + sse[i];
    }

    return sse;
}


//FUNCAO QUE CALCULA UM NOVO VALOR PARA O COEFICIENTE Wn
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

// FUNCAO QUE CALCULA O ESCALAR d
double calculo_d(double *Yn, double *Zn, int n)
{
    double d = 0;

    d = Yn[n] + Zn[n];

    return d;
}

// FUNCAO QUE CALCULA O ESCALAR dw
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


// FUNCAO QUE CALCULA O kw(n)
double *calculo_kw(double *kw, double *vetor_Un, double *vetor_Dn, int n, int M, double eta)
{
    double elemento_entrada[1];
    double elemento_dicionario[1];
    double *elemento_kernel;
    int tamanho = 1;

    //FILE *arquivo = fopen("valores_kernel.csv", "a");

    elemento_kernel = (double *)malloc(tamanho * sizeof(double));

    elemento_entrada[0] = vetor_Un[n];

    for(int i=0; i<M; i++)
    {
        elemento_dicionario[0] = vetor_Dn[i];
        elemento_kernel = kernel_gaussiano(elemento_entrada,elemento_dicionario,eta,tamanho);
        kw[i] = elemento_kernel[0];
    }

    free(elemento_kernel);

    return kw;
}

// FUNCAO QUE GERA O VETOR INICIAL DE COEFICIENTES COM 0 NAS POSICOES
double * cria_vetor_coeficiente(double *vetor, int tamanho)
{
    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i]= 0;
    }

    return vetor;

}

// GERANDO O VETOR DE RUIDO COM VALORES ALEATORIOS E PEQUENOS
double * cria_vetor_ruido(double *vetor, int seed, int tamanho)
{
    double valor;

    // Gernando valores aleatorios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = AWGN_generator();
        vetor[i]= valor/100000.0;
    }

    return vetor;

}

// FUNCAO QUE ATRIBUI OS VALORES DE y(n)
double * sinal_desejado(double *vetor_Un, double *vetor_Yn, int tamanho)
{
    double f1 = 0;
    double f2 = 0;

    /*
    vetor_Yn[0] = sin(vetor_Un[0]);
    vetor_Yn[1] = sin(vetor_Un[1]) + sin(vetor_Un[0]);

    for (int i = 2; i < tamanho; i++)
    {
        vetor_Yn[i] = sin(vetor_Un[i]) + sin(vetor_Un[i-1]) + sin(vetor_Un[i-2]);
    }
    */

    /*
    for (int i = 0; i < tamanho; i++)
    {
        vetor_Yn[i] = exp(vetor_Un[i]);
    }
    */

    for (int i = 0; i < tamanho; i++)
    {
        f1 = vetor_Un[i];
        f2 = (i>0) ? vetor_Un[i-1] : 0;

        vetor_Yn[i] = sin(f1) + sin(f2) + (sin(f1) * sin(f2));
    }

    return vetor_Yn;
}

// FUNCAO QUE CRIA VETOR DE ENTRADA COM VALORES ALEATORIOS
int* cria_vetor_interio(int *vetor, int seed, int tamanho)
{
    vetor = (int *)malloc(tamanho * sizeof(int));

    // Gernando valores aleatorios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i]= rand() % 31; // Garantindo que serao valores aleatorios sejam de -1 a 1
    }

    return vetor;
}

// FUNCAO QUE CRIA VETOR DE ENTRADA COM VALORES ALEATORIOS
double* cria_vetor_dicionario(double *vetor, int seed, int tamanho)
{
    double valor;

    // Gernando valores aleat�rios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = AWGN_generator();
        valor = valor / 1000;
        vetor[i]= valor;
    }
    return vetor;
}

// FUNCAO QUE CRIA VETOR DE ENTRADA COM VALORES ALEAToRIOS
double* cria_vetor_entrada(double *vetor, int seed, int tamanho)
{
    double valor;

    FILE *arquivo = fopen("vetor_entrada.txt", "w");

    // Gernando valores aleatorios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = AWGN_generator();
        valor = valor / 100;
        vetor[i]= valor;
    }

    for (int i = 0; i < tamanho; i++)
    {
        fprintf(arquivo, "%lf\n", vetor[i]);             // Escrevendo valores SSE no arquivo
    }

    fclose(arquivo);

    return vetor;
}

double* calculo_vetor_entrada(double *vetor, int tamanho)
{
    double soma = 0;
    FILE *arquivo = fopen("calculo_entrada.csv", "w");

    soma = vetor[1] + vetor[0];
    //printf("%1f\n",soma);
    for(int i=1; i<(tamanho-1);i++)
    {
        soma = vetor[i+1] + soma;
        //printf("%1f\n",soma);
    }

    fprintf(arquivo, "%lf\n", soma);
    fclose(arquivo);

    return vetor;
}

double *zera_kernel (double *kw, int tamanho)
{
    for(int i=0;i<tamanho;i++)
    {
        kw[i] = 0;
    }

    return kw;
}

double *alocacao_dinamica(double *vetor, int tamanho)
{
    vetor = (double *)malloc(tamanho * sizeof(double));

    return vetor;
}

// FUNCAO PRINCIPAL
int main ()
{
    // Declarando Variaveis

    int M = 5;          // Tamanho de Vetor: Kernel, Dicionário, Coeficientes
    int N = 5000;       // Numero de amostras 

    // PARAMETROS
    double passo = 0.5;
    double eta = 0.0015;

    double *dicionario= NULL; // Dicionario
    double *Un= NULL;         // Amostras de entrada
    double *Yn= NULL;         // Sinal Desejado (Valor Base)
    double *Zn= NULL;         // Sinal Ruidoso
    double *Wn= NULL;         // Coeficientes (Pesos)
    double *kw= NULL;         // Kernel
    double dw;          // Saida Estimada no instante n (Valor Escalar) 
    double d;           // Sinal desejado no instante n (Valor Escalar)
    double e = 0;       // Erro no instante n (Valor Escalar)
    double *sse = NULL;         // Erro quadrático médio
    double *salva_resultado= NULL;    // Vetor auxiliar


    sse = alocacao_dinamica(sse,N);
    sse = cria_vetor_coeficiente(sse,N);

    Un = alocacao_dinamica(Un,N);                   
    Yn = alocacao_dinamica(Yn,N);                  
    Zn = alocacao_dinamica(Zn,N);                     
    dicionario = alocacao_dinamica(dicionario,M); 
    Wn = alocacao_dinamica(Wn,M);                  
    salva_resultado = alocacao_dinamica(salva_resultado,N);
    kw = alocacao_dinamica(kw,M);      

    FILE *arquivo = fopen("valores.csv", "w");
    FILE *arquivo_txt = fopen("valores_txt.txt", "w");

    for(int m=0; m<100; m++)
    {
        Un = cria_vetor_entrada(Un,10,N);                   // Criando o vetor de entrada com valores aleatorios
        Yn = sinal_desejado(Un, Yn, N);                     // Realizando a operacao Exponencial y(n)
        Zn = cria_vetor_ruido(Zn,1,N);                      // Criando o vetor com os dados de ruido 10^(-5)
        dicionario = cria_vetor_dicionario(dicionario,2,M); // Criando o Vetor Dicionario
        Wn = cria_vetor_coeficiente(Wn,M);                  // Criando Vetor de coeficientes inicializados com 0
        salva_resultado = cria_vetor_coeficiente(salva_resultado,N);
        kw = cria_vetor_coeficiente(kw,M);      // Criando o vetor Kernel inicialmente zerado


        for(int n=0; n<N; n++)
        {
            kw = zera_kernel(kw,M);                     // Zerando vetor kernel
            kw = calculo_kw(kw,Un,dicionario,n,M,eta);  // Calculando todos os valores de Kernel atraves da interacao dos valores de entrada e dicionario
            dw = calculo_dw(Wn, kw, M);                 // Calculando o valor desejado Dw atraves de um produto escalar entre vetores
            d = calculo_d(Yn,Zn,n);                     // Calculando o valor de d
            e = dw - d;                                 // Calculando o erro
            salva_resultado[n] = e;
            Wn = calculo_Wn(Wn,passo,kw,e,M);       // Atualizando os valores do vetor coeficiente
            dicionario = atualizar_dicionario(Un,dicionario,n,M); // Atualizando os valores do dicionario com os valores de entrada
        }

        sse = calculo_sse(sse,salva_resultado,N);

        //passo = passo * taxa_decaimento;

        for (int i = 0; i < N; i++)
        {
            fprintf(arquivo, "%0.20lf\n", sse[i]);             // Escrevendo valores SSE no arquivo
        }

        for (int i = 0; i < N; i++)
        {
            fprintf(arquivo_txt, "%0.20lf\n", sse[i]);             // Escrevendo valores SSE no arquivo
        }
    }

    // Obtendo a data e hora atuais
    time_t agora = time(NULL); // Captura o tempo atual em segundos desde 1970
    struct tm *tempo_local = localtime(&agora); // Converte para o formato local de data/hora

    // Formatando a data e hora em uma string
    char data_hora[100];
    strftime(data_hora, sizeof(data_hora), "%Y-%m-%d %H:%M:%S", tempo_local);

    // Gravando a data e hora no arquivo
    fprintf(arquivo_txt, "Data e Hora: %s\n", data_hora);

    fclose(arquivo);
    fclose(arquivo_txt);

    printf("ENTRADA (UN):\n");
    imprime_teste_1(Un,N);
    printf("\n");

    //printf("Y(N):\n");
    //imprime_teste_1(Yn,N);
    //printf("\n");

    //printf("RUIDO:\n");
    //imprime_teste_2(Zn,N);
    //printf("\n");

    //printf("DICIONARIO:\n");
    //imprime_teste_1(dicionario,M);
    //printf("\n");

    //printf("COEFICIENTE:\n");
    //imprime_teste_1(Wn,M);
    //printf("\n");

    //printf("KERNEL:\n");
    //imprime_teste_1(kw,M);
    //printf("\n");

    //printf("Dw:\n");
    //printf("%lf",dw);
    //printf("\n");

    //printf("D:\n");
    //printf("%0.7lf",d);
    //printf("\n");

    //printf("e:\n");
    //printf("%lf",e);
    //printf("\n");

    //printf("e2:\n");
    //printf("%lf",e2);
    //printf("\n");

    //printf("TODOS OS VALORES DE E:\n");
    //imprime_teste_1(salva_resultado,N);
    //printf("\n");

    //printf("VETOR SSE:\n");
    //imprime_teste_1(sse,N);
    //printf("\n");

    free(Un);
    free(Yn);
    free(Zn);
    free(dicionario);
    free(Wn);
    free(salva_resultado);
    free(kw);
}
