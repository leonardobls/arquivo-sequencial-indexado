#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define VARCHAR 100
#define ROW 500
#define TAM_REG 100000

//*** STRUCTS
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
// *** FIM STRUCTS

// *** FUNCOES
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
    //printf("aqui");
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

    char pais[] = "aa";
    char cidade[VARCHAR];
    //long posicao, posicao_inicial =  sizeof(CITY);
    float quantidade = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");

    CITY dados;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

            //posicao = ftell(arquivoDados) - sizeof(CITY);
            if(quantidade < atof(dados.population)){
                    //printf("%f\n", atof(dados.population));
                quantidade = atof(dados.population);
                strcpy(cidade, dados.name);
            }

            //printf("%s, %d, %d\n", dados.country, posicao_inicial, posicao);
            if(strcasecmp(dados.country, pais) > 0){
               //printf("%s %d %d\n", pais, quantidade, posicao);
                insereArvore(&(*raiz), pais, cidade, quantidade);
                strcpy(pais, dados.country);
                quantidade = 0;
                //posicao_inicial = posicao;

            }



    }
    insereArvore(&(*raiz), pais, dados.name, quantidade);

    fclose(arquivoDados);
}
/*FIM ARVORE*/

//-------------------------------------------------------------

void exibeOpcoes(){  setlocale(LC_ALL, "Portuguese");

    printf("\n\n---------------*** QUADRO DE OPERACOES ***---------------\n");
    printf("1 - Listar registros com paginacao.\n");
    printf("2 - Realizar a pesquisa binaria.\n");
    printf("3 - Pesquisar id do item pelo arquivo de indices.\n");
    printf("4 - Pesquisar cidade pelo nome.\n");
    printf("5 - Ver cidades pertencentes ao mesmo pais.\n");
    printf("6 - Ver as cidades mais populosas de cada pais.\n\n");
    printf("0 - Sair.\n");
    printf("---------------------------------------------------------\n");
    printf("Entrada: ");
}

//-------------------------------------------------------------

