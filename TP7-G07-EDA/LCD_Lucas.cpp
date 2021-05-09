#include "LCD_Lucas.h"


LCD_Lucas::LCD_Lucas()
{

	//ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_W, DISPLAY_H);
	ALLEGRO_FONT* font = al_create_builtin_font();
	this->cursor.column = 0;
	this->cursor.row = 0;
	this->initError = 0;
	for (int i = 0; i < LCD_ROWS; i++)
	{
		for (int j = 0; j < LCD_COLUMNS; j++)
		{
			this->array[i][j] = NULL;
		}
	}
}
LCD_Lucas::~LCD_Lucas()
{
	//al_destroy_display(display);
}

cursorPosition LCD_Lucas::lcdGetCursorPosition()
{
	return this->cursor;
}

bool LCD_Lucas::lcdSetCursorPosition(const cursorPosition pos)
{
	if (pos.column >= LCD_COLUMNS || pos.row >= LCD_ROWS) //Fuera de rango
		return false;
	else
	{
		this->cursor = pos;
		return true;
	}
		
}

bool LCD_Lucas::lcdMoveCursorLeft()
{
	if (this->cursor.column <= 0 )
	{								
		if (lcdMoveCursorUp())
		{
			this->cursor.column = LCD_COLUMNS-1;
			return true;//moverse arriba (si puede) 
		}
		else
		{
			return false;
		}
	}
	else
	{
		cursor.column--;
		return true;
	}
}

bool LCD_Lucas::lcdMoveCursorRight()
{
	if (this->cursor.column >= LCD_COLUMNS - 1)//resto uno porque el contador arranca en 0
	{											//resto otro porque si esta en la ultima columna no puede
		if (lcdMoveCursorDown())
		{
			this->cursor.column = 0;
			return true;//moverse abajo (si puede) 
		}
		else
		{
			return false;
		}
	}

	else
	{
		cursor.column++;	//moverse a l a derecha
		return true;
	}
}

bool LCD_Lucas::lcdMoveCursorDown()
{
	if (this->cursor.row >= LCD_ROWS - 1)//resto uno porque el contador arranca en 0
	{
		return false;							
	}
	else
	{
		cursor.row++;
		return true;
	}
}

bool LCD_Lucas::lcdMoveCursorUp()
{
	if (this->cursor.row <= 0)
	{										
		return false;							
	}
	else
	{
		cursor.row--;
		return true;
	}
}

bool LCD_Lucas::lcdClearToEOL()
{
	if (this->cursor.column < LCD_COLUMNS)
	{
		for (int i = this->cursor.column; i < LCD_COLUMNS; i++)
		{
			this->array[this->cursor.row][i] = NULL;
		}
		simularLCD_Lucas();
		return true;
	}
	else
	{
		return false;
	}
}

bool LCD_Lucas::lcdClear()
{
	if (!initError)
	{
		for (int i = 0; i<LCD_ROWS; i++)
		{
			for (int j = 0 ; j<LCD_COLUMNS; j++)
			{
				this->array[i][j] = NULL;
			}
		}
		simularLCD_Lucas();
		return true;
	}
	else
	{
		return false;
	}
}

basicLCD& LCD_Lucas::operator<<(const unsigned char c)
{
	this->array[this->cursor.row][this->cursor.column] = c;
	if (!LCD_Lucas::lcdMoveCursorRight())
	{
		if (LCD_Lucas::lcdMoveCursorDown())
		{
			this->cursor.column = 0;
		}
	}
	simularLCD_Lucas();
	return *this;
}

basicLCD& LCD_Lucas::operator<<(const char* c)
{
	int strLength = 0;
	while (c[strLength] != '\0' && c[strLength] != '\n')
	{
		strLength++;
	}

	if (strLength >= (LCD_ROWS * LCD_COLUMNS)) // 2 x 16 = 32
	{
		this->cursor.column = LCD_COLUMNS-1 ;
		this->cursor.row = LCD_ROWS-1;
		for (int i = LCD_ROWS-1; i >=0; i--)
		{
			for (int j = LCD_COLUMNS-1; j >= 0; j--)
			{
				this->array[i][j] = c[strLength];
				strLength--;
				this->lcdMoveCursorLeft();
			}
		}
	}
	else
	{
		int next = true;
		int i = 0;
		do
		{
			if (c[i] != '\n' && c[i] != '\0')
			{
				this->array[this->cursor.row][this->cursor.column] = c[i];
				i++;
				if (!lcdMoveCursorRight())
				{
					next = false;
				}
			}
			else
			{
				next = false;
			}

		} while (next);
	}
	simularLCD_Lucas();
	return *this;
}




bool LCD_Lucas::lcdInitOk()
{
	if (initError)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LCD_Lucas::simularLCD_Lucas()
{

	for (int i = 0; i < LCD_ROWS; i++)
	{
		for (int j = 0; j < LCD_COLUMNS; j++)
		{
			if (i == 1)
			{
				al_draw_textf(font, al_map_rgb(255, 255, 255), (j * LETTER_W) + LETTER_S, (i * LETTER_H) + LETTER_S, 0, "%c", array[i][j]);
			}
			else
			{
				al_draw_textf(font, al_map_rgb(255, 255, 255), (j * LETTER_W) + LETTER_S, (i * LETTER_H) + LETTER_S, 0, "%c", array[i][j]);
			}
		}
	}

	al_draw_line(cursor.column * LETTER_W + LETTER_S, (cursor.row + 1) * LETTER_H, (cursor.column * LETTER_W + LETTER_W), (cursor.row + 1) * LETTER_H, al_map_rgb(0, 255, 0), 2);

}
lcdError& LCD_Lucas::lcdGetError()
{
	return this->Lucas_LCDError;
}


