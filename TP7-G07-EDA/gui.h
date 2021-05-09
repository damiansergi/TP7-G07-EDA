#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include <vector>

#include "Tweet.h"
#include "basicLCD.h"

#define DISPLAYS 3
#define FRAMESREFERENCE 250
#define PAUSA_PRUDENCIA 2

enum LCDNAME {DAMIAN = 0, KEVIN, MATEO, DONTSHOW};

enum DisplayState {STOP_RUNNING = 0, USERNAMEINPUT, DOWNLOADINGTWEETS, SHOWINGTWEETS, TRANSFORMINGTWEETS, PRINTERROR};

class Gui {
public:

	//Methods
	Gui(int height, int width);
	~Gui();
	void setup();

	DisplayState functions(DisplayState estado, vector<string>& tweets);
	int printInDisplay(LCDNAME name1, LCDNAME name2, LCDNAME name3, vector<string>& tweets);
	void setLCDArray(int index, basicLCD* lcd);

	//Variables
	int numberOfTweets;
	std::string username;


private:

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;

	char bufUsername[MAXIMUM_USERNAME_LENGTH + 1] = { 0 };      //Maximum length of a name is 15 characters long
	char bufNumberOfTweets[MAXIMUM_TWEETS_DIGITS + 1] = { 0 };

	float speed = 0.5;  //Varia entre 0 y 1

	bool checkbox_lcdDAMI_selected;
    bool checkbox_lcdKEVIN_selected;
    bool checkbox_lcdMATEO_selected;

	int tweetShownOFFSET;
	int actualTweet;
	int framesCounter;

	basicLCD * lcdArray[DISPLAYS];
};