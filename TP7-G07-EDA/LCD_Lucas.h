#pragma once
#include "basiclcd.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define DISPLAY_W 200
#define DISPLAY_H 50
#define LETTER_W 10
#define LETTER_H 12
#define LETTER_S 3	//letter space. el espacio entre las letras

#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define LCD_CLEAR_CONDITION 1
class LCD_Lucas : public basicLCD
{
public:
	/*=====================================================
	* Name: basicLCD
	* Entra: -
	* Resulta: Constructor de la clase. Inicializa el LCD y deja
	* todo listo comenzar a utilizarlo.
	*
	* cadd =1 (cursor address) (ver NOTA 1)
	*=====================================================*/
	LCD_Lucas();

	/*=====================================================
	* Name: ~basicLCD
	* Entra: -
	4/22
	* Resulta: Destructor de la clase. Libera cualquier recurso
	* que se hubiera tomado de forma de evitar
	* "resources leak".
	*=====================================================*/
	~LCD_Lucas();
	/*=====================================================
	* Name: lcdInitOk
	* Entra: -
	* Resulta: No genera ningún cambio en el display.
	* Devuelve en su nombre “true” si el display se inicializó
	* correctamente (el constructor no tuvo errores) o “false
	* en caso contrario.
	*=====================================================*/
	bool lcdInitOk();

	/*=====================================================
	* Name: lcdGetError
	* Entra: -
	* Resulta: No genera ningún cambio en el display.
	* Devuelve en su nombre un lcdError&
	*=====================================================*/
	lcdError& lcdGetError() ;
	/*=====================================================
	* Name: lcdClear
	* Entra: -
	* Resulta: Borra el display y el cursor va a HOME
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdClear();
	/*=====================================================
	* Name: lcdClearToEOL
	* Entra: -
	* Resulta: Borra el display desde la posición actual
	* del cursor hasta el final de la línea.
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdClearToEOL();
	/*=====================================================
	* Name: operator<<()
	* Entra: Un carácter
	* Resulta: Pone el carácter en la posición actual
	* del cursor del display y avanza el cursor a la próxima
	* posición respetando el gap (si el carácter no es imprimible
	* lo ignora)
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la función:
	* basicLCD lcd;
	* lcd << ‘a’ << ‘b’ << ‘c’;
	*=====================================================*/
	basicLCD& operator<<(const unsigned char c);

	/*=====================================================
	* Name: operator<<()
	* Entra: Una cadena de caracteres NULL terminated
	* Resulta: imprime la cadena de caracteres en la posición actual
	* del cursor y avanza el cursor al final de la cadena respetando
	* el gap (si algún carácter no es imprimible lo ignora). Si recibe una
	* cadena de más de 32 caracteres, muestra los últimos 32 en el display.
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la función:
	* basicLCD lcd;
	* lcd << “Hola” << “ “ << “Mundo”;
	*=====================================================*/
	basicLCD& operator<<(const char* c);
	/*=====================================================
	* Name: lcdMoveCursorUp
	*
	* Entra: -
	* Resulta: Pasa el cursor a la primera línea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorUp();

	/*=====================================================
	* Name: lcdMoveCursorDown
	*
	* Entra: -
	* Resulta: Pasa el cursor a la segunda línea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorDown();
	/*=====================================================
	* Name: lcdMoveCursorRight
	*
	* Entra: -
	* Resulta: Avanza el cursor una posición
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorRight();
	/*=====================================================
	* Name: lcdMoveCursorLeft
	*
	* Entra: -
	* Resulta: Retrocede el cursor una posición
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdMoveCursorLeft();

	/*=====================================================
	* Name: lcdSetCursorPosition
	* Entra: Recibe una estructura tipo cursorPosition
	* Resulta: Posiciona el cursor en la posición dada
	* por row y column. row[0-1] col[0-19]. Ante un valor inválido
	* de row y/o column ignora la instrucción (no hace nada).
	*
	* Devuelve en su nombre “true” si fue satisfactoria “false”
	* en caso contrario.
	*=====================================================*/
	bool lcdSetCursorPosition(const cursorPosition pos);
	/*=====================================================
	* Name: lcdGetCursorPosition
	* Entra: -
	* Resulta: Devuelve la posición actual del cursor.
	*
	*
	* Devuelve una estructura tipo cursorPosition
	*=====================================================*/
	cursorPosition lcdGetCursorPosition();
	/*=======================================================
	Name: simularLCD
	Entra: Referencia al objeto del LCD a simular
	Resulta: simulacion correspondiente
	=======================================================*/
	void simularLCD_Lucas();
	

	

private:
	lcdError Lucas_LCDError;
	cursorPosition cursor;
	char array[LCD_ROWS][LCD_COLUMNS];		// EL LCD en si
	bool initError;

};
