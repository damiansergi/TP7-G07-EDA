#pragma warning( disable : 26451 )

#include"KevinLCD.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>

#define IMG "img/background.jpg" 
#define SANS "img/Sans.ttf"
#define TEXT_SIZE 30
#define COLUMN  16
#define ROW 2

#define FPS 20

#define INIT_X_POS 100
#define X_OFFSET 22.5
#define Y_OFFSET 35
#define INIT_Y_POS 90

#define MOVE_X 30
#define MOVE_Y 30

/*ERROR CODES*/
enum ERROR { KEYBOARD_ERROR = 1, DISPLAY_ERROR, IMAGE_ERROR, EVENT_QUEUE_ERROR, FONT_ERROR, CURSOR_SET, CURSOR_UP, CURSOR_DOWN, CURSOR_LEFT, CURSOR_RIGHT, CURSOR_STILL_RIGHT };


ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_BITMAP* img = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_COLOR letter = al_map_rgb(34, 46, 32);

using namespace std;

bool KevinLCD::lcdInitOk() {
    /*INICIALIZACION DE TECLADO*/
    if (!al_install_keyboard()) {
        this->myerror = lcdError("Allegro", "Failed to initialize the keyboard!", KEYBOARD_ERROR);
        return false;
    }

    /*INICIALIZACION DE EVENTOS*/
    event_queue = al_create_event_queue();

    if (!event_queue) {
        this->myerror = lcdError("Allegro", "Failed to create event_queue!", EVENT_QUEUE_ERROR);
        return false;
    }

    img = al_load_bitmap(IMG);
    if (!img) {
        this->myerror = lcdError("Allegro", "Failed to load image!", IMAGE_ERROR);
        al_destroy_event_queue(event_queue);
        return false;
    }

    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    font = al_load_ttf_font(SANS, TEXT_SIZE, 0);
    if (!font) {
        this->myerror = lcdError("Allegro", "Could not load 'Sans.ttf", FONT_ERROR);
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(img);
        return false;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO

    return true;
}

KevinLCD::KevinLCD() :myerror("No error", "No error", 0) {  // Inicializacion
    cursor.column = 1;
    cursor.row = 1;
    str = "";
    str.resize(COLUMN * ROW, ' ');
};

KevinLCD::~KevinLCD() {
    //Destruir recursor empleados 
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(img);
    al_destroy_font(font);
}

lcdError& KevinLCD::lcdGetError() {
    return myerror;
};

bool KevinLCD::lcdClear() {
    str.resize(0, ' ');             // Borro el string
    str.resize(COLUMN * ROW, ' ');
    al_draw_bitmap(img, 0+MOVE_X, 0+MOVE_Y, 0);
    al_flip_display();
    cursor.column = 1;              // Reseteo el cursor
    cursor.row = 1;
    return true;
}

bool KevinLCD::lcdClearToEOL() {
    str.resize(cursor.column + (cursor.row - 1) * COLUMN - 1, ' ');     // Borro el string desde el cursor
    str.resize(COLUMN * ROW, ' ');                                      // y lo completo con espacios
    al_draw_bitmap(img, 0 + MOVE_X, 0 + MOVE_Y, 0);
    *this << ' ';
    lcdMoveCursorLeft();
    return true;
}

basicLCD& KevinLCD::operator<<(const char c) {
    al_draw_bitmap(img, 0+MOVE_X, 0+MOVE_Y, 0);
    if ((cursor.column + (cursor.row - 1) * COLUMN >= ROW * COLUMN) && (myerror.getErrorCode() == CURSOR_RIGHT)) {  //Caso en el que el cursor se encuentra al final de todo
        for (int i = 0; i < ROW * COLUMN - 1; i++) {
            str[i] = str[i + 1];
        }
    }
    str.insert((cursor.column + (cursor.row - 1) * COLUMN) - 1, 1, c);
    for (int j = 0; j < ROW; j++) {
        if (j == ROW - 1) {
            for (int i = 0; i < COLUMN; i++) {      // Caso segunda columna
                al_draw_textf(font, letter, INIT_X_POS + i * X_OFFSET+MOVE_X, INIT_Y_POS + Y_OFFSET + MOVE_Y, ALLEGRO_ALIGN_CENTER, "%c", str[i + COLUMN]);
            }
        }
        else {
            for (int i = 0; i < COLUMN; i++) {      // Caso primera columna
                al_draw_textf(font, letter, INIT_X_POS + i * X_OFFSET + MOVE_X, INIT_Y_POS + MOVE_Y, ALLEGRO_ALIGN_CENTER, "%c", str[i]);
            }
        }
    }
    lcdMoveCursorRight();
    al_flip_display();
    //al_rest(1.0 / FPS);
    return *this;

}

basicLCD& KevinLCD::operator<<(const char* c) {
    for (int i = 0; c[i] != '\0'; i++) {        // Escribo caracter por caracter
        *this << c[i];
    }
    if (myerror.getErrorCode() == CURSOR_RIGHT) {
        this->myerror = lcdError("Cursor", "Can´t move cursor right", CURSOR_STILL_RIGHT);
    }
    return *this;
}

bool KevinLCD::lcdMoveCursorUp() {
    if (cursor.row == 1) {
        this->myerror = lcdError("Cursor", "Can´t move cursor up", CURSOR_UP);
        return false;
    }
    cursor.row = 1;
    return true;
}

bool KevinLCD::lcdMoveCursorDown() {
    if (cursor.row == 2) {
        this->myerror = lcdError("Cursor", "Can´t move cursor down", CURSOR_DOWN);
        return false;
    }
    cursor.row = 2;
    return true;
}

bool KevinLCD::lcdMoveCursorRight() {
    if (cursor.column < COLUMN) {
        cursor.column++;
    }
    else {
        if (cursor.row == 1) {
            cursor.column = 1;
            return lcdMoveCursorDown();
        }
        else {
            this->myerror = lcdError("Cursor", "Can´t move cursor right", CURSOR_RIGHT);
            return false;
        }
    }
    return true;
}

bool KevinLCD::lcdMoveCursorLeft() {
    if (cursor.column > 1) {
        cursor.column--;
    }
    else {
        if (cursor.row == 2) {
            cursor.column = 1;
            return lcdMoveCursorUp();
        }
        else {
            this->myerror = lcdError("Cursor", "Can´t move cursor left", CURSOR_LEFT);
            return false;
        }
    }
    return true;
}




bool KevinLCD::lcdSetCursorPosition(const cursorPosition pos) {
    if (cursor.column > COLUMN || cursor.row > ROW || cursor.column < 0 || cursor.row < 0) {
        this->myerror = lcdError("Cursor", "Can´t set the cursor", CURSOR_SET);
        return false;
    }
    cursor.column = pos.column + 1;
    cursor.row = pos.row + 1;
    return true;
}

cursorPosition KevinLCD::lcdGetCursorPosition() {
    return cursor;
}