#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
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

#pragma pack(pop)

// Estructura para representar un píxel BGR de 24 bits
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;


int main()
{
    FILE* file=fopen("out.bmp","rb");
    if(file==NULL)
    {
        printf("error al abrir el archivo");
        exit(1);
    }
    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    fread(&file_header, sizeof(BMPFileHeader), 1, file);
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);
    printf( "%" PRIu32 "\n",info_header.bit_count);


    if (file_header.file_type != 0x4D42 || info_header.bit_count != 24) 
    {
        printf("Error: Archivo BMP no válido o no es de 24 bits.\n");
        fclose(file);
        return 2;
    }
    int width = info_header.width;
    int height = abs(info_header.height);//int height = abs(info_header.height); 
    int padding = (4 - (width * 3) % 4) % 4;
    Pixel** image_data = (Pixel**)malloc(height * sizeof(Pixel*));
    if (!image_data) 
    {
        printf("Error de memoria.\n");
        fclose(file);
        return 3;
    }
    for (int i = 0; i < height; i++) 
    {
        image_data[i] = (Pixel*)malloc(width * sizeof(Pixel));
        if (!image_data[i]) 
        {
            printf("Error de memoria.\n");
            // Liberar memoria ya asignada
            for (int j = 0; j < i; j++) {
                free(image_data[j]);
            }
            free(image_data);
            fclose(file);
            return 3;
        }
    }
    fseek(file, file_header.offset_data, SEEK_SET);

    for (int y = 0; y < height; y++) 
    {
        fread(image_data[y], sizeof(Pixel), width, file);

        fseek(file, padding, SEEK_CUR); // Ignorar los bytes de padding
    }

    /*image_data[0][0].blue=0;
    image_data[1][0].blue=0;
    image_data[0][0].red=255;
    image_data[1][0].red=255;
    image_data[0][1].blue=255;
    image_data[0][1].red=255;
    image_data[0][1].green=255;
    image_data[1][1].blue=255;
    image_data[1][1].red=255;
    image_data[1][1].green=255;*/
    
    /*
    int gris;
    for(int h=0;h<height;h++)
    {
        for(int k=0;k<width;k++)
        {
            gris=(image_data[h][k].blue + image_data[h][k].green + image_data[h][k].red)/3;   //FILTRO GRIS
            image_data[h][k].blue=gris;
            image_data[h][k].red=gris;
            image_data[h][k].green=gris;
        }
    }

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<width;k++)
        {
            image_data[h][k].blue=255-image_data[h][k].blue;
            image_data[h][k].red=255-image_data[h][k].red;   //FILTRO NEGATIVO
            image_data[h][k].green=255-image_data[h][k].green;
        }
    }
    */

    /*for(int h=0;h<height;h++)
    {
        for(int k=0;k<width;k++)
        {
            image_data[h][k]=image_data[height-1-h][k]; //SE ESPEJA LA MITAD
        }
    }*/
    
    
   
    for(int h=0;h<height;h++) 
    { 
        for(int k=0;k<width/2;k++) 
        { 
            image_data[h][k]=image_data[width-1-k][h]; 
        } 
    }





    fclose(file);

    // Recalcular y actualizar los campos de tamaño en la cabecera
    uint32_t row_bytes = (width * 3) + padding;
    uint32_t image_data_size = row_bytes * height;
    uint32_t total_size = file_header.offset_data + image_data_size; 

    // Actualizar la cabecera del archivo (BMPFileHeader)
    file_header.file_size = total_size;

    // Actualizar la cabecera de información (BMPInfoHeader)
    info_header.image_size = image_data_size;

    FILE* new_file = fopen("outrotada.bmp", "wb");
    if (!new_file)
    {
        perror("Error al crear el archivo de salida");
        return 1;
    }

    fwrite(&file_header, sizeof(BMPFileHeader), 1, new_file);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, new_file);
    fseek(new_file, file_header.offset_data, SEEK_SET);
    // Escribir los datos de los píxeles
    for (int y = 0; y < height; y++) {
        fwrite(image_data[y], sizeof(Pixel), width, new_file);
        // Escribir el padding
        for (int i = 0; i < padding; i++) {
            fputc(0x00, new_file);
        }
    }
    fclose(new_file);
    


    Pixel** m=image_data;
    Pixel** ult = m + ( height - 1);

    for(Pixel** i = m; i <= ult; i++)
    {
        free(*i);
    }

    free(m);
   
    return 0;

}