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
    float latitude;
    float longitude;
}CITY;

//-------------------------------------------------------------

void exibeOpcoes(){  setlocale(LC_ALL, "Portuguese");
    printf("\n1 - Listar todos os registros.\n");
    printf("2 - Realizar a pesquisa binária.\n");
    printf("3 - Sair.\n\n");
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
                    // printf("country %s\n", column);
                    strcpy(city.country, column);
                } 
                else if(j == 1){
                    // printf("city %s\n", column);
                    strcpy(city.name, column);
                } 
                else if(j == 2){
                    // printf("acce %s\n", column);
                    strcpy(city.accentCity, column);
                }
                else if(j == 3){
                    // printf("region %s\n", column);
                    strcpy(city.region, column);
                }
                else if(j == 4){
                    // printf("population %s\n", column);
                    strcpy(city.population, column);
                }
                else if(j == 5){
                    // printf("lat %s\n", column);
                    city.latitude = atof(column);
                }
                else if(j == 6){
                    // printf("long %s\n", column);
                    city.longitude = atof(column);
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

    printf("\n\nREGISTROS: ----------------------------\n\n");

    while(fread(&city, sizeof(CITY), 1, arquivo) != NULL){


        // printf("%d\n", city.id);
        printf("Id: %d\nCidade: %s\nPaís: %s\nRegião: %s\nSotaque: %s\nPopulação: %s\nLongitude: %f\nLatitude: %f\n\n", city.id, city.name, city.region, city.accentCity, city.population, city.longitude, city.latitude);
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
                
                printf("%d", city.id);
                break;
            }
        }
    }
}

//-------------------------------------------------------------

int main(){  setlocale(LC_ALL, "Portuguese");
    
    FILE *arquivoOriginal = fopen("teste.csv", "r");

    if(arquivoOriginal == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    criaArquivoBinarioIndexadoOrdenado(arquivoOriginal);

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

             case 3 :
                controller = 0;
                printf("Tchauzin :)\n");
                break;
        }
    }
    
    return 0;
}
