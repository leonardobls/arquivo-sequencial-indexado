#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define VARCHAR 100
#define ROW 500
#define TAM_REG 20

//-------------------------------------------------------------

typedef struct city{
    int id;
    char country[VARCHAR];
    char name[VARCHAR];
    char accentCity[VARCHAR];
    char region[VARCHAR];
    char population[VARCHAR];
    char latitude[VARCHAR];
    char longitude[VARCHAR];
}CITY;

typedef struct city_pesquisa{
    int id;
    long posicao;
}CITY_INDEX;

typedef struct city_pesquisa_pais{
    int id;
    char nome[VARCHAR];
    long posicao;
}CITY_INDEX_NOME;

typedef struct nodo_lista{
    char pais[VARCHAR];
    long posicao_inicial;
    int quant;
    struct NODO_PAIS * proximo;
}NODO_PAIS;

typedef struct lista{
    NODO_PAIS * inicio;
    NODO_PAIS * fim;
    int tam;
}Lista;
/*ARVORE*/
struct nodo{
    char pais[VARCHAR];
    char cidade[VARCHAR];
    float quant;
    //long posicao;
    int alt;
    struct nodo * esq;
    struct nodo * dir;
};
typedef struct nodo ARVORE;


//-------------------------------------------------------------

int altura (ARVORE * nodo){
    if(nodo == NULL){
        return -1;
    }
    else{
        return nodo->alt;
    }
}
//-------------------------------------------------------------
int fator_balanceamento(ARVORE * nodo){
    return altura(nodo->esq) - altura(nodo->dir);
}
//-------------------------------------------------------------
int maior(int a, int b){
   return (a>b?a:b);
}
//-------------------------------------------------------------
void rotacao_esquerda(ARVORE **raiz){
    ARVORE * nodo = (*raiz)->dir;
    (*raiz)->dir = nodo->esq;
    nodo->esq = (*raiz);
    (*raiz)->alt = maior(altura((*raiz)->esq), altura((*raiz)->dir)) + 1;
    nodo->alt = maior(altura(nodo->esq), altura(nodo->dir)) + 1;
    (*raiz) = nodo;
}
//-------------------------------------------------------------
void rotacao_direita(ARVORE **raiz){
    ARVORE * nodo = (*raiz)->esq;
    (*raiz)->esq = nodo->dir;
    nodo->dir = (*raiz);
    (*raiz)->alt = maior(altura((*raiz)->esq), altura((*raiz)->dir)) + 1;
    nodo->alt = maior(altura(nodo->esq), (*raiz)->alt) + 1;
    (*raiz) = nodo;
}
//-------------------------------------------------------------
void escreve(ARVORE * raiz){
    if(raiz != NULL){
        escreve(raiz->esq);
        if(raiz->quant != 0){
            printf("Pais: %s, Cidade: %s, População: %.2f\n", raiz->pais, raiz->cidade,raiz->quant);
        }

        escreve(raiz->dir);
    }
}
//-------------------------------------------------------------
void rotacao_esquerda_direita(ARVORE ** raiz){
    rotacao_esquerda(&(*raiz)->esq);
    rotacao_direita(raiz);
}
//-------------------------------------------------------------
void rotacao_direita_esquerda(ARVORE ** raiz){
    rotacao_direita(&(*raiz)->dir);
    rotacao_esquerda(raiz);
}
//-------------------------------------------------------------
void insereArvore(ARVORE ** raiz, char pais[], char cidade[], float quant){
    if((*raiz) == NULL){
        ARVORE * nodo = (ARVORE *) malloc(sizeof(ARVORE));
        strcpy(nodo->pais, pais);
        strcpy(nodo->cidade, cidade);
        nodo->quant = quant;
        //nodo->posicao = posicao;
        nodo->alt = 0;
        nodo->dir = NULL;
        nodo->esq = NULL;
        (*raiz) = nodo;
    }
    else if((*raiz)->quant > quant){
        insereArvore(&(*raiz)->esq, pais, cidade, quant);
        if(abs(fator_balanceamento((*raiz))) >= 2){
            if((*raiz)->esq->quant > quant){
                rotacao_direita(raiz);
            }
            else{
                rotacao_esquerda_direita(raiz);
            }
        }
    }
    else if((*raiz)->quant < quant){
        insereArvore(&(*raiz)->dir, pais, cidade, quant);
        if(abs(fator_balanceamento((*raiz))) >= 2){
            if((*raiz)->dir->quant < quant){
                rotacao_esquerda(raiz);
            }
            else{
                rotacao_direita_esquerda(raiz);
            }
        }
    }
    (*raiz)->alt = maior(altura((*raiz)->esq), altura((*raiz)->dir)) + 1;
}

//-------------------------------------------------------------

