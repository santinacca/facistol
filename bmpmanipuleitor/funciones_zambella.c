#include "funciones_zambella.h"

#define EXITO 0
#define ERROR_ARG 1
#define ERROR_ARCH 2
#define ERROR_MEM 3
#define BMP_INVALIDO 4
#define MAXLETRAS 255


int facistolTonalidadRoja(char* arg, int porcentaje,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }

    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_tonalidad_roja_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",info->width,info->height);
    }
    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: tonalidad roja\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red=valorAumento(m[h][k].red, porcentaje);   //TONALIDAD ROJA
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro tonalidad roja completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);
    return EXITO;
}

int facistolTonalidadAzul(char* arg, int porcentaje,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_tonalidad_azul_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",info->width,info->height);
    }
    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: tonalidad azul\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].blue=valorAumento(m[h][k].blue, porcentaje);   //TONALIDAD AZUL
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro tonalidad azul completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);
    return EXITO;
}

int facistolTonalidadVerde(char* arg, int porcentaje,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_tonalidad_verde_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",info->width,info->height);
    }
    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: tonalidad verde\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].green=valorAumento(m[h][k].green, porcentaje);   //TONALIDAD VERDE
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro tonalidad verde completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);
    return EXITO;
}

int valorAumento(int pix, int porcentaje){
    float aumento = porcentaje/100.0;
    int nuevoValor = (int)(pix + (pix*aumento));
    if(nuevoValor > 255){
        return 255;
    }else{
        return nuevoValor;
    }
}

int facistolNegativo(char* arg,bool verb,bool* segundoverb)
{
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;

    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }

    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_negativo_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",info->width,info->height);
    }
    Pixel** mat = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(mat==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, mat, p, padding);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: negativo\n");
    }


    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            mat[h][k].blue=255-mat[h][k].blue;
            mat[h][k].red=255-mat[h][k].red;   //FILTRO NEGATIVO
            mat[h][k].green=255-mat[h][k].green;
        }
    }

    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, mat, fo, padding);

    if(verb==true && propioverb==true)
    {
        printf("[INFO] Filtro negativo completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)mat, height);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);
    return EXITO;
}

int facistolInfo(char* arg){
    FILE* p = fopen(arg, "rb");
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p, file, info);
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    printf("Archivo: %s\n",arg);
    printf("Tamanio de archivo: %" PRIu32 "\n", file->file_size);
    printf("Dimension de imagen: %" PRId32 "x %" PRId32"\n", info->width, info->height);
    printf("Profundida de color: %" PRIu16 "\n", info->bit_count);
    printf("Compresion: No comprimido\n");
    printf("Offset de datos: %" PRIu32"\n", file->offset_data);
    printf("Tamanio de la imgaen: %" PRIu32"\n", info->image_size);
    fclose(p);
    free(file);
    free(info);
    return EXITO;
}

int facistolValidar(char* arg){
    FILE* p = fopen(arg, "rb");
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p, file, info);
    printf("Validando %s\n", arg);
    if(file->file_type != 0x4D42){
        printf("Tipo de archivo no valido\n");
        printf("ARCHIVO INVALIDO - No se puede procesar\n");
        return BMP_INVALIDO;
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
        printf("Archivo comprimido\n");
        printf("ARCHIVO INVALIDO - No se puede procesar\n");
        exit(1);
    }else{
        printf("Compresion: No comprimido\n");
    }
    printf("ARCHIVO VALIDO - Listo para procesar\n");
    fclose(p);
    free(file);
    free(info);
    return EXITO;
}

