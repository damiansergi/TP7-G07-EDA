/*

    MAIN()

Inicializacion de la GUI
        |
        |
Ver que es lo que se quiere hacer
        |
        |
Crear una lista de tweets --> para esto hay que parsear todos los datos en libCurlExternal.cpp
        |
        |
Ir mostrandolos y checkeando que es lo que quiere hacerse en la GUI
*/

#include "libCurlExternal.h"
#include "Tweet.h"
#include "basicLCD.h"
#include "damianLCD.h"
#include "gui.h"

#include <stdint.h>

using namespace std;

int main(int, char**)
{

    Gui gui(720, 1280);

    damianLCD LCDDamian;
    basicLCD* lcdArray[DISPLAYS] = { &LCDDamian, NULL, NULL };

    for (int i = 0; i < DISPLAYS; i++) {

        gui.setLCDArray(i, lcdArray[i]);
    }

    gui.setup();

    DisplayState state = USERNAMEINPUT; //We begin reading the username and number of tweets that will be read


    // Main loop

    while (state != STOP_RUNNING)
    {
        if (state == USERNAMEINPUT) {


            state = gui.functions(state);
        }
        else if (state == DOWNLOADINGTWEETS) {


            state = gui.functions(state);
        }
        else if (state == SHOWINGTWEETS) {


            state = gui.functions(state);
        }
    }

    return 0;
}