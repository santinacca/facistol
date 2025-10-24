#include "funciones_nacca.h"

#define EXITO 0
#define ERROR_ARG 1
#define ERROR_ARCH 2
#define ERROR_MEM 3
#define BMP_INVALIDO 4
#define MAXLETRAS 255

void readOrWriteMatrix(int esc, int is_top_down, int32_t height, int32_t width, Pixel** mat, FILE* p, int padding){
    if(esc == LEER){
        if(is_top_down){
            for(int j = 0; j < height; j++){
                fread(mat[j], sizeof(Pixel), width, p);
                fseek(p, padding, SEEK_CUR);
            }}else{
                for(int j = height-1; j >= 0; j--){
                    fread(mat[j], sizeof(Pixel), width, p);
                    fseek(p, padding, SEEK_CUR);
                }
            }
    }else{
        if(is_top_down){
            for(int k = 0; k < height; k++){
                fwrite(mat[k], sizeof(Pixel), width, p);
                for (int i = 0; i < padding; i++) {
                    fputc(0x00, p);
                }
            }
        }else{
            for(int k = height -1; k >= 0; k--){
                fwrite(mat[k], sizeof(Pixel), width, p);
                for (int i = 0; i < padding; i++) {
                    fputc(0x00, p);
                }
            }
        }
    }
}

int validarImagen(char* arg)
{
    FILE* p = fopen(arg, "rb");
    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    facistolLeerHeader(p, file, info);
    int valido=0;
    if(file->file_type != 0x4D42)
    {
        printf("El archivo no es BMP\n");
        valido=-1;
        return valido;
    }
    if(info->bit_count !=24)
    {
        printf("El archivo no es de 24 bits\n");
        valido=-1;
        return valido;
    }
    if(info->compression != 0)
    {
        printf("El archivo es comprimido\n");
        valido=-1;
        return valido;
    }

    fclose(p);
    free(file);
    free(info);
    return EXITO;
}

int facistolLeerHeader(FILE* p, BMPFileHeader* file, BMPInfoHeader* info){
    if(file == NULL || info == NULL){
        printf("Error al asignar memoria\n");
        return ERROR_MEM;
    }
    fread(file, sizeof(BMPFileHeader), 1, p);
    fread(info, sizeof(BMPInfoHeader), 1, p);
    return EXITO;
}