void stringReplace(char str[VARCHAR]) {

       char substr[VARCHAR] = ",, ", replace[VARCHAR] = ", , ", output[VARCHAR];

        int i = 0, j = 0, flag = 0, start = 0;

        str[strlen(str) - 1] = '\0';
        substr[strlen(substr) - 1] = '\0';
        replace[strlen(replace) - 1] =  '\0';
        // char *value;

        // check whether the substring to be replaced is present
        while (str[i] != '\0')
        {
                if (str[i] == substr[j])
                {
                        if (!flag)
                                start = i;
                        j++;
                        if (substr[j] == '\0')
                                break;
                        flag = 1;
                }
                else
                {
                        flag = start = j = 0;
                }
                i++;
        }
        if (substr[j] == '\0' && flag)
        {
                for (i = 0; i < start; i++)
                        output[i] = str[i];

                // replace substring with another string
                for (j = 0; j < strlen(replace); j++)
                {
                        output[i] = replace[j];
                        i++;
                }
                // copy remaining portion of the input string "str"
                for (j = start + strlen(substr); j < strlen(str); j++)
                {
                        output[i] = str[j];
                        i++;
                }
                // print the final string
                output[i] = '\0';
                strcpy(str, output);
        }
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
        if(i > 0 && i <= TAM_REG){


           stringReplace(row);

            char *column = strtok(row, ",");

            int j = 0;
             while( column != NULL ) {
                city.id = i;

                if(j == 0 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.country, column);
                }
                else if(j == 1 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.name, column);
                }
                else if(j == 2 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.accentCity, column);
                }
                else if(j == 3 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.region, column);
                }
                else if(j == 4 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.population, column);
                }
                else if(j == 5 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.latitude, column);
                }
                else if(j == 6 && column != NULL){
                    // printf("%s\n", column);
                    strcpy(city.longitude, column);
                }

                column = strtok(NULL, ",");
                j++;
            }

        // printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %s\nLatitude: %s\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);

	    fwrite(&city, sizeof(CITY), 1, arquivoBinario);
        }
        i++;
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


            printf("ID: %d\n", city.id);

            if(strcmp(city.country, " ") != 0){
                printf("PAIS: %s\n", city.country);
            }else{
                printf("PAIS: nao consta!\n");
            }

            if(strcmp(city.name, " ") != 0){
                printf("CIDADE: %s\n", city.name);
            }else{
                printf("CIDADE: nao consta!\n");
            }

            if(strcmp(city.accentCity, " ") != 0){
                printf("SOTAQUE: %s\n", city.accentCity);
            }else{
                printf("SOTAQUE: nao consta!\n");
            }

            if(strcmp(city.region, " ") != 0){
                printf("REGIAO: %s\n", city.region);
            }else{
                printf("REGIAO: nao consta!\n");
            }

            if(strcmp(city.population, " ") != 0){
                printf("POPULACAO: %s\n", city.population);
            }else{
                printf("POPULACAO: nao consta!\n");
            }

            if(strcmp(city.latitude, " ") != 0){
                printf("LATITUDE: %s\n", city.latitude);
            }else{
                printf("LATITUDE: nao consta!\n");
            }

            if(strcmp(city.longitude, " ") != 0){
                printf("LONGITUDE: %s\n", city.longitude);
            }else{
                printf("LONGITUDE: nao consta!\n");
            }

            printf("\n");

            count ++;
        }

        printf("Para exibir mais 20 itens (s - SIM / n - NAO): ");

        scanf(" %c", &condicao);

        if(condicao == 's'){
            count = 0;
            if(city.id != TAM_REG){ //testar se o id atual não é o utlimo, se n é ent continua
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
    printf("\nDigite o id que voce deseja buscar: ");
    scanf("%d", &id);

    FILE *arquivo = fopen("arquivoBinarioIndexado.bin", "rb");

    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo!\n");
    }

    int inicio, meio, fim;

    inicio = 0;
    fim = TAM_REG;

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

                    printf("\nEscolha uma das opcoes para pesquisar: \n\n");
                    printf("------------------------------------------------------\n");
                    printf("1 - Qual e o numero de habitantes dessa cidade?\n");
                    printf("2 - Qual e a longitude e latitude dessa cidade no globo?\n");
                    printf("3 - Qual e o pais dessa cidade?\n");
                    printf("4 - Qual e o sotaque dessa cidade?\n\n");
                    printf("0 - Cancelar a pesquisa\n");
                    printf("------------------------------------------------------\n\n");


                    while(scanf("%d", &valor) != NULL){

                        if(valor >= 0 && valor <= 4){
                            break;
                        }else{
                            printf("Digite um valor de 0 a 4!!\n\n");
                        }
                    }

                    if(valor == 0){
                        break;
                    }

                    switch (valor){
                        case 1 :
                            if(strcmp(city.population, " ") != 0){
                                printf("\t***Resposta: Sao %s MIL habitantes\n", city.population);
                            }else{
                                printf("\t***Resposta: O numero de habitantes nao consta no registro! \n");
                            }
                            break;

                        case 2 :
                            if(strcmp(city.latitude, " ") != 0 && strcmp(city.longitude, " ") != 0){
                                printf("\t***Resposta: Para o exio x temos uma latitude de %s\nPara o eixo y temos uma longitude de %s\n", city.latitude, city.longitude);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;

                        case 3:
                            if(strcmp(city.country, " ") != 0){
                                printf("\t***Resposta: A sigla do país é %s\n", city.country);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;

                        case 4:
                            if(strcmp(city.country, " ") != 0){
                                printf("\t***Resposta: O sotaque dessa cidade é denomindado de %s\n", city.accentCity);
                            }else{
                                printf("\t***Resposta: A informação solicitada não consta no registro! \n");
                            }
                            break;
                    }
                }
                break;
            }
        }
    }

    printf("Registro nao encontrado ou acao cancelada!\n");

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


