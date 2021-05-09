#include "gui.h"

#include <stdio.h>
#include <vector>

using namespace std;

Gui::Gui(int height, int width) {

    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(width, height);
    al_set_window_title(display, "Tweeter Reader");
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    if (!al_init_font_addon()) {

        cout << "failed to initialize allegro fonts!\n" << endl;
    }
    if (!al_init_ttf_addon()) {

        cout << "failed to initialize allegro ttf!\n" << endl;
    }

    numberOfTweets = 0;
    speed = 0.5;
    tweetShownOFFSET = 0;
    actualTweet = 0;

    framesCounter = 0.5 * FRAMESREFERENCE;
}

Gui::~Gui() {

    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);
}

void Gui::setup() {

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAllegro5_Init(display);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

}

DisplayState Gui::functions(DisplayState estado, vector<string>& tweets) {

    LCDNAME name1 = DAMIAN;   //This variables will tell which displays are shown
    LCDNAME name2 = DONTSHOW;
    LCDNAME name3 = DONTSHOW;

    vector<string> ultimoTweet;

    int moveTweet = 0;

    DisplayState salida = estado; 

    al_clear_to_color(al_map_rgb(100,100,100));   //Clearing of the display is made before LCD are written

    ALLEGRO_EVENT ev;

    while (al_get_next_event(queue, &ev))
    {
    ImGui_ImplAllegro5_ProcessEvent(&ev);
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        salida = STOP_RUNNING;

    if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            ImGui_ImplAllegro5_InvalidateDeviceObjects();
            al_acknowledge_resize(display);
            ImGui_ImplAllegro5_CreateDeviceObjects();
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    if (estado == USERNAMEINPUT)
    {
        ImGui::Begin("Twitter Input");

        ImGui::Text("Input the twitter username and quantity of tweets");    // Display some text (you can use a format strings too)

        ImGui::InputText("Username", bufUsername, MAXIMUM_USERNAME_LENGTH +1 );
        ImGui::InputText("Number of tweets", bufNumberOfTweets, MAXIMUM_TWEETS_DIGITS + 1);

        if (ImGui::Button("Done")) {    // Buttons return true when clicked (most widgets return true when edited/activated)

            username = string(bufUsername);
            numberOfTweets = atoi(bufNumberOfTweets);

            salida = DOWNLOADINGTWEETS;
        }

        ImGui::End();
    }
    else if (estado == DOWNLOADINGTWEETS){

        //TODO: Mostramos en el display que se estan descargando los datos

        ImGui::Begin("Cancel Download");

        if (ImGui::Button("Cancel")) {

            salida = STOP_RUNNING;
        }

        ImGui::End();
    }
    else if (estado == SHOWINGTWEETS) {

        framesCounter++;

        ImGui::Begin("Twitter Reader"); 
        ImGui::Text("Select how tweets will be shown");    // Display some text (you can use a format strings too)

        ImGui::Checkbox("Unshow LCD Dami", &checkbox_lcdDAMI_selected);
        if (checkbox_lcdDAMI_selected) {

            name1 = DONTSHOW;
        }

        ImGui::Checkbox("Unshow LCD Kevin", &checkbox_lcdKEVIN_selected);
        if (checkbox_lcdKEVIN_selected) {

            name2 = DONTSHOW;
        }

        ImGui::Checkbox("Unshow LCD Mateo", &checkbox_lcdMATEO_selected);
        if (checkbox_lcdMATEO_selected) {

            name3 = DONTSHOW;
        }

        moveTweet = printInDisplay(name1, name2, name3, tweets);

        if (framesCounter >= (int)(FRAMESREFERENCE * (1 - speed))) {    //Actualizacion del programa

            tweetShownOFFSET++;
            if (moveTweet) {
                actualTweet++;
                if (actualTweet > numberOfTweets - 1) {

                    actualTweet = 0;
                }
            }
            framesCounter = 0;
        }
        if (ImGui::Button("Reshow Tweet")) {
            tweetShownOFFSET = 0;
        }
        if (ImGui::Button("Jump to Previous Tweet")) {
            actualTweet++;
            tweetShownOFFSET = 0;
        }
        if (ImGui::Button("Jump to Next Tweet")) {
            actualTweet--;
            tweetShownOFFSET = 0;
        }
        ImGui::SliderFloat("Display Speed", &speed, 0.0f, 1.0f);

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
    al_flip_display();

    return salida;
}

void Gui::setLCDArray(int index, basicLCD * lcd) {

    lcdArray[index] = lcd;
}

int Gui::printInDisplay(LCDNAME name1, LCDNAME name2, LCDNAME name3, vector<string>& tweets) {

    int salida = 0;
    string tweetprint;  
    basicLCD* lcdShownArray[DISPLAYS];
    int arrayIndex = 0;

    if (name1 == DAMIAN || name2 == DAMIAN || name3 == DAMIAN) {    //Cargo los displays que se hallan llamado

        lcdShownArray[arrayIndex] = lcdArray[DAMIAN];
        arrayIndex++;
    }
    if (name1 == KEVIN || name2 == KEVIN || name3 == KEVIN) {

        lcdShownArray[arrayIndex] = lcdArray[KEVIN];
        arrayIndex++;
    }
    if (name1 == MATEO || name2 == MATEO || name3 == MATEO) {

        lcdShownArray[arrayIndex] = lcdArray[MATEO];
        arrayIndex++;
    }
    for (; arrayIndex < DISPLAYS; arrayIndex++) {       //Lleno los que no se llamaron con NULL

        lcdShownArray[arrayIndex] = NULL;
    }


    for (int i = 0; i < DISPLAYS; i++) {

        if (lcdShownArray[i] != NULL) {
            
            lcdShownArray[i]->lcdClear();

            *(lcdShownArray[i]) << (tweets[actualTweet * 2]).c_str();   //Imprimo la primera linea

            *(lcdShownArray[i]) << ' ';
            
            tweetprint = (tweets[actualTweet * 2 + 1]).substr(tweetShownOFFSET, 16);    //Imprimo la 2da linea y la voy corriendo

            *(lcdShownArray[i]) << tweetprint.c_str();

            if (tweetprint[15] == '\0') {
                
                salida = 1;
                tweetShownOFFSET = 0;
                actualTweet++;

            }
        }
    }

    return salida;
}