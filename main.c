#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "variables.h"
#include "colors.h"
#include "functions.h"

int main()
{
    int resultados[PARTIDAS_MAX] = {0};
    int cant_partidas = 1;
    int puntaje_total = 5000;
    char continue_game;
    int win = 0;

    do
    {
        printf("Ingrese cantidad de partidas a jugar. (MAX 8) \n");
        scanf("%d", &cant_partidas); //pedir al usuario la cantidad de partidas a jugar
    } while (cant_partidas > PARTIDAS_MAX || cant_partidas <= 0); // se le vuelve a pedir en caso que el numero no sea valido


    for (int i = 0; i < cant_partidas; i++) //repetir este bucle por cada partida
    {
        char palabra[7]; //almacena la palabra que devuelve getWordinLine
        srand(time(NULL)); //semilla para el rand
        getWordInLine("words.txt", rand() % 30 + 1, palabra);
        int puntaje[CANT_INTENTOS][CANT_LETRAS + 1]; // almacena el puntaje correspondiente a cada acierto. (0, 1 o 2)
        char palabras[CANT_INTENTOS][CANT_LETRAS + 1]; //almacena todas las palabras que ingreso por partida

        char letras_total[6]; //almacena las letras que ya sumaron el acierto total
        letras_total[0] = '\0';
        char letras_parcial[6]; //almacena las letras que ya sumaron el acierto parcial
        letras_parcial[0] = '\0';

        puts("----------------");
        printf("Partida %i de %i\n", i + 1, cant_partidas);
        puts("----------------");

        for (int j = 0; j < CANT_INTENTOS; j++) //se repite este bucle para cada intento 
        {
            printf("Ingresa una palabra de 5 letras: ");
            scanf("%5s", palabras[j]); //pide una palabra al usuario y se almacena en la matriz "palabras"

            //calculo el puntaje total correspondiente a ese acierto, 
            //ademas la funcion "validar palabra" actualiza la matriz de puntos por letra
            int puntaje_vuelta = validar_palabra(palabra, palabras[j], puntaje[j], letras_total, letras_parcial, &win);

            for (int p = 0; p <= j; p++) //el bucle recorre las filas de la matriz de puntajes y la de palabras
            {
                for (int k = 0; k < 5; k++) //recorro las columnas de ambas matrices
                {
                    print_result(puntaje[p][k], palabras[p][k]); //valida que color le corresponde a cada letra segun el puntaje
                }
                normalPrint();
                printf("\n");
            }

            if (win == 1) //en caso que "validar_palabra" haya actualizado el valor de win si los strings coinciden
            {
                int premio = (j == 0) ? PREMIO_MAYOR : PREMIO_MENOR; //defino el premio correspondiente
                resultados[i] += premio; //actualizo el puntaje de la partida
                printf("\n");
                printf("*\t*\t*\t*\t*\t*\t*\t*\n");
                printf("*\t*\t*\tGANASTE!!\t*\t*\t*\n");
                printf("*\t*\t*\t*\t*\t*\t*\t*\n");
                printf("\n");
                win = 0; //reseteo el valor de win
                break; //corto la partida
            }

            if (j + 1 == CANT_INTENTOS) //si es el ultimo intento
            {
                resultados[i] = 0; //el valor de la partida es 0, si llego aca es porque no gano
            }
            else //si no es el ultimo intento
            {
                resultados[i] += puntaje_vuelta; //voy actualizando el puntaje de la partida con lo que haya acertado en el intento
            }

            puntaje_total -= DESCUENTO_INTENTO; //descuento el puntaje correspondiente por un nuevo intento
            printf("Puntaje: %i\n", (puntaje_total + resultados[i]));
        }

        puntaje_total += resultados[i]; //actualizo el puntaje acumulado a lo correspondiente a la partida
        printf("La palabra a adivinar era %s\n", palabra);
        resumen(resultados, cant_partidas); //muestro el resumen
        printf("Puntaje final: %i\n", puntaje_total);

        if ((i + 1) < cant_partidas) //valido que no sea la ultima partida
        {
            do //ofrecer al usuario la opcion de terminar el juego
            {
                printf("continuar jugando? (S)i - (N)o\n");

                scanf(" %c", &continue_game);

                if (!(continue_game == 'n' || continue_game == 's' || continue_game == 'N' || continue_game == 'S'))
                {
                    printf("Ingresaste un caracter invalido\n");
                }

            } while (!(continue_game == 'n' || continue_game == 's' || continue_game == 'N' || continue_game == 'S'));
        }
        if (continue_game == 'n' || continue_game == 'N') //si elije no continuar, corta la jugada
        {
            break;
        }
    }

    return 0;
}