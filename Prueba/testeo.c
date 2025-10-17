#include "testeo.h"

void procesarImagen(int num, char* arg[]){
    FILE* p = fopen(arg[1], "rb");
    if(p == NULL){
        printf("error al abrir el archivo\n");
        exit(1);
    }
    // FILE* p2 = fopen(arg[2], "rb");
    // if(p2 == NULL){
    //     printf("Error al abrir el segundo archivo\n");
    //     exit(1);
    // }

    BMPFileHeader* file = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader* info = malloc(sizeof(BMPInfoHeader));
    //
    // BMPFileHeader* file2 = malloc(sizeof(BMPFileHeader));
    // BMPInfoHeader* info2 = malloc(sizeof(BMPInfoHeader));

    facistolLeerHeader(p, file, info);
    //facistolLeerHeader(p2, file2, info2);

    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    // int size2 = abs(info2->height) * (info2->width);
    // int is_top_down2 = (info2->height < 0);
    // int padding2 = (4-(info2->width * 3) %4) %4;
    // int32_t height2 = abs(info2->height);

    Pixel** mat = (Pixel**)facistolCrearMatriz(height, info->width, sizeof(Pixel));
    // Pixel** mat2 = (Pixel**)facistolCrearMatriz(height2, info2->width, sizeof(Pixel));

    fseek(p, file->offset_data, SEEK_SET);
    //fseek(p2, file2->offset_data, SEEK_SET);

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

    // if(is_top_down2){
    //     for(int j = 0; j < height2; j++){
    //         fread(mat2[j], sizeof(Pixel), info2->width, p2);
    //         fseek(p2, padding2, SEEK_CUR);
    //     }
    // }else{
    //     for(int j = height2-1; j >= 0; j--){
    //         fread(mat2[j], sizeof(Pixel), info2->width, p2);
    //         fseek(p2, padding2, SEEK_CUR);
    //     }
    // }
    
    facistolValidar(file, info, arg[1]);
    facistolInfo(file, info);

    // facistolValidar(file2, info2, arg[2]);
    // facistolInfo(file2, info2);
    //facistolCopiar(file, info, arg[1], p, mat);
    //facistolNegativo(mat, file, info, arg[1]);
    //facistolGris(mat, file, info, arg[1]);
    //facistolRotacion90Izquierda(mat, file, info, arg[1]);
    //facistolRotacion90Derecha(mat, file, info, arg[1]);
    //facistolEspejarHorizontal(mat, file, info, arg[1]);
    //facistolEspejarVertical(mat, file, info, arg[1]);
    printf("%d\n", num);
    if(num > 2){
        //facistolTonalidadRoja(mat, file, info, arg[1], atoi(arg[2]));
        //facistolTonalidadVerde(mat, file, info, arg[1], atoi(arg[2]));
        //facistolTonalidadAzul(mat, file, info, arg[1], atoi(arg[2]));
        //facistolAumentarContraste(mat, file, info, arg[1], atoi(arg[2]));
        //facistolDisminuirContraste(mat, file, info, arg[1], atoi(arg[2]));
        //facistolRecortar(mat, file, info, arg[1], atoi(arg[2]));
        //facistolConcatenarHorizontal(mat, file, info, arg[1], arg[2], file2, info2, mat2);
        //facistolConcatenarVertical(mat, file, info, arg[1], arg[2], file2, info2, mat2);
        facistolAchicar(mat, file, info, arg[1], atoi(arg[2]));
    }else{
        //facistolNegativo(mat, file, info, arg[1]);
        facistolCopiar(file, info, arg[1], p, mat);
    }
    printf("Hemos rotado\n");
    
    fclose(p);
    //fclose(p2);
    free(file);
    free(info);
    // free(file2);
    // free(info2);
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
int facistolCopiar(BMPFileHeader* file, BMPInfoHeader* info, char* arg, FILE* p, Pixel** mat){
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

//jofrder
void facistolNegativo(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_negativo_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].blue=255-m[h][k].blue;
            m[h][k].red=255-m[h][k].red;   //FILTRO NEGATIVO
            m[h][k].green=255-m[h][k].green;
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}

void facistolGris(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_gris_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

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

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}

void facistolRotacion90Izquierda(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_90Izquierda_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int32_t height = abs(info->height);
    int padding = (4 - (height * 3) % 4) % 4;
    BMPFileHeader newFile = *file;
    BMPInfoHeader newInfo = *info;

    newInfo.width = info->height;
    newInfo.height = info->width;


    printf("antes de escribir el header\n");
    fwrite(&newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(&newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel** newMat = (Pixel**)facistolCrearMatriz(info->width, height, sizeof(Pixel));
    
    for(int x=0;x<height;x++) 
    { 
        for(int y=0;y<info->width;y++) 
        { 
            
            newMat[info->width-1-y][x]=m[x][y];
        }

    }
    printf("Despues de cambiar la matriz\n");

    if(is_top_down){
        for(int k = 0; k < info->width; k++){
            fwrite(newMat[k], sizeof(Pixel), height, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = info->width -1; k >= 0; k--){
            fwrite(newMat[k], sizeof(Pixel), height, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }
    printf("Despues de escribir todo\n");

    facistolDestruirMatriz((void**)m, height);
    facistolDestruirMatriz((void**)newMat, info->width);
    fclose(fo);
}

void facistolRotacion90Derecha(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_90Derecha_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int32_t height = abs(info->height);
    int padding = (4 - (height * 3) % 4) % 4;
    BMPFileHeader newFile = *file;
    BMPInfoHeader newInfo = *info;

    newInfo.width = info->height;
    newInfo.height = info->width;


    printf("antes de escribir el header\n");
    fwrite(&newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(&newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel** newMat = (Pixel**)facistolCrearMatriz(info->width, height, sizeof(Pixel));
    
    for(int x=0;x<height;x++) 
    { 
        for(int y=0;y<info->width;y++) 
        { 
            
            newMat[y][height-1-x]=m[x][y];
        }

    }
    printf("Despues de cambiar la matriz\n");

    if(is_top_down){
        for(int k = 0; k < info->width; k++){
            fwrite(newMat[k], sizeof(Pixel), height, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = info->width -1; k >= 0; k--){
            fwrite(newMat[k], sizeof(Pixel), height, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }
    printf("Despues de escribir todo\n");

    facistolDestruirMatriz((void**)m, height);
    facistolDestruirMatriz((void**)newMat, info->width);
    fclose(fo);
}
//

void facistolEspejarHorizontal(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_espejado_horizontal_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel* vec = malloc(height*sizeof(Pixel));
    if(vec == NULL){
        free(vec);
        printf("error al crear vec\n");
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

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    free(vec);
    fclose(fo);
}

void facistolEspejarVertical(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg){
    char newName[255] = "facistol_espejado_vertical_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    Pixel* vec = malloc(info->width*sizeof(Pixel));
    if(vec == NULL){
        free(vec);
        printf("error al crear vec\n");
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

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    free(vec);
    fclose(fo);
}

void facistolTonalidadRoja(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "facistol_tonalidad_roja_alta_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red=facistolValorAumento(m[h][k].red, porcentaje);   //TONALIDAD ROJA
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}

int facistolValorAumento(int pix, int porcentaje){
    float aumento = porcentaje/100.0;
    int nuevoValor = (int)(pix + (pix*aumento));
    if(nuevoValor > 255){
        return 255;
    }else{
        return nuevoValor;
    }
}

void facistolTonalidadVerde(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "facistol_tonalidad_verde_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].green=facistolValorAumento(m[h][k].green, porcentaje);   //TONALIDAD VERDE
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}


void facistolTonalidadAzul(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "facistol_tonalidad_azul_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].blue=facistolValorAumento(m[h][k].blue, porcentaje);   //TONALIDAD AZUL
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}
//

void facistolAumentarContraste(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "facistol_aumentar_contraste_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red = facistolContraste(m[h][k].red, porcentaje);
            m[h][k].blue = facistolContraste(m[h][k].blue, porcentaje);  //CONTRASTE AUMENTO
            m[h][k].green = facistolContraste(m[h][k].green, porcentaje);   
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
}

int facistolContraste(int pix, int por)
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
//
void facistolRecortar(Pixel** mat, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "cortado_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    
    double sqArea = sqrt((size * porcentaje)/ 100.0);
    int newDimension = (int)sqArea;
    int32_t height = newDimension;
    int32_t width = newDimension;
    int padding = (4 - (width * 3) % 4) % 4;

    info->image_size = (uint32_t)sqArea;
    file->file_size = info->image_size + info->header_size;
    info->height = height;
    info->width = width;


    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);
    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(mat[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(mat[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)mat, height);
    fclose(fo);
}

void facistolDisminuirContraste(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    char newName[255] = "facistol_disminuir_contraste_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);
    int padding = (4 - (info->width * 3) % 4) % 4;
    int32_t height = abs(info->height);

    fwrite(file, sizeof(BMPFileHeader), 1, fo);
    fwrite(info, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, file->offset_data, SEEK_SET);

    for(int h=0;h<height;h++)
    {
        for(int k=0;k<info->width;k++){
            m[h][k].red = contrasteNegativo(m[h][k].red, porcentaje);
            m[h][k].blue = contrasteNegativo(m[h][k].blue, porcentaje);  //CONTRASTE AUMENTO
            m[h][k].green = contrasteNegativo(m[h][k].green, porcentaje);   
        }
    }

    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(m[k], sizeof(Pixel), info->width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }

    facistolDestruirMatriz((void**)m, height);
    fclose(fo);
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
//
void facistolConcatenarHorizontal(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, char* arg2, BMPFileHeader* file2, BMPInfoHeader* info2, Pixel** mat2){
    char newname[255] = "facistol_concatenacion_";
    strcat(newname, arg);
    strcat(newname, "_");
    strcat(newname, arg2);
    printf("Nuevo nombre: %s\n", newname);

    FILE* fo = fopen(newname, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }

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
    printf(PRId32 "altura: %" "\n", newInfo->height );

    int newSize = abs(newInfo->height) * newInfo->width;
    int is_top_down = (newInfo->height < 0);

    int32_t height = abs(newInfo->height);
    int32_t width = newInfo->width;
    int padding = (4 - (width * 3) % 4) % 4;

    newInfo->image_size = (uint32_t)newSize;
    newFile->file_size = newInfo->image_size + newInfo->header_size;

    fwrite(newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, newFile->offset_data, SEEK_SET);

    Pixel** newMat = (Pixel**)facistolCrearMatriz(height, width, sizeof(Pixel));

    for(int w = 0; w < info->width; w++){
        for(int h = 0; h < abs(info->height); h++){
            newMat[h][w] = m[h][w];
        }
    }

    if(abs(info->height) >= abs(info2->height)){
        for(int h2 = 0; h2 < abs(info2->height); h2++){
            //for(int w2 = info->width; w2 < width; w2++){
            for(int w2 = 0; w2 < info2->width; w2++){
                newMat[h2][info->width + w2] = mat2[h2][w2];
                }
        }
        for(int h3 = abs(info2->height); h3 < height; h3++){
            for(int w3 = info->width; w3 < width; w3++){
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
    }else{
        for(int h3 = abs(info->height); h3 < height; h3++){
            for(int w3 = 0; w3 < info->width; w3++){
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
        for(int h2 = 0; h2 < abs(height); h2++){
            for(int w2 = 0; w2 < info2->width; w2++){
                newMat[h2][info->width + w2] = mat2[h2][w2];
                }
            }
    }
    printf("Terminamos de escribir la matriz\n");

    printf("topdown: %d\n", is_top_down);
    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(newMat[k], sizeof(Pixel), width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(newMat[k], sizeof(Pixel), width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }
    printf("Escribimos en el archivo\n");
    facistolDestruirMatriz((void**)newMat, height);
    facistolDestruirMatriz((void**)m, abs(info->height));
    facistolDestruirMatriz((void**)mat2, abs(info2->height));
    free(newFile);
    free(newInfo);
    fclose(fo);
}

void facistolConcatenarVertical(Pixel** m, BMPFileHeader* file, BMPInfoHeader* info, char* arg, char* arg2, BMPFileHeader* file2, BMPInfoHeader* info2, Pixel** mat2){
    char newname[255] = "facistol_concatenacion_vertical_";
    strcat(newname, arg);
    strcat(newname, "_");
    strcat(newname, arg2);
    printf("Nuevo nombre: %s\n", newname);

    FILE* fo = fopen(newname, "wb");
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }

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
    printf(PRId32 "altura: %" "\n", newInfo->height );

    int newSize = abs(newInfo->height) * newInfo->width;
    int is_top_down = (newInfo->height < 0);

    int32_t height = abs(newInfo->height);
    int32_t width = newInfo->width;
    int padding = (4 - (width * 3) % 4) % 4;

    newInfo->image_size = (uint32_t)newSize;
    newFile->file_size = newInfo->image_size + newInfo->header_size;

    fwrite(newFile, sizeof(BMPFileHeader), 1, fo);
    fwrite(newInfo, sizeof(BMPInfoHeader), 1, fo);
    fseek(fo, newFile->offset_data, SEEK_SET);

    Pixel** newMat = (Pixel**)facistolCrearMatriz(height, width, sizeof(Pixel));

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
                newMat[info->height + h2][w2] = mat2[h2][w2];
            }
        }
        for(int h3 = abs(info->height); h3 < height; h3++)
        {
            for(int w3 = info2->width; w3 < width; w3++)
            {
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
    }else{
        for(int h3 = 0; h3 < abs(info->height); h3++)
        {
            for(int w3 = info->width; w3 < width; w3++)
            {
                newMat[h3][w3].blue = 0;
                newMat[h3][w3].red = 0;
                newMat[h3][w3].green = 0;
            }
        }
        for(int h2 = 0; h2 < abs(info2->height); h2++)
        {
            for(int w2 = 0; w2 < width; w2++)
            {
                newMat[info->height + h2][w2] = mat2[h2][w2];
            }
        }
    }
    printf("Terminamos de escribir la matriz\n");

    printf("topdown: %d\n", is_top_down);
    if(is_top_down){
        for(int k = 0; k < height; k++){
            fwrite(newMat[k], sizeof(Pixel), width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }else{
        for(int k = height -1; k >= 0; k--){
            fwrite(newMat[k], sizeof(Pixel), width, fo);
            for (int i = 0; i < padding; i++) {
                fputc(0x00, fo);
            }
        }
    }
    printf("Escribimos en el archivo\n");
    facistolDestruirMatriz((void**)newMat, height);
    facistolDestruirMatriz((void**)m, abs(info->height));
    facistolDestruirMatriz((void**)mat2, abs(info2->height));
    free(newFile);
    free(newInfo);
    fclose(fo);
}
//

void facistolAchicar(Pixel** mat, BMPFileHeader* file, BMPInfoHeader* info, char* arg, int porcentaje){
    if(porcentaje < 1 || porcentaje > 100){
        printf("Porcentaje no valido\n");
        porcentaje = 100;
    }
    int factor = (int)(porcentaje/100);

    char newName[255] = "achicado_";
    strcat(newName, arg);
    FILE* fo = fopen(newName, "wb"); // IMPORTANT
    if (fo == NULL){
        printf("error al abrir el nuevo archivo\n");
        exit(1);
    }
    int32_t origenH = abs(info->height);
    int32_t origenW = info->width;

    int size = abs(info->height) * (info->width);
    int is_top_down = (info->height < 0);

    int32_t height = origenH / porcentaje;
    int32_t width = origenW / porcentaje;


    Pixel** newMat = (Pixel**)facistolCrearMatriz(height, width, sizeof(Pixel));


    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            newMat[h][w] = calculoPromedio(mat, porcentaje, h, w, origenH, origenW);
        }
    }

    int padding = (4- (width * 3) %4) %4;
    uint32_t imageSizeNew = (uint32_t)(height * (width * 3 + padding));
    uint32_t fileSizeNew = file->offset_data + imageSizeNew;

    file->file_size = fileSizeNew;
    info->image_size = imageSizeNew;

    if(is_top_down){
        info->height = 0 - height;
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

    facistolDestruirMatriz((void**)mat, height);
    fclose(fo);
}

Pixel calculoPromedio(Pixel** mat, int porcentaje, int newH, int newW, int oldH, int oldW){

    int origenH = newH * porcentaje;
    int orgienW = newW * porcentaje;
    int endH;
    int endW;
    
    if(origenH + porcentaje < oldH ){
        endH = origenH + porcentaje;
    }else{
        endH = oldH;
    }

    if(orgienW + porcentaje < oldW){
        endW = orgienW + porcentaje;
    }else{
        endW = oldW;
    }

    int acuRed = 0;
    int acuGreen = 0;
    int acuBlue = 0;
    int cantPixeles = 0;
    for(int i = origenH; i < endH; i++){
        for(int j = orgienW; j <= endW; j++){
            acuRed+= mat[i][j].red;
            acuGreen+= mat[i][j].green;
            acuBlue+= mat[i][j].blue;
            cantPixeles++;
        }
    }

    Pixel devolver;
    acuRed = (int)acuRed/cantPixeles;
    acuGreen = (int)acuGreen/cantPixeles;
    acuBlue = (int)acuBlue/cantPixeles;

    if(cantPixeles > 0){
        devolver.red = acuRed;
        devolver.blue = acuBlue;
        devolver.green = acuGreen;
    }else{
        devolver.red = 0;
        devolver.blue = 0;
        devolver.green = 0;
    }
    return devolver;
}