void criarArvore(ARVORE ** raiz){

    char pais[] = "ad";
    char cidade[VARCHAR];
    //long posicao, posicao_inicial =  sizeof(CITY);
    float quantidade = 0;
    int pula = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");

    CITY dados;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

        if(pula == 0){
            pula++;
            continue;
        }

        if(!feof(arquivoDados)){
            //posicao = ftell(arquivoDados) - sizeof(CITY);
            if(quantidade < atof(dados.population)){
                    //printf("%f\n", atof(dados.population));
                quantidade = atof(dados.population);
                strcpy(cidade, dados.name);
            }

            //printf("%s, %d, %d\n", dados.country, posicao_inicial, posicao);
            if(strcasecmp(pais, dados.country) != 0){
               //printf("%s %d %d\n", pais, quantidade, posicao);
                insereArvore(&(*raiz), pais, cidade, quantidade);
                strcpy(pais, dados.country);
                quantidade = 0;
                //posicao_inicial = posicao;

            }

        }

    }
    insereArvore(&(*raiz), pais, dados.name, quantidade);

    fclose(arquivoDados);
}



/*FIM ARVORE*/
//-------------------------------------------------------------
void exibeOpcoes(){  setlocale(LC_ALL, "Portuguese");

    printf("\n\n---------------*** QUADRO DE OPERACOES ***---------------\n");
    printf("1 - Listar todos os registros.\n");
    printf("2 - Realizar a pesquisa binária.\n");
    printf("3 - Pesquisar id do item pelo arquivo de indices.\n");
    printf("4 - Pesquisar cidade pelo nome.\n");
    printf("5 - Ver cidades pertencentes ao mesmo pais.\n\n");
    printf("6 - Ver as cidades mais populosas de cada país.\n\n");
    printf("0 - Sair.\n");
    printf("---------------------------------------------------------\n");
}

//-------------------------------------------------------------

void criaArquivoBinarioIndexadoOrdenado(FILE *arquivo){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivoBinario = fopen("arquivoBinarioIndexado.bin", "wb");

    if(arquivoBinario == NULL){
        printf("Não foi possível criar o arquivo!\n");
    }

    char row[VARCHAR];
    char *value;
    CITY city;

    int i = 0;
    while(fgets(row, VARCHAR, arquivo) != NULL){
        if(i > 0 && i <= 100000){
            char *column = strtok(row, ",");

            int j = 0;
             while( column != NULL ) {
                city.id = i;
                if(j == 0){
                    strcpy(city.country, column);
                }
                else if(j == 1){
                    strcpy(city.name, column);
                }
                else if(j == 2){
                    strcpy(city.accentCity, column);
                }
                else if(j == 3){
                    strcpy(city.region, column);
                }
                else if(j == 4){
                    strcpy(city.population, column);
                }
                else if(j == 5){
                    strcpy(city.latitude, column);
                }
                else if(j == 6){
                    strcpy(city.longitude, column);
                }

                column = strtok(NULL, ",");
                j++;
            }
        }
        i++;

	    fwrite(&city, sizeof(CITY), 1, arquivoBinario);
    }

    fclose(arquivo);
    fclose(arquivoBinario);
}

//-------------------------------------------------------------

void exibeDadosArquivoBinario(){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivo = fopen("arquivoBinarioIndexado.bin", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    CITY city;
    char condicao;
    int count = 0;

    printf("\n\nREGISTROS: ----------------------------\n\n");

    while(1){
        while(count < 20 && fread(&city, sizeof(CITY), 1, arquivo) != NULL){
            printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %f\nLatitude: %f\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);
            count ++;
        }

        printf("Para exibir mais 20 itens (s - SIM / n - NAO): ");

        scanf(" %c", &condicao);

        // printf("\n\t\t%c\n", condicao);

        if(condicao == 's'){
            count = 0;
            if(city.id != 20){ //testar se o id atual não é o utlimo, se n é ent continua
                continue;
            }else{
                printf("não existem mais registros\n");
                break;
            }
        }else{
            break;
        }
    }

    fclose(arquivo);

}

//-------------------------------------------------------------


