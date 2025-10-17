#include <stdio.h>
#include <stdlib.h>
#include "testeo.c"

int main(int argc, char *argv[])
{
    procesarImagen(argc, argv);
    printf("finish\n");
    return 0;
}

// Pixel calculoPromedio(Pixel** mat, int factor, int h_orig, int w_orig, int h_new, int w_new) {
//     // factor: Cuántos píxeles de la imagen original corresponden a un píxel de la nueva imagen
//     // h_new, w_new: Coordenadas del píxel en la NUEVA matriz
    
//     // Calcula la esquina superior izquierda del bloque en la imagen original
//     int start_h = h_new * factor;
//     int start_w = w_new * factor;
    
//     // Determina el final del bloque a promediar, asegurando no exceder los límites
//     // de la imagen original (h_orig, w_orig).
//     int end_h = start_h + factor < h_orig ? start_h + factor : h_orig;
//     int end_w = start_w + factor < w_orig ? start_w + factor : w_orig;

//     long acuRed = 0;
//     long acuGreen = 0;
//     long acuBlue = 0;
//     int count = 0; // Contador de píxeles promediados

//     for (int i = start_h; i < end_h; i++) {
//         for (int j = start_w; j < end_w; j++) {
//             acuRed += mat[i][j].red;
//             acuGreen += mat[i][j].green;
//             acuBlue += mat[i][j].blue;
//             count++;
//         }
//     }

//     Pixel devolver;
//     if (count > 0) {
//         // CORRECCIÓN: Dividir por el número TOTAL de píxeles promediados (count),
//         // no por 3 (número de canales).
//         devolver.red = (uint8_t)(acuRed / count);
//         devolver.green = (uint8_t)(acuGreen / count);
//         devolver.blue = (uint8_t)(acuBlue / count);
//     } else {
//         // En caso de error (no debería pasar con límites correctos)
//         devolver.red = devolver.green = devolver.blue = 0;
//     }
    
//     return devolver;
// }