void mostraCidadesPais(long inicio ,char pais[]){
    FILE * arquivoDados;
    CITY city;

    arquivoDados = fopen("arquivoBinarioIndexado.bin","rb");
    fseek(arquivoDados, inicio - sizeof(CITY), SEEK_SET);

    // printf("quanti %d\n", quant);

    while(fread(&city, sizeof(CITY), 1, arquivoDados)!= NULL ){

        if(strcasecmp(city.country, pais) > 0){
            break;
        }
        else{
        printf("ID: %d\n", city.id);

        if(strcmp(city.country, " ") != 0){
            printf("PAIS: %s\n", city.country);
        }else{
            printf("PAIS: nao consta!\n");
        }

        if(strcmp(city.name, " ") != 0){
            printf("CIDADE: %s\n", city.name);
        }else{
            printf("CIDADE: nao consta!\n");
        }

        if(strcmp(city.accentCity, " ") != 0){
            printf("SOTAQUE: %s\n", city.accentCity);
        }else{
            printf("SOTAQUE: nao consta!\n");
        }

        if(strcmp(city.region, " ") != 0){
            printf("REGIAO: %s\n", city.region);
        }else{
            printf("REGIAO: nao consta!\n");
        }

        if(strcmp(city.population, " ") != 0){
            printf("POPULACAO: %s\n", city.population);
        }else{
            printf("POPULACAO: nao consta!\n");
        }

        if(strcmp(city.latitude, " ") != 0){
            printf("LATITUDE: %s\n", city.latitude);
        }else{
            printf("LATITUDE: nao consta!\n");
        }

        if(strcmp(city.longitude, " ") != 0){
            printf("LONGITUDE: %s\n", city.longitude);
        }else{
            printf("LONGITUDE: nao consta!\n");
        }

        printf("\n");
        }

    }
    fclose(arquivoDados);
}

//-------------------------------------------------------------

void imprimir(Lista *lista) {
    char pais[VARCHAR];
    printf("Digite a sigla do país que deseja ver as cidades\n");
    scanf("%s", &pais);
    NODO_PAIS *aux = lista->inicio;
    printf("Tamanho da lista: %d\n", lista->tam);
    while(aux != NULL) {
        if(strcasecmp(aux->pais, pais) == 0){
            //printf("%s, %d, %d\n", aux->pais, aux->posicao_inicial, aux->posicao_final);
            mostraCidadesPais(aux->posicao_inicial, aux->pais);
            break;
        }
        aux = aux->proximo;
    }
    printf("\n\n");
}

//-------------------------------------------------------------

void criarArquivoIndexPais(){

    char value[] = "aa";
    char row[VARCHAR];
    long posicao;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");
    FILE * arquivoIndex = fopen("arquivoIndex_pais.bin", "wb");

    CITY dados;
    CITY_INDEX_NOME index;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){
        if(strcasecmp(dados.country, value) > 0){
             strcpy(value, dados.country);
            posicao = ftell(arquivoDados);
            //posicao = ftell(arquivoDados));
            index.id = dados.id;
            strcpy(index.nome, dados.country);
            index.posicao = posicao - sizeof(CITY);


        // printf("ITEM: %d e posicao %ld \n", index.id, index.posicao);

        fwrite(&index, sizeof(CITY_INDEX_NOME), 1, arquivoIndex);
        }
    }

    // while(fread(&index, sizeof(CITY), 1, arquivoIndex) != NULL){
    //     printf("%d\n", index.id);
    // }

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

    fseek(arquivo, 0, SEEK_END);
    fim = ftell(arquivo)/sizeof(CITY_INDEX_NOME);


    CITY_INDEX_NOME index;

    fseek(arquivo, 0, SEEK_SET);
    while(inicio <= fim){
        meio = (inicio + fim) / 2;
        fseek(arquivo, meio * sizeof(CITY_INDEX_NOME), SEEK_SET);
        fread(&index, sizeof(CITY_INDEX_NOME), 1, arquivo);
        if(strcmp(pais, index.nome) > 0){
            inicio = meio+1;
        }else{
            if(strcmp(pais, index.nome) < 0){
                fim = meio - 1;
                // printf("if %d\n", meio);
            }else{

                mostraItemPais(index.posicao);
                break;
            }
        }
    }

    fclose(arquivo);

}
//-------------------------------------------------------------

