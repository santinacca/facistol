#include "funciones_grupo.h"
#define EXITO 0
#define ERROR_ARG 1
#define ERROR_ARCH 2
#define ERROR_MEM 3
#define BMP_INVALIDO 4
#define MAXLETRAS 255

int procesarImagen(int num, char* arg[])
{
    bool verbos=false;
    bool segunda_llamda = false;
    bool *segundoverbos= &segunda_llamda;
    int filtrosunicos=0,cantImg=0, cantArchivos = 0;

    char* imagen1=NULL;
    char* imagen2=NULL;
    char** argmain=(char**)facistolCrearMatriz(num,MAXLETRAS,sizeof(char*));

    for(int i=0;i<num;i++)
    {
        argmain[i]=NULL;
    }
    for(int j=0;j<num;j++)
    {
        if(busquedaString(arg[j],argmain,filtrosunicos)==0)
        {
            argmain[filtrosunicos]=strdup(arg[j]);
            filtrosunicos ++;
        }
    }
    for(int p = 1; p < filtrosunicos; p++){
        if(strstr(argmain[p], ".bmp") != NULL){
                if(imagen1 == NULL){
                    imagen1 = argmain[p];
                    cantImg++;
                }else if(imagen2 == NULL){
                    imagen2 = argmain[p];
                    cantImg++;
                }else{
                    cantImg++;
                }
            }
    }
    for(int i=0;i<num;i++)
    {
        if(strcmpi(arg[i],"--verbose")==0)
        {
            verbos=true;
            i=num;
        }
    }
    if (verbos)
    {
        printf("[INFO] Iniciando bmpmanipuleitor...\n");
        printf("[INFO] Argumentos detectados:\n");
        for(int i=1;i<num;i++)
        {
            printf("%s\n",arg[i]);
        }
    }

    for(int n = 1; n < filtrosunicos;n++){
        if(strstr(argmain[n], "--info") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            facistolInfo(imagen1);
        }
        if(strstr(argmain[n], "--validar") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolValidar(imagen1);
            if(devolucion != 0){
                printf("La imagen enviada a la funcion %s no es un archivo BMP valido\n", argmain[n]);
            }
        }
        if(strstr(argmain[n], "--aumentar-contraste") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolAumentarContraste(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--reducir-contraste") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolDisminuirContraste(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--tonalidad-roja") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolTonalidadRoja(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--tonalidad-verde") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolTonalidadVerde(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--tonalidad-azul") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolTonalidadAzul(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
        }cantArchivos++;
        }
        if(strstr(argmain[n], "--escala-de-grises") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolGris(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--rotar-izquierda") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolRotacion90Izquierda(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--rotar-derecha") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolRotacion90Derecha(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--espejar-horizontal") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolEspejarHorizontal(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--espejar-vertical") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolEspejarVertical(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--concatenar-horizontal") != NULL)
        {
            if(cantImg !=2){
                printf("Cantidad de imagenes incorrecta, se esperan dos\n");
            }
            int devolucion;
            devolucion = facistolConcatenarHorizontal(imagen1,imagen2,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--concatenar-vertical") != NULL)
        {
            if(cantImg !=2){
                printf("Cantidad de imagenes incorrecta, se esperan dos\n");
            }
            int devolucion;
            devolucion = facistolConcatenarVertical(imagen1,imagen2,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--recortar") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolRecortar(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--achicar") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int porcentaje=cortarString(argmain[n]);
            if(porcentaje!=-1)
            {
                int devolucion;
                devolucion = facistolAchicar(imagen1,porcentaje,verbos,segundoverbos);
                if(devolucion == 2){
                    printf("Error en el archivo pasado a %s\n", argmain[n]);
                }else if(devolucion == 3){
                    printf("Error de memoria en la funcion %s\n", argmain[n]);
                }else if(devolucion == 4){
                    printf("Error con el archivo pasado a %s\n", argmain[n]);
                }
            }else{
                printf("Error en el argumento pasado a %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--help") != NULL)
        {
            facistolHelp();
        }
        if(strstr(argmain[n], "--negativo") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolNegativo(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
        if(strstr(argmain[n], "--comodin") != NULL)
        {
            if(cantImg !=1){
                printf("Cantidad de imagenes incorrecta, se espera solo una\n");
            }
            int devolucion;
            devolucion = facistolComodin(imagen1,verbos,segundoverbos);
            if(devolucion == 4){
                printf("Error con el archivo pasado a %s\n", argmain[n]);
            }else if(devolucion == 2){
                printf("Error al crear archivos en la funcion %s\n", argmain[n]);
            }else if(devolucion == 3){
                printf("Error de memoria en la funcion %s\n", argmain[n]);
            }
            cantArchivos++;
        }
    }
    if(verbos)
    {
        printf("[INFO] Proceso finalizado - %d archivos generados\n",cantArchivos);
    }

    facistolDestruirMatriz((void**)argmain,num);
    return EXITO;
}

int busquedaString(char* arg, char** vector, int size){

    for(int i = 0; i < size; i++){
        if(strcasecmp(arg, vector[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int cortarString(char* arg){

    char* p = strstr(arg, "=");
    if(p != NULL){
        size_t length = strlen(arg);
        if(strcmpi(p+1, "\0") == 0){
            printf("Falta el valor del porcentaje\n");
            return -1;
        }else{
            char* sliced = (char*)malloc((length + 1)*sizeof(char));
            strncpy(sliced, p+1, length);
            sliced[length] = '\0';
            int porcentaje = atoi(sliced);
            if(porcentaje > 100 || porcentaje < 0){
                printf("Error en el porcentaje, valor no valido\n");
            }else{
                return porcentaje;
            }
            free(sliced);
        }

    }else{
        printf("Mal formato de argumento, falta el =\n");
        int porcentaje=-1;
        return porcentaje;
    }
    return -1;
}

