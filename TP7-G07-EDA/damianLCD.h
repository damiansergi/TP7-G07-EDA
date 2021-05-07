#ifndef damianLCD_H

#define damianLCD_H

#include "basicLCD.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class damianLCD : public basicLCD {

public:
	damianLCD();
	~damianLCD();

	virtual bool lcdInitOk();
	virtual lcdError& lcdGetError();
	virtual bool lcdClear();
	virtual bool lcdClearToEOL();
	virtual basicLCD& operator<<(const unsigned char c);
	virtual basicLCD& operator<<(const char* c);
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

	bool printMessage();

private:

	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;
	lcdError damianLCDError;
	bool error;

	string message;
	cursorPosition pos;
};

#endif // !damianLCD_H