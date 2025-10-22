#include <stdio.h>
#include <stdlib.h>
#include "testeo.c"

int main(int argc, char *argv[])
{
    procesarImagen(argc, argv);
    printf("finish\n");
    return 0;
}
/*
FILE* p = fopen(arg, "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        exit(1);
    }
    char newName[255] = "facistol_tonalidad_roja";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p, file, info);
    
    int32_t height = abs(info->height);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int size = abs(info->height) * (info->width);

    Pixel** m = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    fseek(p, file->offset_data, SEEK_SET);
    readOrWriteMatrix(LEER, is_top_down, height, info->width, m, p, padding);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);


    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);

    facistolDestruirMatriz((void**)m, height);
    fclose(p);
    free(file);
    free(info);
    fclose(fo);*/

/*void facistolAchicar(Pixel** mat, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
if(porcentaje < 1 || porcentaje > 100){
printf("Porcentaje no valido\n");
porcentaje = 100;  // Valor por defecto
}
float factor = (100.0 / (float)porcentaje);  // Factor de escalado

char newName[255] = "achicado_";
strcat(newName, arg);
FILE* fo = fopen(newName, "wb");  // Modo binario
if (fo == NULL){
    printf("error al abrir el nuevo archivo\n");
    exit(1);
}
int32_t origenH = abs(info->height);
int32_t origenW = info->width;

int size = abs(info->height) * (info->width);
int is_top_down = (info->height < 0);

int32_t height = (int32_t)(origenH / factor);  // Altura nueva
int32_t width = (int32_t)(origenW / factor);   // Ancho nuevo

Pixel** newMat = (Pixel**)facistolCrearMatriz(height, width, sizeof(Pixel));

for(int h = 0; h < height; h++){
    for(int w = 0; w < width; w++){
        newMat[h][w] = calculoPromedio(mat, factor, h, w, origenH, origenW, height, width);  // Pasa factor correcto
    }
}

int padding = (4 - (width * 3) % 4) % 4;
uint32_t imageSizeNew = (uint32_t)(height * (width * 3 + padding));
uint32_t fileSizeNew = file->offset_data + imageSizeNew;

file->file_size = fileSizeNew;
info->image_size = imageSizeNew;

if(is_top_down){
    info->height = 0 - height;  // Mantener el signo
}else{
    info->height = height;
}
info->width = width;

fwrite(file, sizeof(BMPFileHeader), 1, fo);
fwrite(info, sizeof(BMPInfoHeader), 1, fo);
fseek(fo, file->offset_data, SEEK_SET);

if(is_top_down){
    for(int k = 0; k < height; k++){
        fwrite(newMat[k], sizeof(Pixel), info->width, fo);
        for (int i = 0; i < padding; i++) {
            fputc(0x00, fo);
        }
    }
}else{
    for(int k = height -1; k >= 0; k--){
        fwrite(newMat[k], sizeof(Pixel), info->width, fo);
        for (int i = 0; i < padding; i++) {
            fputc(0x00, fo);
        }
    }
}

facistolDestruirMatriz((void**)mat, origenH);
facistolDestruirMatriz((void**)newMat, height);
fclose(fo); */