void mostraItemPais(long posicao){

    FILE * arquivoDados;
    CITY city;
    char cidade[VARCHAR];

    printf("Digite o nome da cidade: ");
    scanf("%s", &cidade);

    arquivoDados = fopen("arquivoBinarioIndexado.bin","rb");
    fseek(arquivoDados, posicao, SEEK_SET);

    while(fread(&city, sizeof(CITY), 1, arquivoDados) != NULL){
        if (strcasecmp(city.name, cidade) == 0){
             printf("ID: %d\n", city.id);

    if(strcmp(city.country, " ") != 0){
        printf("PAIS: %s\n", city.country);
    }else{
        printf("PAIS: nao consta!\n");
    }

    if(strcmp(city.name, " ") != 0){
        printf("CIDADE: %s\n", city.name);
    }else{
        printf("CIDADE: nao consta!\n");
    }

    if(strcmp(city.accentCity, " ") != 0){
        printf("SOTAQUE: %s\n", city.accentCity);
    }else{
        printf("SOTAQUE: nao consta!\n");
    }

    if(strcmp(city.region, " ") != 0){
        printf("REGIAO: %s\n", city.region);
    }else{
        printf("REGIAO: nao consta!\n");
    }

    if(strcmp(city.population, " ") != 0){
        printf("POPULACAO: %s\n", city.population);
    }else{
        printf("POPULACAO: nao consta!\n");
    }

    if(strcmp(city.latitude, " ") != 0){
        printf("LATITUDE: %s\n", city.latitude);
    }else{
        printf("LATITUDE: nao consta!\n");
    }

    if(strcmp(city.longitude, " ") != 0){
        printf("LONGITUDE: %s\n", city.longitude);
    }else{
        printf("LONGITUDE: nao consta!\n");
    }
        }
    }
    fclose(arquivoDados);



}
//-------------------------------------------------------------

void criarArquivoIndexSequencial(){

    char *value;
    char row[VARCHAR];
    long posicao;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");
    FILE * arquivoIndex = fopen("arquivoIndex_campo1.bin", "wb");

    CITY dados;
    CITY_INDEX index;

    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){

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


    printf("ID: %d\n", city.id);

    if(strcmp(city.country, " ") != 0){
        printf("PAIS: %s\n", city.country);
    }else{
        printf("PAIS: nao consta!\n");
    }

    if(strcmp(city.name, " ") != 0){
        printf("CIDADE: %s\n", city.name);
    }else{
        printf("CIDADE: nao consta!\n");
    }

    if(strcmp(city.accentCity, " ") != 0){
        printf("SOTAQUE: %s\n", city.accentCity);
    }else{
        printf("SOTAQUE: nao consta!\n");
    }

    if(strcmp(city.region, " ") != 0){
        printf("REGIAO: %s\n", city.region);
    }else{
        printf("REGIAO: nao consta!\n");
    }

    if(strcmp(city.population, " ") != 0){
        printf("POPULACAO: %s\n", city.population);
    }else{
        printf("POPULACAO: nao consta!\n");
    }

    if(strcmp(city.latitude, " ") != 0){
        printf("LATITUDE: %s\n", city.latitude);
    }else{
        printf("LATITUDE: nao consta!\n");
    }

    if(strcmp(city.longitude, " ") != 0){
        printf("LONGITUDE: %s\n", city.longitude);
    }else{
        printf("LONGITUDE: nao consta!\n");
    }
}

//-------------------------------------------------------------

void pesquisaBinariaIndex(){
    int id;
    printf("\nDigite o id que voce deseja buscar: ");
    scanf("%d", &id);


    FILE *arquivo = fopen("arquivoIndex_campo1.bin", "rb");

    if(arquivo == NULL){
        printf("Nao foi possivel abrir o arquivo!\n");
    }

    int inicio, meio, fim;

    inicio = 0;
    fim = TAM_REG;

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
                break;
            }
        }
    }

    fclose(arquivo);

}

//-------------------------------------------------------------

void criarLista(Lista * lista){
    char *value;
    char row[VARCHAR];
    char nome[] = "aa";
    long posicao, posicao_inicial =  sizeof(CITY);
    int quantidade = 0;

    FILE * arquivoDados =  fopen("arquivoBinarioIndexado.bin", "rb");

    CITY dados;


    while(fread(&dados, sizeof(CITY), 1, arquivoDados) != NULL){
            quantidade++;
            if(strcasecmp(dados.country, nome) > 0){
                posicao = ftell(arquivoDados) ;
                strcpy(nome, dados.country);
                posicao_inicial = posicao;
                inserirLISTA(lista, nome, posicao_inicial, quantidade);
                quantidade = 0;

            }
            else{
                continue;
            }

    }


    fclose(arquivoDados);
}

//-------------------------------------------------------------
// *** FIM FUNCOES

// *** MAIN

int main(){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivoOriginal = fopen("worldcitiespop.csv", "r");
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

    criarArvore(&raiz);

    int valor;
    int controller = 1;

    while(controller){

        printf ("\n\nEscolha uma opcao abaixo: ");
        exibeOpcoes();

        while(scanf("%d", &valor) != NULL){
            if(valor >= 0 && valor <= 7){
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