void** facistolCrearMatriz(int fil, int col, size_t elem){
    void** m = malloc(fil*sizeof(void*));
    if(!m){
        printf("Error al crear matriz");
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

int facistolAumentarContraste(char* arg, int porcentaje,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_aumentar_contraste_";
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
        printf("[INFO] Aplicando filtro: aumentar contraste\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red = contrastePositivo(m[h][k].red, porcentaje);
            m[h][k].blue = contrastePositivo(m[h][k].blue, porcentaje);  //CONTRASTE AUMENTO
            m[h][k].green = contrastePositivo(m[h][k].green, porcentaje);
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro aumentar contraste completado exitosamente\n");
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

int facistolDisminuirContraste(char* arg, int porcentaje,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_disminuir_contraste_";
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
        printf("[INFO] Aplicando filtro: disminuir contraste\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red = contrasteNegativo(m[h][k].red, porcentaje);
            m[h][k].blue = contrasteNegativo(m[h][k].blue, porcentaje);
            m[h][k].green = contrasteNegativo(m[h][k].green, porcentaje);
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro disminuir contraste completado exitosamente\n");
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

int contrastePositivo(int pix, int por)
{
    float factor= 1.0 + ((float)por/100);
    float nuevoPixel = (pix - 128) * factor + 128;
    int valorFinal = (int)nuevoPixel;
    if(valorFinal > 255){
        return 255;
    }else if(valorFinal < 0){
        return 0;
    }else{
        return valorFinal;
    }
}

int contrasteNegativo(int pix, int por)
{
    float factor= 1.0 - ((float)por/100);
    float nuevoPixel = (pix - 128) * factor + 128;
    int valorFinal = (int)nuevoPixel;
    if(valorFinal > 255){
        return 255;
    }else if(valorFinal < 0){
        return 0;
    }else{
        return valorFinal;
    }
}

int facistolGris(char* arg,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_gris_";
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
        printf("[INFO] Aplicando filtro: escala de grises\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    int gris;
    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++)
        {
            gris=(m[h][k].blue + m[h][k].green + m[h][k].red)/3;   //FILTRO GRIS
            m[h][k].blue=gris;
            m[h][k].red=gris;
            m[h][k].green=gris;
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro escala de grises completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    free(file);
    free(info);
    fclose(p);
    fclose(fo);
    return EXITO;
}

int facistolComodin(char* arg,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_comodin_";
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
        printf("[INFO] Aplicando filtro: comodin\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++)
        {
            int red = m[h][k].red;
            int green = m[h][k].green;
            int blue = m[h][k].blue;


            int new_blue = round(.272 * red + .534 * green + .131 * blue);
            int new_red = round(.393 * red + .769 * green + .189 * blue);
            int new_green = round(.349 * red + .686 * green + .168 * blue);


            if (new_red > 255) new_red = 255;
            if (new_green > 255) new_green = 255;
            if (new_blue > 255) new_blue = 255;


            m[h][k].red = new_red;
            m[h][k].green = new_green;
            m[h][k].blue = new_blue;
        }
    }

    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro comodin completado exitosamente\n");
    }
    if(verb)
    {
        printf("[INFO] Liberando memoria...\n");
    }
    facistolDestruirMatriz((void**)m, height);
    free(file);
    free(info);
    fclose(p);
    fclose(fo);
    return EXITO;
}

void facistolHelp()
{
        printf("BMPMANIPULEITOR - Manipulador de imágenes BMP 24 bits\n");
        printf("GRUPO: FACISTOL\n");
        printf("Integrantes:\n");
        printf("1. 47170837 - NACCA, Santino\n");
        printf("2. 45072269 - ZAMBELLA, Lautaro\n");
        printf("Uso: bmpmanipuleitor.exe [OPCIONES]\n");
        printf("EJEMPLOS:\n");
        printf("bmpmanipuleitor.exe --negativo foto.bmp\n");
        printf("bmpmanipuleitor.exe --info imagen.bmp --validar\n");
        printf("bmpmanipuleitor.exe foto.bmp --verbose --escala-de-grises --aumentar-contraste=25\n");
        printf("FILTROS:\n");
        printf("--info <Se informan las cualidades de la imagen>\n");
        printf("-validar- <Se valida la imagen>\n");
        printf("--negativo <A la imagen se le aplica un filtro negativo>\n");
        printf("--comodin <A la imagen se le aplica un filtro sepia>\n");
        printf("--aumentar-contraste=X <A la imagen se le aumenta el contraste un X porciento>\n");
        printf("--disminuir-contraste=X <A la imagen se le disminuye el contraste un X porciento>\n");
        printf("--tonalidad-roja=X <A la imagen se le aplica un filtro de tonalidad roja un X porciento>\n");
        printf("--tonalidad-azul=X <A la imagen se le aplica un filtro de tonalidad azul un X porciento>\n");
        printf("--tonalidad-verde=X <A la imagen se le aplica un filtro de tonalidad verde un X porciento>\n");
        printf("--escala-de-grises <A la imagen se le aplica un filtro gris>\n");
        printf("--rotar-derecha <A la imagen se la rota a la derecha 90 grados>\n");
        printf("--rotar-izquierda <A la imagen se la rota a la izquierda 90 grados>\n");
        printf("--espejar-horizontal <A la imagen se la espeja horizontalmente>\n");
        printf("--espejar-vertical <A la imagen se la espeja verticalmente>\n");
        printf("--concatenar-vertical <A la imagen se la concatena con otra imagen verticalmente>\n");
        printf("--concatenar-horizontal <A la imagen se la concatena con otra imagen horizontalmente>\n");
        printf("--recortar=X <A la imagen se la recorta un X porciento>\n");
        printf("--achicar=X <A la imagen se la achica un X porciento>\n");
}

int facistolConcatenarVertical(char* arg, char* arg2,bool verb,bool* segundoverb){
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
    char newname[255] = "facistol_concatenacion_vertical_";
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

    newInfo->height = info->height + info2->height;
    if(abs(info->width) >= abs(info2->width)){
        newInfo->width = info->width;
    }else{
        newInfo->width = info2->width;
    }

    int newSize = abs(newInfo->height) * newInfo->width;
    int newIs_top_down = (newInfo->height < 0);

    int32_t newHeight = abs(newInfo->height);
    int32_t newWidth = newInfo->width;
    int newPadding = (4 - (newWidth* 3) % 4) % 4;

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
        printf("[INFO] Aplicando filtro: concatenar vertical\n");
    }
    for(int w = 0; w < info->width; w++){
        for(int h = 0; h < abs(info->height); h++){
            newMat[h][w] = m[h][w];
        }
    }

    if(abs(info->width) >= abs(info2->width)){
        for(int h2 = 0; h2 < abs(info2->height); h2++)
        {
            for(int w2 = 0; w2 < info2->width; w2++)
            {
                newMat[info->height + h2][w2] = m2[h2][w2];
            }
        }
        for(int h3 = abs(info->height); h3 < newHeight; h3++)
        {
            for(int w3 = info2->width; w3 < newWidth; w3++)
            {
                newMat[h3][w3].blue = 255;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
    }else{
        for(int h3 = 0; h3 < abs(info->height); h3++)
        {
            for(int w3 = info->width; w3 < newWidth; w3++)
            {
                newMat[h3][w3].blue = 255;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
        for(int h2 = 0; h2 < abs(info2->height); h2++)
        {
            for(int w2 = 0; w2 < newWidth; w2++)
            {
                newMat[info->height + h2][w2] = m2[h2][w2];
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
        printf("[INFO] Filtro concatenar vertical completado exitosamente\n");
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

int facistolRotacion90Derecha(char* arg,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_90Grados_Derecha_";
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
        printf("[INFO] Aplicando filtro: rotacion 90 grados a la derecha\n");
    }
    for(int x=0;x<height;x++)
    {
        for(int y=0;y<info->width;y++)
        {

            newMat[y][info->height-1-x]=m[x][y];
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
        printf("[INFO] Filtro rotacion 90 grados a la derecha completado exitosamente\n");
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

int facistolEspejarVertical(char* arg,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_espejado_vertical_";
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
        printf("[INFO] Aplicando filtro: espejar vertical\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel* vec = malloc(info->width*sizeof(Pixel));
    if(vec == NULL){
        free(vec);
        printf("error al crear vec\n");
        return ERROR_MEM;
        exit(1);
    }

    for(int h=0;h<(height)/2;h++)
    {
        for(int k=0;k<info->width;k++){
            vec[k] = m[height-1-h][k];
            m[height-1-h][k] = m[h][k];
            m[h][k] = vec[k];
        }
    }
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, height, info->width, m, fo, padding);
    if(verb)
    {
        printf("[INFO] Filtro espejar vertical completado exitosamente\n");
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

int facistolRecortar(char* arg, int porcentaje,bool verb,bool* segundoverb){
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
    char newName[255] = "facistol_recortado_";
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
    int32_t newHeight = (abs(info->height)*porcentaje)/100 ;
    int32_t newWidth = (info->width*porcentaje)/100;
    int newPadding = (4 - (newWidth* 3) % 4) % 4;

    info->image_size = newHeight*newWidth;
    file->file_size = info->image_size + info->header_size;
    info->height = newHeight;
    info->width = newWidth;
    if(verb==true && propioverb==true)
    {
        printf("[INFO] Datos cargados correctamente\n");
    }
    if(verb)
    {
        printf("[INFO] Aplicando filtro: recortar\n");
    }
    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    if(verb)
    {
        printf("[INFO] Guardando resultado: %s\n",newName);
    }
    readOrWriteMatrix(ESCRIBIR, is_top_down, newHeight, newWidth, m, fo, newPadding);
    if(verb)
    {
        printf("[INFO] Filtro recortar completado exitosamente\n");
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

