void getWordInLine(char *fileName, int lineNumber, char *p)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fileName, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i = 1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i == lineNumber)
        {
            strcpy(p, line);
            fclose(fp);
            return;
        }
        i++;
    }
}

int validar_palabra(char palabra_juego[], char palabra_user[], int puntaje[], char letras_total[], char letras_parcial[], int *win)
{
    int puntaje_vuelta = 0; //va acumulando el puntaje del intento actual

    if (strncmp(palabra_juego, palabra_user, CANT_LETRAS) == 0) //valido primero si los strings son iguales
    {
        *win = 1;
        for (int i = 0; i < CANT_LETRAS; i++)
        {
            puntaje[i] = 2; //cargo en la matriz de puntajes el valor del acierto para todas las letras
        }
        return puntaje_vuelta; //devuelvo solo el puntaje de vuelta ya que la jugada solo suma el premio por acierto
    }
    else //si la palabra que ingreso el usuario no es igual a la que hay que adivinar
    {
        for (int m = 0; m < CANT_LETRAS; m++) //recorro el string para calcular los aciertos
        {
            char p = toupper(palabra_user[m]); //convierto a uppercase el char
            
            if ((strchr(palabra_juego, p) != NULL)) // si el char esta en la palabra
            {
                if (palabra_juego[m] == p) // si ademas esta en la posicion correcta
                {
                    if (strchr(letras_total, p) == NULL) // si no se contabilizaron los puntos previamente
                    {
                        puntaje_vuelta += ACIERTO_TOTAL; //sumo al puntaje del intento el correspondiente al acierto total
                        strncat(letras_total, &p, 1); //concateno ese char a un string para no volver a contabilizarlo en otro intento
                    }

                    puntaje[m] = 2; //actualizo el puntaje correspondiente a ese acierto
                }
                else //si la letra esta pero en otra posicion
                {
                    if (strchr(letras_parcial, p) == NULL) // si no se contabilizaron los puntos previamente
                    {
                        puntaje_vuelta += ACIERTO_PARCIAL; //sumo al puntaje del intento el correspondiente al acierto parcial
                        strncat(letras_parcial, &p, 1); //concateno ese char a un string para no volver a contabilizarlo en otro intento
                    }

                    puntaje[m] = 1; //actualizo el puntaje correspondiente a ese acierto
                }
            }
            else //el char no esta en la palabra
            {
                puntaje[m] = 0;
            }
        }
    }

    return puntaje_vuelta;
}

void print_result(int puntaje, char letra)
{
    if (puntaje == 2)
    {
        printGreen();
    }
    else if (puntaje == 1)
    {
        printYellow();
    }
    else
    {
        printWhite();
    }
    printf(" %c ", letra);
    normalPrint();
}

void resumen(int resultados[], int cant_partidas)
{
    int puntaje_max = 0;
    int puntaje_min = 10000;
    int partida_max;
    int partida_min;
    int partidas_ganadas = 0;

    for (int i = 0; i <= cant_partidas; i++) //recorro el array con los resultados por partida
    {
        if (resultados[i] > 0) //valido solo las partidas que haya ganado, las que no gano valen 0
        {
            partidas_ganadas++;
            if (resultados[i] > puntaje_max) //si el puntaje de esa partida es mayor que el puntaje maximo actualizo el valor
            {
                puntaje_max = resultados[i];
                partida_max = i;
            }
            if (resultados[i] < puntaje_min) //si el puntaje de esa partida es menor que el puntaje minimo actualizo el valor
            {
                puntaje_min = resultados[i];
                partida_min = i;
            }
        }
    }

    if (partidas_ganadas > 0) //si gano alguna partida
    {
        if (partidas_ganadas == 1) //si gano solo una, solo muestro esa
        {
            printf("* * * * Ganaste la partida numero %i con %i puntos * * * *\n", partida_max + 1, puntaje_max);
        }
        else //si gano mas de una muestro cual fue la mejor y cual la peor
        {
            printf("* * * * Tu puntaje maximo fue de %i en la partida %i * * * *\n", puntaje_max, partida_max + 1);
            printf("* * * * Tu puntaje minimo fue de %i en la partida %i * * * *\n", puntaje_min, partida_min + 1);
        }
        printf("* * * * Promedio de victorias: %i de %i * * * *\n", partidas_ganadas, cant_partidas + 1);
    }
    else //si no gano ninguna partida
    {
        printf("* * * * No ganaste ninguna de las %i partidas * * * *\n", cant_partidas + 1);
    }
}