void pesquisaBinariaSimples(){

    int id, valor;
    printf("Digite o id que você deseja buscar: ");
    scanf("%d", &id);

    FILE *arquivo = fopen("arquivoBinarioIndexado.bin", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    int inicio, meio, fim;

    inicio = 0;
    fim = 19;

    CITY city;

    while(inicio <= fim){
        meio = (inicio+fim) / 2;
        fseek(arquivo, meio * sizeof(CITY), SEEK_SET);
        fread(&city, sizeof(CITY), 1, arquivo);
        if(id > city.id){
            inicio = meio+1;
        }else{
            if(id < city.id){
                fim = meio - 1;
            }else{

                while(1){

                    printf("\nEscolha uma das opções para pesquisar: \n\n");
                    printf("------------------------------------------------------\n");
                    printf("1 - Qual é o número de habitantes dessa cidade?\n");
                    printf("2 - Qual é a longitude e latitude dessa cidade no globo?\n");
                    printf("3 - Qual é o país dessa cidade?\n");
                    printf("4 - Qual é o sotaque dessa cidade?\n\n");
                    printf("0 - Cancelar a pesquisa\n");
                    printf("------------------------------------------------------\n\n");


                    while(scanf("%d", &valor) != NULL){

                        if(valor >= 0 && valor <= 4){
                            break;
                        }else{
                            printf("Digite um valor de 0 a 4!!\n\n");
                        }
                    }


                    switch ( valor ){
                        case 1 :
                            if(city.population != NULL){
                                printf("\t***Resposta: São %s habitantes\n", city.population);
                            }else{
                                printf("\t***Resposta: O número de habitantes não consta no registro! \n");
                            }
                            break;

                        case 2 :
                            if(city.latitude != NULL && city.longitude != NULL){
                                printf("\t***Resposta: Para o exio x temos uma latitude de %s\nPara o eixo y temos uma longitude de %s\n", city.latitude, city.longitude);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;

                        case 3:
                            if(city.country != NULL){
                                printf("\t***Resposta: A sigla do país é %s\n", city.country);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;

                        case 4:
                            if(city.country != NULL){
                                printf("\t***Resposta: O sotaque dessa cidade é denomindado de %s\n", city.accentCity);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;

                        case 0:
                            break;
                    }
                }

                break;
            }
        }
    }

    printf("Não foi encontrado nenhum registro!\n");

    fclose(arquivo);
}

//-------------------------------------------------------------


void inserirLISTA(Lista * lista, char pais[], long inicio, int quant) {
    NODO_PAIS *novo = (NODO_PAIS*)malloc(sizeof(NODO_PAIS));
    strcpy(novo->pais, pais);
    novo->posicao_inicial = inicio;
    novo->quant = quant;
    novo->proximo = NULL;

    if(lista->inicio == NULL) {
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        lista->fim->proximo = novo;
        lista->fim = novo;
    }
    lista->tam++;
}

//-------------------------------------------------------------


void mostraCidadesPais(long inicio ,int quant){
    FILE * arquivoDados;
    CITY city;

    arquivoDados = fopen("arquivoBinarioIndexado.bin","rb");
    fseek(arquivoDados, inicio, SEEK_SET);

    while(fread(&city, sizeof(CITY), 1, arquivoDados)!= NULL && quant-1 > 0){
         printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %f\nLatitude: %f\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);
         quant--;
    }
    fclose(arquivoDados);
}

//-------------------------------------------------------------

void imprimir(Lista *lista) {
    char pais[VARCHAR];
    printf("Digite a sigla do país que deseja ver as cidades\n");
    scanf("%s", &pais);
    NODO_PAIS *aux = lista->inicio;

    while(aux != NULL) {
        if(strcasecmp(aux->pais, pais) == 0){
            //printf("%s, %d, %d\n", aux->pais, aux->posicao_inicial, aux->posicao_final);
            mostraCidadesPais(aux->posicao_inicial, aux->quant);
            break;
        }
        aux = aux->proximo;
    }
    printf("\n\n");
}

//-------------------------------------------------------------

void criarArquivoIndexPais(){

    char *value;
    char row[VARCHAR];
    long posicao, pula = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");
    FILE * arquivoIndex = fopen("arquivoIndex_pais.bin", "wb");

    CITY dados;
    CITY_INDEX_NOME index;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

        if(pula == 0){
            pula++;
            continue;
        }

        if(!feof(arquivoDados)){
            posicao = ftell(arquivoDados) - sizeof(CITY);
            //posicao = ftell(arquivoDados));

            index.id = dados.id;
            strcpy(index.nome, dados.name);
            index.posicao = posicao;

            }

            //printf("ITEM: %d e posicao %ld \n", index.id, index.posicao);
            fwrite(&index, sizeof(CITY_INDEX_NOME), 1, arquivoIndex);
        }



    fclose(arquivoDados);
    fclose(arquivoIndex);

}

//-------------------------------------------------------------

void pesquisaBinariaIndexPais(){
    char pais[VARCHAR];
    printf("Digite o pais que você deseja buscar: ");
    scanf("%s", &pais);


    FILE *arquivo = fopen("arquivoIndex_pais.bin", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    int inicio, meio, fim;

    inicio = 0;
    fim = 19;

    CITY_INDEX_NOME index;

    while(inicio <= fim){
        meio = (inicio+fim) / 2;
        fseek(arquivo, meio * sizeof(CITY_INDEX_NOME), SEEK_SET);
        fread(&index, sizeof(CITY_INDEX_NOME), 1, arquivo);
        if(strcasecmp(pais, index.nome) > 0){
            inicio = meio+1;
        }else{
            if(strcasecmp(pais, index.nome) < 0){
                fim = meio - 1;
            }else{
                mostraItem(index.posicao);
                break;
            }
        }
    }

    fclose(arquivo);

}

//-------------------------------------------------------------

void criarArquivoIndexSequencial(){

    char *value;
    char row[VARCHAR];
    long posicao, pula = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");
    FILE * arquivoIndex = fopen("arquivoIndex_campo1.bin", "wb");

    CITY dados;
    CITY_INDEX index;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

        if(pula == 0){
            pula++;
            continue;
        }

        if(!feof(arquivoDados)){
            posicao = ftell(arquivoDados) - sizeof(CITY);
            //posicao = ftell(arquivoDados));
            index.id = dados.id;
            index.posicao = posicao;

            //printf("ITEM: %d e posicao %ld \n", index.id, index.posicao);
        }

        fwrite(&index, sizeof(CITY_INDEX), 1, arquivoIndex);
    }

    fclose(arquivoDados);
    fclose(arquivoIndex);

}

//-------------------------------------------------------------

void mostraItem(long posicao){

    FILE * arquivoDados;
    CITY city;

    arquivoDados = fopen("arquivoBinarioIndexado.bin","rb");
    fseek(arquivoDados, posicao, SEEK_SET);

    fread(&city, sizeof(CITY), 1, arquivoDados);
    fclose(arquivoDados);
    printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %f\nLatitude: %f\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);


}

//-------------------------------------------------------------

void pesquisaBinariaIndex(){
    int id;
    printf("Digite o id que você deseja buscar: ");
    scanf("%d", &id);


    FILE *arquivo = fopen("arquivoIndex_campo1.bin", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    int inicio, meio, fim;

    inicio = 0;
    fim = 19;

    CITY_INDEX index;

    while(inicio <= fim){
        meio = (inicio+fim) / 2;
        fseek(arquivo, meio * sizeof(CITY_INDEX), SEEK_SET);
        fread(&index, sizeof(CITY_INDEX), 1, arquivo);
        if(id > index.id){
            inicio = meio+1;
        }else{
            if(id < index.id){
                fim = meio - 1;
            }else{
                mostraItem(index.posicao);
                // printf("\t%d\n", index.id);
                break;
            }
        }
    }

    fclose(arquivo);

}

//-------------------------------------------------------------
void criarLista(Lista * lista){
    char row[VARCHAR];
    char nome[] = "ad";
    long posicao, posicao_inicial =  sizeof(CITY);
    int quantidade = 0;
    int pula = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");

    CITY dados;


    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

        if(pula == 0){
            pula++;
            continue;
        }

        if(!feof(arquivoDados)){
            posicao = ftell(arquivoDados) - sizeof(CITY);
            quantidade++;
            //printf("%s, %d, %d\n", dados.country, posicao_inicial, posicao);
            if(strcasecmp(nome, dados.country) != 0){
                inserirLISTA(lista, nome, posicao_inicial, quantidade);
                strcpy(nome, dados.country);
                posicao_inicial = posicao;

            }
            else{
                continue;
            }

        }

    }
    inserirLISTA(lista, nome, posicao_inicial, posicao);

    fclose(arquivoDados);
}


//-------------------------------------------------------------

int main(){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivoOriginal = fopen("teste.csv", "r");
    Lista lista;
    ARVORE * raiz = NULL;
    lista.fim = NULL;
    lista.inicio = NULL;
    lista.tam = 0;

    if(arquivoOriginal == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    criaArquivoBinarioIndexadoOrdenado(arquivoOriginal);

    criarArquivoIndexSequencial();

    criarArquivoIndexPais();

    criarLista(&lista);



    int valor;
    int controller = 1;

    while(controller){

        printf ("\n\nEscolha uma opção abaixo: ");
        exibeOpcoes();

        while(scanf("%d", &valor) != NULL){
            if(valor >= 0 && valor <= 6){
                break;
            }else{
                printf("Por favor, insira um número de 0 a 5!\n");
            }
        }

        switch ( valor )
        {
            case 1 :
                exibeDadosArquivoBinario();
                break;

            case 2 :
                pesquisaBinariaSimples();
                break;

            case 3:
                pesquisaBinariaIndex();
                break;

            case 4:
                pesquisaBinariaIndexPais();
                break;

             case 5:
                imprimir(&lista);
                break;

             case 6:
                criarArvore(&raiz);
                escreve(raiz);
                break;

             case 0:
                controller = 0;
                printf("Tchauzin :)\n");
                break;
        }
    }
    return 0;
}
