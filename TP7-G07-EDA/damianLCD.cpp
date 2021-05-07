
#include "damianLCD.h"
#include <iostream> 

using namespace std;

#define DISPLAYWIDTH 720
#define DISPLAYHEIGHT 120

#define MOVE_Y -250
#define MOVE_X 225

#define ALLEGRODISPLAYWIDTH 1280
#define ALLEGRODISPLAYHEIGHT 720

static int CursorPosToNumber(cursorPosition pos);  //Translate from the cursor position to the index in the string

damianLCD::damianLCD() {
    /*
    error = false;

    if (!al_init()){

        cout << "failed to initialize allegro!\n" << endl;
        error = true;
    }

    if (!al_init_primitives_addon()) {

        cout << "failed to initialize allegro primitives!\n" << endl;
        error = true;
    }
    if (!al_init_font_addon()) {

        cout << "failed to initialize allegro fonts!\n" << endl;
        error = true;
    }
    if (!al_init_ttf_addon()) {

        cout << "failed to initialize allegro ttf!\n" << endl;
        error = true;
    }
    */

    if (!(font = al_load_ttf_font("LiberationSerif-Regular.ttf", 50, 0))) {

        cout << "failed to initialize the font!\n" << endl;
        error = true;
    }

    /*
    display = al_create_display(ALLEGRODISPLAYWIDTH, ALLEGRODISPLAYHEIGHT);
    */

    pos.column = 0;
    pos.row = 0;
    message = string("                                ");
}

damianLCD::~damianLCD() {

    al_destroy_display(display);
}

bool damianLCD::lcdInitOk() {

    if (error == true){
    
        return false;
    }

    if (display == nullptr) {

        return false;
    }

    return true;

}


lcdError& damianLCD::lcdGetError() {

    return damianLCDError;
}


bool damianLCD::lcdClear() {

    message.clear();
    message = string("                                ");
    pos.row = 0;
    pos.column = 0;

    printMessage();

    return true;
}

bool damianLCD::lcdClearToEOL(){  

    if (pos.row == 0) {

        message.replace(pos.column, 16 - pos.column, 16 - pos.column, ' ');
    }
    else if (pos.row == 1) {

        message.replace(pos.column + 16, 16 - pos.column, 16 - pos.column, ' ');
    }

    printMessage();

    return true;
}

basicLCD& damianLCD::operator<<(const unsigned char c) { 

    if (isprint(c)) {
        message.replace(CursorPosToNumber(pos), 1, 1, c);

        printMessage();
        lcdMoveCursorRight();
    }
    return *this;
}

basicLCD& damianLCD::operator<<(const char* c) {

    string auxString = string(c);
    int index = CursorPosToNumber(pos);
    int copyIndex = 0;

     while (auxString.size() > 32) {

         auxString.erase(0, 1);
     }

     while (index < 32 && copyIndex < (int) auxString.size() ) {

         message.replace(index, 1, 1, auxString[copyIndex] );
         index++;
         copyIndex++;

         lcdMoveCursorRight();

         if (pos.column == 0 && pos.row == 0) {

             index = CursorPosToNumber(pos);
         }
     }

    printMessage();

    return *this;
}

bool damianLCD::lcdMoveCursorLeft() {

    if (pos.column == 0 && pos.row == 1) {

        pos.column = 15;
        pos.row = 0;
    }
    else if (pos.column > 0) {
        pos.column--;
    }

    return true;
}

bool damianLCD::lcdMoveCursorRight() {

    if (pos.column == 15 && pos.row == 0) {

        pos.column = 0;
        pos.row = 1;
    }
    else if (pos.column < 15) {
        pos.column++;
    }
    else if (pos.column == 15 && pos.row == 1) {
        pos.column = 0;
        pos.row = 0;
    }
    else {
        return false;
    }

    return true;
}

bool damianLCD::lcdMoveCursorDown() {

    if (pos.row == 0) {
        pos.row++;
    }
    else {
        return false;
    }

    return true;
}

bool damianLCD::lcdMoveCursorUp() {

    if (pos.row == 1) {
        pos.row--;
    }
    else {
        return false;
    }

    return true;
}

bool damianLCD::lcdSetCursorPosition(const cursorPosition pos) {

    if (pos.column <= 15 && pos.row <= 1 && pos.column >= 0 && pos.row >= 0) {
            
        this->pos = pos;
    }
    else {

        return false;
    }

    return true;
}

cursorPosition damianLCD::lcdGetCursorPosition() {

    return pos;
}

bool damianLCD::printMessage() {

    char buffer[2] = {1, '\0'};

    //al_clear_to_color(al_map_rgb(255, 255, 255));

    al_draw_filled_rectangle(((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) - 50 + MOVE_X,
        ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) - 50 + MOVE_Y,
        (((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + DISPLAYWIDTH) + 50 + MOVE_X,
        (((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT) + 50 + MOVE_Y,
        al_map_rgb(146, 42, 141));

    al_draw_filled_rectangle((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2 + MOVE_X,
        (ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2 + MOVE_Y,
        ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + DISPLAYWIDTH + MOVE_X,
        ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT + MOVE_Y,
        al_map_rgb(50, 150, 200));

    al_draw_rectangle((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2 + MOVE_X,
        (ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2 + MOVE_Y,
        ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + DISPLAYWIDTH + MOVE_X,
        ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT + MOVE_Y,
        al_map_rgb(0, 0, 0), 5);

    al_draw_line((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2 + MOVE_X, ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT / 2 + MOVE_Y,
        ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + DISPLAYWIDTH + MOVE_X,
        ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT - DISPLAYHEIGHT / 2 + MOVE_Y, al_map_rgb(0, 0, 0), 5);

    for (int i = 0; i < 16; i++) {

        al_draw_line(((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + 45 * i + MOVE_X, ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + MOVE_Y,
            ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + 45 * i + MOVE_X, ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + DISPLAYHEIGHT + MOVE_Y,
            al_map_rgb(0, 0, 0), 5);
    }

    for (int i = 0; i < (int) message.size(); i++) {
        buffer[0] = message[i];

        if (i < 16) {
            al_draw_text(font, al_map_rgb(0, 0, 0), ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + 10 + 45 * i + MOVE_X,
                ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2) + MOVE_Y,
                0, buffer);
        }
        else if (i<32) {
            al_draw_text(font, al_map_rgb(0, 0, 0), ((ALLEGRODISPLAYWIDTH - DISPLAYWIDTH) / 2) + 10 + 45 * (i - 16) + MOVE_X,
                ((ALLEGRODISPLAYHEIGHT - DISPLAYHEIGHT) / 2 + 60) + MOVE_Y,
                0, buffer);
        }
    }
    /*
    al_flip_display();
    */

    
    return true;
}

static int CursorPosToNumber(cursorPosition pos) {  //Translate from the cursor position to the index in the string

    return (pos.row) * 16 + (pos.column);
}