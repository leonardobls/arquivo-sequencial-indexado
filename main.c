
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define VARCHAR 100
#define ROW 500
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

//-------------------------------------------------------------

void exibeOpcoes(){  setlocale(LC_ALL, "Portuguese");
    printf("\n1 - Listar todos os registros.\n");
    printf("2 - Realizar a pesquisa binária.\n");
    printf("3 - Pesquisar id do item pelo arquivo de indices.\n");
    printf("4 - Sair.\n\n");


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
                    printf("country %s\n", column);
                    if(column != ',') strcpy(city.country, column);
                }
                else if(j == 1){
                    printf("city %s\n", column);
                    if(column != ',') strcpy(city.name, column); 
                    
                }
                else if(j == 2){
                    printf("acce %s\n", column);
                    if(column != ',') strcpy(city.accentCity, column); 
                }
                else if(j == 3){
                    printf("region %s\n", column);
                    if(column != ',') strcpy(city.region, column);
                }
                else if(j == 4){
                    printf("population %s\n", column);
                    if(column != ',') strcpy(city.population, column);
                }
                else if(j == 5){
                    printf("lat %s\n", column);
                    if(column != ',') strcpy(city.latitude, column);
                }
                else if(j == 6){
                    printf("long %s\n", column);
                    if(column != ',') strcpy(city.longitude, column);
                }

                column = strtok(NULL, ",");
                j++;
            }
        }
        i++;

        // printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %s\nLatitude: %s\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);

	    fwrite(&city, sizeof(CITY), 1, arquivoBinario);
    }

    fclose(arquivo);
    fclose(arquivoBinario);

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
    printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %s\nLatitude: %s\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);


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

    // printf("%d", fim);

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

void exibeDadosArquivoBinario(){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivo = fopen("arquivoBinarioIndexado.bin", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    CITY city;

    printf("\n\nREGISTROS: ----------------------------\n\n");

    while(fread(&city, sizeof(CITY), 1, arquivo) != NULL){


        // printf("%d\n", city.id);
        printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %s\nLatitude: %s\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);
    }

    printf("-------------------------------------------");
    fclose(arquivo);

}

//-------------------------------------------------------------

void pesquisaBinariaSimples(){

    int id;
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

            printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %f\nLatitude: %f\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);
            break;
            }
        }
    }

    fclose(arquivo);
}

//-------------------------------------------------------------

int main(){  setlocale(LC_ALL, "Portuguese");

    FILE *arquivoOriginal = fopen("teste.csv", "r");

    if(arquivoOriginal == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    criaArquivoBinarioIndexadoOrdenado(arquivoOriginal);

    // FILE *teste = fopen("arquivoIndex_campo1.bin", "rb");

    // criarArquivoIndexSequencial();

    // CITY_INDEX key;

    // while(fread(&key, sizeof(CITY_INDEX), 1, teste) != NULL){
    //     printf("%d\n", key.id);
    // }

    int valor;
    int controller = 1;

    while(controller){
        printf ("\n\nEscolha uma opção abaixo: ");
        exibeOpcoes();
        scanf("%d", &valor);

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

             case 4 :
                controller = 0;
                printf("Tchauzin :)\n");
                break;
        }
    }
    return 0;
}