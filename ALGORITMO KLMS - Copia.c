#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// FUNÇÃO PARA VERIFICAR A CONSTRUÇÃO DO VETOR
void imprime_teste_2(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%0.7lf",vetor[i]);
        printf("\n");
    }
}

// FUNÇÃO PARA VERIFICAR A CONSTRUÇÃO DO VETOR
void imprime_teste_1(double *vetor,int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%0.8lf",vetor[i]);
        printf("\n");
    }
}

// FUNÇÃO PARA VERIFICAR A CONSTRUÇÃO DO VETOR
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

double calculo_e2(double e2, double e)
{
    e2 = e2 + (pow(e,2));

    return e2;
}


//FUNÇÃO QUE CALCULA UM NOVO VALOR PARA O COEFICIENTE Wn
double calculo_Wn(double Wn_n,double passo,double kw_n,double e)
{
    double aux, resultado;

    aux = 2 * passo * kw_n * e;
    resultado = Wn_n - aux;

    return resultado;
}

// FUNÇÃO QUE CALCULA O ESCALAR d
double calculo_d(double *Yn, double *Zn, int n)
{
    double d = 0;

    d = Yn[n] + Zn[n];

    return d;
}

// FUNÇÃO QUE CALCULA O ESCALAR dw
double calculo_dw(double *Wn, double kwn, int tamanho)
{
    int j;
    double dw = 0;
    double multiplicacao = 0;

        for(j=0; j<tamanho; j++)
        {
            multiplicacao = kwn * Wn[j];
            dw = dw + multiplicacao;
        }

    return dw;
}


// FUNÇÃO QUE CALCULA O kw(n) POSSÍVEL ERRO NESTA FUNÇÃO
double calculo_kw(double *vetor_Un, double *vetor_Dn, int n)
{
    //Declarando Variáveis
    double aux;         // Variável que guarda a subtração de Un e Dn
    double numerador;   // Elevo a subtração ao quadrado
    double aux_1;       // Elevo o sigma ao quadrado
    double denominador; // Multiplico o valor de aux_1 por 2
    double resultado;   // divido o numerado pelo denominador
    double kw_n;        // elevamos Oiler (e) ao resultado encontrado

    printf("Un: %lf\n",vetor_Un[n]);
    printf("Dn: %1f\n",vetor_Dn[n]);

    aux = (vetor_Un[n] - vetor_Dn[n]);
    aux_1 = pow(0.3,2);

    numerador = pow(aux,2);
    denominador = aux_1 * 2;

    resultado = (numerador / denominador);

    kw_n = 1 / (exp(resultado));



    printf("Numerador: %lf\n",numerador);
    printf("Denominador: %lf\n",denominador);
    printf("Resultado: %lf\n",resultado);
    printf("Kw(n): %lf\n\n",kw_n);

    return kw_n;
}

// FUNÇÃO QUE GERA O VETOR INICIAL DE COEFICIENTES COM 0 NAS POSIÇÕES
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

// GERANDO O VETOR DE RUÍDO COM VALORES ALEATÓRIOS E PEQUENOS
double * cria_vetor_ruido(double *vetor, int seed, int tamanho)
{
    double valor;

    // Alocando vetor
    vetor = (double *)malloc(tamanho * sizeof(double));

    // Gernando valores aleatórios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor= 2.0 * (rand() / (double)RAND_MAX) - 1.0; // Garantindo que os valores aleatorios sejam elevados a -05
        vetor[i]= valor/100000.0;
    }

    return vetor;

}

// FUNÇÃO QUE ATRIBUI OS VALORES DE y(n)
double * operacao_exponencial(double *vetor_Un, double *vetor_Yn, int tamanho)
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

