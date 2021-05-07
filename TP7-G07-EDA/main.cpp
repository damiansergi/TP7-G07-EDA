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
    damianLCD LCDDamian;

    basicLCD* lcdArray[DISPLAYS] = { &LCDDamian, NULL, NULL };

    Gui gui(720, 1280, lcdArray);

    gui.setup();

    // Main loop
    bool running = true;

    while (running)
    {

        gui.functions();

    }

    return 0;
}