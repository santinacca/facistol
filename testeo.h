#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#ifndef TESTEO_H_INCLUDED
#define TESTEO_H_INCLUDED

#define ESCRIBIR 10
#define LEER 20

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
int procesarImagen(int num, char* arg[]);
int facistolInfo(char* arg);
int facistolValidar(char* arg);
void facistolHelp();
int facistolCopiar(char* arg);
int facistolLeerHeader(FILE* p, BMPFileHeader* file, BMPInfoHeader* info);
void** facistolCrearMatriz(int fil, int col, size_t elem);
void facistolDestruirMatriz(void** m, int fil);
int facistolNegativo(char* arg);
int facistolComodin(char* arg);
int facistolAumentarContraste(char* arg, int porcentaje);
int facistolDisminuirContraste(char* arg, int porcentaje);
int facistolTonalidadRoja(char* arg, int porcentaje);
int facistolTonalidadVerde(char* arg, int porcentaje);
int facistolTonalidadAzul(char* arg, int porcentaje);
int facistolGris(char* arg);
int facistolRotacion90Izquierda(char* arg);
int facistolRotacion90Derecha(char* arg);
int facistolEspejarHorizontal(char* arg);
int facistolEspejarVertical(char* arg);
int facistolConcatenarHorizontal(char* arg, char* arg2);
int facistolConcatenarVertical(char* arg, char* arg2);
int facistolContraste(int pixel, int porcentaje);
int contrasteNegativo(int pixel, int porcentaje);
int facistolValorAumento(int pixel, int porcentaje);
int facistolRecortar(char* arg, int porcentaje);
int facistolAchicar(char* arg, int porcentaje);
Pixel calculoPromedio(Pixel** m, float por, int h, int w, int hold, int wold, int32_t height, int32_t width);
void readOrWriteMatrix(int esc, int is_top_down, int32_t height, int32_t width, Pixel** mat, FILE* p, int padding);


#endif // TESTEO_H_INCLUDED
