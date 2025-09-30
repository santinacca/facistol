#include "testeo.h"

void procesarImagen(int num, char* arg[]){
    printf("Entro a procesosar imagene\n");
    printf("%s\n", arg[1]);
    FILE* p = fopen(arg[1], "rb");
    printf("abrio el archivo\n");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if (!file || !info) {
        printf("Error al asignar memoria\n");
        exit(1);
    }
    printf("antes de leer\n");
    fread(file, sizeof(BMPFileHeader), 1, p);
    fread(info, sizeof(BMPInfoHeader), 1, p);
    printf("leyo el archivo\n");
    facistolValidar(file, info, arg[1]);
    facistolInfo(file, info);
    facistolCopiar(file, info, arg[1], p);
    
    fclose(p);
    free(file);
    free(info);
}

void facistolInfo(BMPFileHeader* file, BMPInfoHeader* info){
    printf("Entroa info imagen\n");
    printf("Tamanio de archivo: %" PRIu32 "\n", file->file_size);
    printf("Dimension de imagen: %" PRId32 "x %" PRId32"\n", info->width, info->height);
    printf("Profundida de color: %" PRIu16 "\n", info->bit_count);
    printf("Compresion: No comprimido\n");
    printf("Offset de datos: %" PRIu32"\n", file->offset_data);
    printf("tamanio de la imgaen: %" PRIu32"\n", info->image_size);
}

void facistolValidar(BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    printf("Validando %s\n", arg);
    if(file->file_type != 0x4D42){
        printf("tipo de archivo no valido\n");
        printf("ARCHIVO INVALIDO - No se puede procesar\n");
        exit(1);
    }else{
        printf("Signature BMP valido\n");
    }
    if(info->bit_count !=24){
        printf("Profundidad de color incorrecta (%" PRIu16 "bits, esperado 24 bits)", info->bit_count);
        printf("ARCHIVO INVALIDO - No se puede procesar\n");
        exit(1);
    }else{
        printf("Profundidad de 24 bits confirmada\n");
    }
    if(info->compression != 0){
        printf("archivo comprimido\n");
        printf("ARCHIVO INVALIDO - No se puede procesar\n");
        exit(1);
    }else{
        printf("Compresion: No comprimido\n");
    }
    printf("ARCHIVO VALIDO - Listo para procesar\n");
}
//joder
int facistolCopiar(BMPFileHeader* file, BMPInfoHeader* info, char* arg, FILE* p){
    char newName[255] = "NEW";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    Pixel** mat = malloc(height * sizeof(Pixel*));
    if(mat == NULL){
        printf("error al crear matriz\n");
        free(mat);
        fclose(fo);
        return 1;
    }
    for(int i = 0; i < height; i++){
        mat[i] = malloc(info->width * sizeof(Pixel));
        if(mat[i] == NULL){
            printf("Error al crear cada fila entera digamos\n");
            free(mat);
            fclose(fo);
            return 1;
        }
    }
    

    fseek(p, file->offset_data, SEEK_SET);

    if(is_top_down){
        for(int j = 0; j < height; j++){
            fread(mat[j], sizeof(Pixel), info->width, p);
            fseek(p, padding, SEEK_CUR);
        }
    }else{
        for(int j = height-1; j >= 0; j--){
            fread(mat[j], sizeof(Pixel), info->width, p);
            fseek(p, padding, SEEK_CUR);
        }
    }

    printf("pixel tanto tiene blue: %" PRIu8 " green: %" PRIu8 " red: %" PRIu8 "\n", mat[30][90].blue, mat[30][90].green, mat[30][90].red);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(mat[k], sizeof(Pixel), info->width, fo);
            // fwrite("0x00", sizeof("0x00"), padding, fo);
            // for (int i = 0; i < padding; i++) fputc(0x00, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(mat[k], sizeof(Pixel), info->width, fo);
            // fwrite("0x00", sizeof("0x00"), padding, fo);
            // for (int i = 0; i < padding; i++) fputc(0x00, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    for (int i = 0; i < height; i++) {
        free(mat[i]);
    }
    free(mat);
    fclose(fo);
    return 0;
}