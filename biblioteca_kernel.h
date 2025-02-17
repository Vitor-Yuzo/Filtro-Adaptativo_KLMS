#ifndef biblioteca_kernel
#define biblioteca_kernel

double *kernel_gaussiano(double *u, double *u_linha, double eta, int tamanho)
{
    //DECLARANDO VARIÁVEIS
    double aux=0;         // Variável que guarda a subtração de u e u_linha
    double aux_1=0;       // Elevo o eta ao quadrado
    double numerador=0;   // Elevo a subtração ao quadrado
    double denominador=0; // Multiplico o valor de aux_1 por 2
    double resultado=0;   // divido o numerado pelo denominador
    double *kw;

    kw = (double *)malloc(tamanho * sizeof(double));

    for (int i=0; i<tamanho; i++)
    {
        aux = (u[i] - u_linha[i]);
        aux_1 = pow(eta,2);

        numerador = pow(aux,2);
        denominador = aux_1 * 2;

        resultado = (numerador / denominador);

        kw[i] = 1 / (exp(resultado));
    }

    return kw;
}

double *kernel_laplaciano(double *u, double *u_linha, double eta, int tamanho)
{
    //DECLARANDO VARIÁVEIS
    double numerador=0;   // Elevo a subtração ao quadrado
    double resultado=0;   // dividir o numerado pelo denominador
    double *kw;          // elevamos Oiler (e) ao resultado encontrado

    kw = (double *)malloc(tamanho * sizeof(double));

    for (int i=0; i<tamanho; i++)
    {
        numerador = (u[i] - u_linha[i]);
        resultado = (numerador / eta);
        kw[i] = 1 / (exp(resultado));
    }

    return kw;
}

// Entramos com 2 vetores e retornamos um escalar
double kernel_polinomial(double *u, double *u_linha, double alfa, int beta, int tamanho)
{
    //DECLARANDO VARIÁVEIS
    double soma=0;         // guarda o valor do produto interno
    double resultado=0;   // soma entre o produto interno e alfa
    double kw=0;          // elevamos resultado a beta e obtemos o kernel

    for(int i=0; i<tamanho; i++)
    {
        soma = soma + (u[i] * u_linha[i]);
    }

    resultado = alfa + soma;
    kw = pow(resultado,beta);

    return kw;
}

// Entramos com 2 vetores e retornamos um escalar
double kernel_linear(double *u, double *u_linha, int tamanho)
{
    //DECLARANDO VARIÁVEIS
    double kw=0;        // O resultado do produto escalar é o nosso kernel

    for(int i=0; i<tamanho; i++)
    {
        kw = kw + (u[i] * u_linha[i]);
    }

    return kw;
}

double kernel_sigmoidal(double *u, double *u_linha, double alfa, double c, int tamanho)
{
    //DECLARANDO VARIÁVEIS
    double soma=0;          // guarda o valor do produto interno
    double resultado_1=0;   // multiplicação entre o produto interno e alfa
    double resultado_2=0;   // o resultado é somado com a constante de deslocamento c
    double kw=0;            // aplicamos a função tangente hiperbólica (uma funct sigmoidal)

    for(int i=0; i<tamanho; i++)
    {
        soma = soma + (u[i] * u_linha[i]);
    }

    resultado_1 = alfa * soma;
    resultado_2 = resultado_1 + c;
    kw = tanh(resultado_2);

    return kw;
}


#endif
