#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include "Tweet.h"
#include "basicLCD.h"

#define DISPLAYS 3

enum LCDORDER {DAMIAN = 0, KEVIN, MATEO};

enum DisplayState{USERNAMEINPUT, SHOWINGTWEETS, DOWNLOADINGTWEETS};

class Gui {
public:

	Gui(int height, int width, basicLCD* lcdArray[3]);
	~Gui();
	void setup();
	void functions();


private:

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;

	char bufUsername[MAXIMUM_USERNAME_LENGTH + 1] = { 0 };      //Maximum length of a name is 15 characters long
	char bufNumberOfTweets[MAXIMUM_TWEETS_DIGITS + 1] = { 0 };
	int numberOfTweets;
	std::string username;

	float speed = 0.5;  //Varia entre 0 y 1

	bool checkbox_lcdDAMI_selected;
    bool checkbox_lcdKEVIN_selected;
    bool checkbox_lcdMATEO_selected;
	DisplayState state;

	basicLCD * lcdArray[DISPLAYS];
};