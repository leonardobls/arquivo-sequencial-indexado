#include <stdio.h>

int main(){

    FILE *arquivo = fopen("worldcitiespop.csv", "rb");

    if(arquivo == NULL){
        printf("Não foi possível abrir o arquivo!\n");
    }

    int inteiro;
    char value;

    int j = 0, i;

    // printf("%s", word);

    for(i = 0; i <= 6; i++){
        char word[50] = "\0";
        while(fread(&value, sizeof(value), 1, arquivo) != NULL){
            if(value == ','){
            break;
            }
            word[j] = value;
            j++;
        }
        printf("%s", word);
    }

}
