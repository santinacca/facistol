#ifndef FUNCIONES_ZAMBELLA_H_INCLUDED
#define FUNCIONES_ZAMBELLA_H_INCLUDED
// Apellid, nombre(s): Zambella, Lautaro Ivan
// DNI: 45072269
// Entrega: Si

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include "funciones_nacca.c"

#define ESCRIBIR 10
#define LEER 20

int facistolTonalidadRoja(char* arg, int porcentaje,bool verb,bool* segundoverb);
int facistolTonalidadVerde(char* arg, int porcentaje,bool verb,bool* segundoverb);
int facistolTonalidadAzul(char* arg, int porcentaje,bool verb,bool* segundoverb);
int valorAumento(int pixel, int porcentaje);
int facistolNegativo(char* arg,bool verb,bool* segundoverb);
int facistolInfo(char* arg);
int facistolValidar(char* arg);
int facistolConcatenarHorizontal(char* arg, char* arg2,bool verb,bool* segundoverb);
int facistolRotacion90Izquierda(char* arg,bool verb,bool* segundoverb);
int facistolEspejarHorizontal(char* arg,bool verb,bool* segundoverb);
int facistolAchicar(char* arg, int porcentaje,bool verb,bool* segundoverb);
Pixel calculoPromedio(Pixel** m, float por, int h, int w, int hold, int wold, int32_t height, int32_t width);

#endif // FUNCIONES_ZAMBELLA_H_INCLUDED
