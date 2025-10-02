#include "testeo.h"

void procesarImagen(int num, char* arg[]){
    FILE* p = fopen(arg[1], "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p, file, info);
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
    printf("tipo de archivo: %" PRIu16 "\n", file->file_type);
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

    // Pixel** mat = malloc(height * sizeof(Pixel*));
    // if(mat == NULL){
    //     printf("error al crear matriz\n");
    //     free(mat);
    //     fclose(fo);
    //     return 1;
    // }
    // for(int i = 0; i < height; i++){
    //     mat[i] = malloc(info->width * sizeof(Pixel));
    //     if(mat[i] == NULL){
    //         printf("Error al crear cada fila entera digamos\n");
    //         free(mat);
    //         fclose(fo);
    //         return 1;
    //     }
    // }

    Pixel** mat = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    

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

    // for (int i = 0; i < height; i++) {
    //     free(mat[i]);
    // }
    // free(mat);
    facistolDestruirMatriz((void**)mat, height);
    fclose(fo);
    return 0;
}
// ostias
int facistolLeerHeader(FILE* p, BMPFileHeader* file, BMPInfoHeader* info){
    if(file == NULL || info == NULL){
        printf("error al asignar memoria\n");
        return 3;
    }
    fread(file, sizeof(BMPFileHeader), 1, p);
    fread(info, sizeof(BMPInfoHeader), 1, p);
}
//jolines
void** facistolCrearMatriz(int fil, int col, size_t elem){
    void** m = malloc(fil*sizeof(void*));
    if(!m){
        printf("error al crear matriz");
        return NULL;
    }
    void** ult = m + (fil-1);
    for(void** i = m; i <= ult; i++){
        *i = malloc(col*elem);
        if(!*i){
            facistolDestruirMatriz(m, i-m);
            return NULL;
        }
    }
    return m;
}

void facistolDestruirMatriz(void** m, int fil){
    void** ult = m + (fil-1);
    for(void** i = m; i < ult; i++){
        free(*i);
    }
    free(m);
}