int facistolConcatenarHorizontal(char* arg, char* arg2,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    FILE* p2 = fopen(arg2, "rb");
    if(p2 == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newname[255] = "facistol_concatenacion_horizontal_";
    strcat(newname, arg);
    strcat(newname, "_");
    strcat(newname, arg2);

    FILE* fo = fopen(newname, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    //int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);

    BMPFileHeader* file2 = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info2 = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p2, file2, info2);

    //int32_t height2 = abs(info2->height);
    int is_top_down2 = (info2->height < 0);
    int padding2 = (4 - (info2->width * 3) % 4) % 4;
    //int size2 = abs(info2->height) * (info2->width);

    BMPFileHeader* newFile = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* newInfo = malloc(sizeof(BMPInfoHeader));



    if(file->offset_data >= file2->offset_data){
        *newFile = *file;
        *newInfo = *info;
    }else{
        *newFile = *file2;
        *newInfo = *info2;
    }

    newInfo->width = info->width + info2->width;
    if(abs(info->height) >= abs(info2->height)){
        newInfo->height = info->height;
    }else{
        newInfo->height = info2->height;
    }

    int newSize = abs(newInfo->height) * newInfo->width;
    int newIs_top_down = (newInfo->height < 0);

    int32_t newHeight = abs(newInfo->height);
    int32_t newWidth = newInfo->width;
    int newPadding = (4 - (newWidth * 3) % 4) % 4;

    newInfo->image_size = (uint32_t)newSize;
    newFile->file_size = newInfo->image_size + newInfo->header_size;
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",newWidth,newHeight);
    }
    fwrite(newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, newFile->offset_data, SEEK_SET);

    Pixel** newMat = (Pixel**)facistolCrearMatriz(newHeight, newWidth, sizeof(Pixel));
    if(newMat==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(newInfo->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    Pixel** m = (Pixel**)facistolCrearMatriz(abs(info->height), info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, abs(info->height), info->width, m, p, padding);

    Pixel** m2 = (Pixel**)facistolCrearMatriz(abs(info2->height), info2->width, sizeof(Pixel));
    if(m2==NULL)
    {
        return ERROR_MEM;
    }
    fseek(p2, file2->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down2, abs(info2->height), info2->width, m2, p2, padding2);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: concatenar horizontal\n");
    }
    for(int w = 0; w < info->width; w++){
        for(int h = 0; h < abs(info->height); h++){
            newMat[h][w] = m[h][w];
        }
    }

    if(abs(info->height) >= abs(info2->height)){
        for(int h2 = 0; h2 < abs(info2->height); h2++){
            for(int w2 = 0; w2 < info2->width; w2++){
                newMat[h2][info->width + w2] = m2[h2][w2];
                }
        }
        for(int h3 = abs(info2->height); h3 < newHeight; h3++){
            for(int w3 = info->width; w3 < newWidth; w3++){
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 255;
                newMat[h3][w3].green = 0;
            }
        }
    }else{
        for(int h3 = abs(info->height); h3 < newHeight; h3++){
            for(int w3 = 0; w3 < info->width; w3++){
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 255;
                newMat[h3][w3].green = 0;
            }
        }
        for(int h2 = 0; h2 < abs(newHeight); h2++){
            for(int w2 = 0; w2 < info2->width; w2++){
                newMat[h2][info->width + w2] = m2[h2][w2];
                }
            }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newname);
    }
    readOrWriteMatrix(ESCRIBIR, newIs_top_down, newHeight, newWidth, newMat, fo, newPadding);
    if(verb)
    {
        printf("[INFO] Filtro concatenar horizontal completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)newMat, newHeight);
    facistolDestruirMatriz((void**)m, abs(info->height));
    facistolDestruirMatriz((void**)m2, abs(info2->height));
    free(newFile);
    free(newInfo);
    fclose(p);
    fclose(p2);
    free(file);
    free(info);
    free(file2);
    free(info2);
    fclose(fo);
    return EXITO;
}

int facistolRotacion90Izquierda(char* arg,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_90Grados_Izquierda_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);

    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);

    BMPFileHeader newFile = *file;
    BMPInfoHeader newInfo = *info;

    newInfo.width = info->height;
    newInfo.height = info->width;

    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",newInfo.height,newInfo.width);
    }

    fwrite(&newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(&newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel** newMat = (Pixel**)facistolCrearMatriz(info->width, height, sizeof(Pixel));
    if(newMat==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(newInfo.image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: rotar 90 grados a la izquierda\n");
    }
    for(int x=0;x<height;x++)
    {
        for(int y=0;y<info->width;y++)
        {

            newMat[info->width-1-y][x]=m[x][y];
        }

    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    int newPadding = (4 - (newInfo.width * 3) % 4) % 4;

    readOrWriteMatrix(ESCRIBIR, is_top_down, newInfo.height, newInfo.width, newMat, fo, newPadding);

    if(verb)
    {
        printf("[INFO] Filtro rotacion 90 grados a la izquierda completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    facistolDestruirMatriz((void**)newMat, info->width);
    fclose(fo);
    fclose(p);
    free(file);
    free(info);
    return EXITO;
}

int facistolEspejarHorizontal(char* arg,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_espejado_horizontal_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",info->width,info->height);
    }
    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: espejar horizontal\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel* vec = malloc(height*sizeof(Pixel));
    if(vec == NULL){
        free(vec);
        printf("error al crear vec\n");
        return ERROR_MEM;
        exit(1);
    }

    for(int h=0;h<(info->width)/2;h++)
    {
        for(int k=0;k<height;k++){
            vec[k] = m[k][info->width-h-1];
            m[k][info->width-h-1] = m[k][h];
            m[k][h] = vec[k];
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro espejar horizontal completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    free(file);
    free(info);
    free(vec);
    fclose(p);
    fclose(fo);
    return EXITO;
}

int facistolAchicar(char* arg, int porcentaje,bool verb,bool* segundoverb){
    if(validarImagen(arg)==-1)
    {
        return BMP_INVALIDO;
    }
    bool propioverb=false;
    if(*segundoverb==false)
    {
        propioverb=true;
        *segundoverb=true;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Cargando archivo: %s\n",arg);

    }
    if(porcentaje < 1 || porcentaje > 100){
        printf("Porcentaje no valido\n");
        porcentaje = 100;
    }
    float factor = (100.0 / (float)porcentaje);
    FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    char newName[255] = "facistol_achicar_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        return ERROR_ARCH;
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Validando header BMP...\n");
    }
    facistolLeerHeader(p, file, info);

    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);

    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    if(m==NULL)
    {
        return ERROR_MEM;
    }
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);


    int32_t origenH = abs(info->height);
    int32_t origenW = info->width;

    int32_t newheight = (int32_t)(origenH / factor);  // Altura nueva
    int32_t newwidth = (int32_t)(origenW / factor);   // Ancho nuevo
    int newpadding = (4 - (newwidth * 3) % 4) % 4;
    uint32_t imageSizeNew = (uint32_t)(newheight * (newwidth * 3 + newpadding));
    uint32_t fileSizeNew = file->offset_data + imageSizeNew;
    file->file_size = fileSizeNew;
    info->image_size = imageSizeNew;
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Archivo válido - Dimensiones: %"PRId32"x%"PRIu32", Tamanio: %d bytes\n",info->width,info->height,size);
        printf("[INFO] Reservando memoria para matriz %"PRId32"x%"PRIu32"\n",newwidth,newheight);
    }
    Pixel** newMat = (Pixel**)facistolCrearMatriz(newheight, newwidth, sizeof(Pixel));
    if(newMat==NULL)
    {
        return ERROR_MEM;
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Memoria reservada exitosamente (%d pixeles)\n",(int)(info->image_size/3));
        printf("[INFO] Leyendo datos de imagen...\n");
    }
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: achicar\n");
    }
    for(int indexH = 0; indexH < newheight; indexH++){
        for(int indexW = 0; indexW < newwidth; indexW++){
            newMat[indexH][indexW] = calculoPromedio(m, factor, indexH, indexW, origenH, origenW, newheight, newwidth);
        }
    }
    if(is_top_down){
        info->height = 0 - newheight;
    }else{
        info->height = newheight;
    }
    info->width = newwidth;

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, newheight, info->width, newMat, fo, newpadding);
    if(verb)
    {
        printf("[INFO] Filtro achicar completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    facistolDestruirMatriz((void**)newMat, newheight);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);
    return EXITO;
}

Pixel calculoPromedio(Pixel** mat, float factor, int indexH, int indexW, int originalH, int originalW, int32_t newheight, int32_t newwidth){
        int acuRed = 0, acuGreen = 0, acuBlue = 0;
        int cantPixeles = 0;
        int startH = (int)((indexH + 0.5) * (originalH / (float)newheight));
        int startW = (int)((indexW + 0.5) * (originalW / (float)newwidth));
        int endH = (int)((indexH + 0.5) * (originalH / (float)newheight) + 1.0);
        int endW = (int)((indexW + 0.5) * (originalW / (float)newwidth) + 1.0);
        if (startH < 0){
            startH = 0;
        }
        if (startW < 0){
            startW = 0;
        }
        if (endH >= originalH){
            endH = originalH;
        }

        if (endW >= originalW){
            endW = originalW;
        }
        for (int i = startH; i < endH; i++) {
            for (int j = startW; j < endW; j++) {
                acuRed += mat[i][j].red;
                acuGreen += mat[i][j].green;
                acuBlue += mat[i][j].blue;
                cantPixeles++;
            }
        }
        Pixel devolver;
        if (cantPixeles > 0) {
            devolver.red = (uint8_t)(acuRed / cantPixeles);
            devolver.green = (uint8_t)(acuGreen / cantPixeles);
            devolver.blue = (uint8_t)(acuBlue / cantPixeles);
        } else {
            devolver.red = 0;
            devolver.green = 0;
            devolver.blue = 0;
        }
        return devolver;
}

