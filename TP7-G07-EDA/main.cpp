#include "libCurlExternal.h"
#include "Tweet.h"
#include "basicLCD.h"
#include "damianLCD.h"
#include "LCD_Lucas.h"
#include "gui.h"

#include <stdint.h>

using namespace std;

int main(int, char**)
{

    Gui gui(720, 1280);
    getTweet tweet;
    damianLCD LCDDamian;
    LCD_Lucas LCDLucas;
    basicLCD* lcdArray[DISPLAYS] = { &LCDDamian, &LCDLucas, NULL };
    std::vector<std::string> tweets;
    int error = 0;

    for (int i = 0; i < DISPLAYS; i++) {

        gui.setLCDArray(i, lcdArray[i]);
    }

    gui.setup();

    DisplayState state = USERNAMEINPUT; //We begin reading the username and number of tweets that will be read

    // Main loop

    while (state != STOP_RUNNING)
    {
        if (state == USERNAMEINPUT) {

            state = gui.functions(state, tweets);   //Mientras los tweets no esten listos, paso un dummy
        }
        else if (state == DOWNLOADINGTWEETS) {

            error = tweet.getTweets(gui.username, gui, gui.numberOfTweets);

            if (error == 1) {

                state = gui.functions(state, tweets);
            }
            else {

                state = TRANSFORMINGTWEETS;
            }
        }
        else if (state == TRANSFORMINGTWEETS) {

            for (string const& c : tweet.getinfo()) {   //Transformo la lista de tweets en un vector de tweets
                                                        //Para un manejo mucho mas eficiente
                tweets.push_back(c);
            }

            state = SHOWINGTWEETS;
        }
        else if (state == SHOWINGTWEETS) {

            state = gui.functions(state, tweets);
        }
    }

    return 0;
}