// FUNÇÃO QUE CRIA VETOR DE ENTRADA COM VALORES ALEATÓRIOS
int* cria_vetor_interio(int *vetor, int seed, int tamanho)
{
    vetor = (int *)malloc(tamanho * sizeof(int));

    // Gernando valores aleatórios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i]= rand() % 31; // Garantindo que serão valores aleatorios sejam de -1 a 1
    }

    return vetor;
}

// FUNÇÃO QUE CRIA VETOR DE ENTRADA COM VALORES ALEATÓRIOS
double* cria_vetor_entrada(double *vetor, int seed, int tamanho)
{
    double valor;

    vetor = (double *)malloc(tamanho * sizeof(double));

    // Gernando valores aleatórios
    srand(seed);

    // Preenchendo o vetor
    for (int i = 0; i < tamanho; i++)
    {
        valor = 2.0 * (rand() / (double)RAND_MAX) - 1.0;
        vetor[i]= round(valor*10000000.0)/10000000.0; // Garantindo que serão valores aleatorios sejam de -1 a 1
    }

    return vetor;
}

// FUNÇÃO PRINCIPAL
int main ()
{
    // Declarando Variáveis

    int M = 10;       // Tamanho de Vetor
    int N = 10;       // Numero de amostras - testando em um valor baixo

    double *dicionario;  // Vetor Dicionário
    double *Un;          // Vetor de entrada
    double *Yn; // Vetor Exponencial
    double *Zn; // Vetor dos Ruídos
    double *Wn; // Vetor Coeficientes
    double *kw; // Vetor Kernel
    double dw;  // Valor escalar da minha saída estimada
    double d;   // Valor escalar desejado num instante n
    double *e;
    double passo = 0.01;
    double *e2 = 0;



    //for 1

    Un = cria_vetor_entrada(Un,10,N);       // Criando o vetor de entrada com valores aleatórios
    Yn = operacao_exponencial(Un, Yn, N);   // Realizando a operção Exponencial y(n)
    Zn = cria_vetor_ruido(Zn,1,N);          // Criando o vetor com os dados de ruido 10^(-5)
    dicionario = cria_vetor_entrada(dicionario,2,M); // Criando o Vetor Dicionário
    Wn = cria_vetor_coeficiente(Wn,M);      // Criando Vetor de coeficientes inicializados com 0
    kw = cria_vetor_coeficiente(kw,M);      // Criando o vetor Kernel inicialmente zerado
    e = cria_vetor_coeficiente(e,M);
    e2 = cria_vetor_coeficiente(e2,M);

    for(int n=0; n<N; n++)
    {
        kw[n] = calculo_kw(Un,dicionario,n); // Calculando cada elemento do vetor Kernel
        dw = calculo_dw(Wn, kw[n], M);
        d = calculo_d(Yn,Zn,n);
        e[n] = dw - d;
        Wn[n] = calculo_Wn(Wn[n],passo,kw[n],e[n]);
    }

    for (int n=0; n<N; n++)
    {
        e2[n] = calculo_e2(e2[n],e[n]);
    }

    // END FOR


    //printf("ENTRADA (UN):\n");
    //imprime_teste_1(Un,N);
    //printf("\n");

    //printf("Y(N):\n");
    //imprime_teste_1(Yn,N);
    //printf("\n");


    //printf("RUIDO:\n");
    //imprime_teste_2(Zn,N);
    //printf("\n");

    //printf("DICIONARIO:\n");
    //imprime_teste_1(dicionario,M);
    //printf("\n");

    printf("COEFICIENTE:\n");
    imprime_teste_1(Wn,M);
    printf("\n");

    //printf("KERNEL:\n");
    //imprime_teste_1(kw,M);
    //printf("\n");

    printf("Dw:\n");
    printf("%lf",dw);
    printf("\n");

    printf("D:\n");
    printf("%0.7lf",d);
    printf("\n");

    printf("e:\n");
    printf("%0.7lf",e);
    printf("\n");

    printf("e2:\n");
    imprime_teste_1(e2,M);
    printf("\n");
}
