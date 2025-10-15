#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#ifndef TESTEO_H_INCLUDED
#define TESTEO_H_INCLUDED
#pragma pack(push, 1)
typedef struct {
    uint16_t file_type;     // Debe ser 0x4D42 ('BM')
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset_data;   // Offset a los datos de la imagen
} BMPFileHeader;

typedef struct {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;     // Profundidad de color, debe ser 24
    uint32_t compression;   // Debe ser 0 para no comprimido
    uint32_t image_size;
    int32_t x_pixels_per_m;
    int32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
} BMPInfoHeader;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;
#pragma pack(pop)
void procesarImagen(int num, char* arg[]);
void facistolInfo(BMPFileHeader* file, BMPInfoHeader* info);
void facistolValidar(BMPFileHeader* file, BMPInfoHeader* info, char* arg);
int facistolCopiar(BMPFileHeader* file, BMPInfoHeader* info, char* arg, FILE* p, Pixel** mat);
int facistolLeerHeader(FILE* p, BMPFileHeader* file, BMPInfoHeader* info);
void** facistolCrearMatriz(int fil, int col, size_t elem);
void facistolDestruirMatriz(void** m, int fil);
void facistolNegativo(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolAumentarContraste(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);
void facistolDisminuirContraste(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);
void facistolTonalidadRoja(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);
void facistolTonalidadVerde(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);
void facistolTonalidadAzul(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);
void facistolGris(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolRotacion90Izquierda(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolRotacion90Derecha(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolEspejarHorizontal(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolEspejarVertical(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg);
void facistolConcatenarHorizontal(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, char* arg2, BMPFileHeader* file2, BMPInfoHeader* info2, Pixel** mat2);
void facistolConcatenarVertical(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, char* arg2, BMPFileHeader* file2, BMPInfoHeader* info2, Pixel** mat2);
int facistolContraste(int pixel, int porcentaje);
int contrasteNegativo(int pixel, int porcentaje);
int facistolValorAumento(int pixel, int porcentaje);
void facistolRecortar(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje);


#endif // TESTEO_H_INCLUDED
