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

#include <stdint.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

enum DisplayState{USERNAMEINPUT, SHOWINGTWEETS, DOWNLOADINGTWEETS};

using namespace std;

int main(int, char**)
{
    // Setup Allegro
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    al_set_window_title(display, "Tweeter Reader");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    if (!al_init_font_addon()) {

        cout << "failed to initialize allegro fonts!\n" << endl;
    }
    if (!al_init_ttf_addon()) {

        cout << "failed to initialize allegro ttf!\n" << endl;
    }

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

    
    /*
    *
    *
    NUESTRA PARTE DEL CODIGO
    *
    *
    */

    char bufUsername[MAXIMUM_USERNAME_LENGTH + 1] = { 0 };   //Maximum length of a name is 15 characters long
    char bufNumberOfTweets[MAXIMUM_TWEETS_DIGITS + 1] = { 0 };      //TODO: What's the maximum number of tweets that can be demanded?
    int numberOfTweets;
    std::string username;
    DisplayState state = USERNAMEINPUT; //We begin reading the username and number of tweets that will be read

    bool checkbox_lcdDAMI_selected;
    bool checkbox_lcdKEVIN_selected;
    bool checkbox_lcdMATEO_selected;

    float speed = 0.5;  //Varia entre 0 y 1

    basicLCD* p2LCD;

    damianLCD LCDDamian;

    /*
    *
    *-------------------*
    *
    */

    // Main loop
    bool running = true;

    while (running)
    {
        al_clear_to_color(al_map_rgb(100,100,100));   //Clearing of the display is made before LCD are written

        ALLEGRO_EVENT ev;
        while (al_get_next_event(queue, &ev))
        {
            ImGui_ImplAllegro5_ProcessEvent(&ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                running = false;
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

        if (state == USERNAMEINPUT)
        {
            ImGui::Begin("Twitter Input");

            ImGui::Text("Input the twitter username and quantity of tweets");    // Display some text (you can use a format strings too)

            ImGui::InputText("Username", bufUsername, MAXIMUM_USERNAME_LENGTH +1 );
            ImGui::InputText("Number of tweets", bufNumberOfTweets, MAXIMUM_TWEETS_DIGITS + 1);

            if (ImGui::Button("Done")) {    // Buttons return true when clicked (most widgets return true when edited/activated)

                username = string(bufUsername);
                numberOfTweets = atoi(bufNumberOfTweets);

                //if ( GetTweets(username, numberOfTweets, blablabla) == algo ){ Repetimos la entrada de datos } Threads?

                state = SHOWINGTWEETS;
            }

            ImGui::End();
        }
        else if (state == DOWNLOADINGTWEETS){

            //Mostramos en el display que se estan descargando los datos

            ImGui::Begin("Cancel Download");

            if (ImGui::Button("Cancel")) {

                //cancelamos el thread?
            }

            ImGui::End();
        }
        else if (state == SHOWINGTWEETS) {

            ImGui::Begin("Twitter Reader"); 
            //ImGui::SetWindowSize();   //Hacemos la ventana mas grande para que entre todo de una

            ImGui::Text("Select how tweets will be shown");    // Display some text (you can use a format strings too)

            ImGui::Checkbox("Show LCD Dami", &checkbox_lcdDAMI_selected);
            if (checkbox_lcdDAMI_selected) {

                LCDDamian << "ueueueue";
            }

            ImGui::Checkbox("Show LCD Kevin", &checkbox_lcdKEVIN_selected);
            if (checkbox_lcdKEVIN_selected)
                ImGui::Text("Checkbox B seleccionado");

            ImGui::Checkbox("Show LCD Mateo", &checkbox_lcdMATEO_selected);
            if (checkbox_lcdMATEO_selected)
                ImGui::Text("Checkbox C seleccionado");

            if (ImGui::Button("Reshow Tweet")) {

            }
            if (ImGui::Button("Jump to Previous Tweet")) {

            }
            if (ImGui::Button("Jump to Next Tweet")) {

            }
            ImGui::SliderFloat("Display Speed", &speed, 0.0f, 1.0f);


            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}