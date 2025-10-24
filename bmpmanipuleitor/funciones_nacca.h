#ifndef FUNCIONES_NACCA_H_INCLUDED
#define FUNCIONES_NACCA_H_INCLUDED
// Apellido, nombre: Nacca, Santino
// DNI: 47170837
// Entrega: Si

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>


#define ESCRIBIR 10
#define LEER 20

#pragma pack(push, 1)
typedef struct {
    uint16_t file_type;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset_data;
} BMPFileHeader;

typedef struct {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
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


void readOrWriteMatrix(int esc, int is_top_down, int32_t height, int32_t width, Pixel** mat, FILE* p, int padding);
int validarImagen(char* arg);
int facistolLeerHeader(FILE* p, BMPFileHeader* file, BMPInfoHeader* info);
void** facistolCrearMatriz(int fil, int col, size_t elem);
void facistolDestruirMatriz(void** m, int fil);
int facistolAumentarContraste(char* arg, int porcentaje,bool verb,bool* segundoverb);
int facistolDisminuirContraste(char* arg, int porcentaje,bool verb,bool* segundoverb);
int contrastePositivo(int pixel, int porcentaje);
int contrasteNegativo(int pixel, int porcentaje);
int facistolGris(char* arg,bool verb,bool* segundoverb);
int facistolComodin(char* arg,bool verb,bool* segundoverb);
void facistolHelp();
int facistolConcatenarVertical(char* arg, char* arg2,bool verb,bool* segundoverb);
int facistolRotacion90Derecha(char* arg,bool verb,bool* segundoverb);
int facistolEspejarVertical(char* arg,bool verb,bool* segundoverb);
int facistolRecortar(char* arg, int porcentaje,bool verb,bool* segundoverb);

#endif // FUNCIONES_NACCA_H_INCLUDED
