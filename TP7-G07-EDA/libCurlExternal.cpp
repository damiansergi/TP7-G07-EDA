#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include <string>
#include <ctime>
#include "jsonExternal.hpp"
#include "libCurlExternal.h"

#pragma warning(disable : 4996)
//Vamos a usar la librería NLOHMANN JSON 
using json = nlohmann::json;

size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);


int getTweet::getTweets(std::string account, Gui& gui, int number_of_tweets=10)
{
	std::vector<std::string> tweets; //Dummy para llamar a state.functions

	json j;                    //Variable donde vamos a guardar lo que devuelva Twitter

	// Vamos a utilizar la librería CURL ya que debemos conectarons a un servidor HTTPS
	// (HTTP seguro) el cual requeire un protocolo especial de encriptación
	// más complejo que el plain HTTP que utilizamos en el TP de Networking.

	CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLM* multiHandle;			//Variable donde vamos a atachear los easy handles
	CURLcode res;
	std::string readString, token;

	// Query es la dirección de Twitter que vamos a consultar. vamos a bajar los &count twits de screen_name en formato JSON.
	std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=" + account + "&count=" + std::to_string(number_of_tweets);

	// Las dos constantes de abajo son el API_Key y API_SecretKey que necesita Twitter para realizar la autenticación de nuestra App
	// y por lo tanto permitirnos el acceso a sus servidores. Son parte de la estrategia de autenticación Oauth2.
	//Ambas se obtienen de la app que creen en la pagina de Twitter
	std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";


	/************************************************************************************
	*                      Get Bearer Token from the Twitter API						*
	************************************************************************************/

	// Primero realizamos la inicialización de curl para obtener el token de Twitter el cual vamos
	// a utilizar luego para realizar la autenticación por Oauth2.
	curl = curl_easy_init();
	if (curl)
	{
		//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Si la página nos redirije a algún lado, le decimos a curl que siga dicha redirección.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticación por HTTP en modo básico.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Preparamos el password para la autenticación
		std::string userPwd = API_key + ":" + API_SecretKey;
		//Se lo seteamos a CURL.
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

		struct curl_slist* list = NULL;
		//Pueden setear el Header con la linea de abajo, pero necesitan codificar las claves en Base64
		//list = curl_slist_append(list, "Authorization: Basic YlB5alk1bWRMR2V4TlhPTHhSUjd3MUVjUzpkR2liU3FIcURrektQMUtPbzFJTjRBd21tZGI1Tnl5ZjBFQTZkTDBLWlpmZDE0ZnhQQw==");

		//Le decimos a CURL que vamos a mandar URLs codificadas y además en formato UTF8.
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Le decimos a CURL que trabaje con credentials.
		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//Le decimos a curl que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		// Perform the request, res will get the return code
		// Con lo de abajo le decimos a curl que intente conectarse a la página (recordemos que la idea es
		// obtener la autenticación así luego Twitter nos deja conectarnos a cualquier usuario público)
		//Recordar que easy_perform es bloqueante, en este caso es correcto asi al ser inicializacion y no tardar mucho tiempo.
		res = curl_easy_perform(curl);

		// Nos fijamos si hubo algún error
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return 0;
		}

		// Si no hubo errores hago un clean up antes de realizar un nuevo query.
		curl_easy_cleanup(curl);


		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//Abajo busco el token en el JSON para luego acceder a los twits.
		j = json::parse(readString);

		//Se encierra el parseo en un bloque try-catch porque la libreria maneja errores por excepciones.
		//Pueden tambien parsear usando iteradores o la forma que quieras, buscar en la documentacion las diferentes formas.
		try
		{
			//Tratamos de acceder al campo acces_token del JSON
			std::string aux = j["access_token"];
			token = aux;
			//std::cout << "Bearer Token get from Twitter API: \n" << token << std::endl;
		}
		catch (std::exception& e)
		{
			//Si hubo algun error, se muestra el error que devuelve la libreria
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
		return 0;
	}



	//Una vez obtenido el Token ahora voy a buscar los Twits

	/************************************************************************************
	*                        Get Tweets from the Twitter API							*
	************************************************************************************/

	// Nuevamente reinicializo todo curl.
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist* list = NULL;
		std::string aux = "Authorization: Bearer ";
		aux = aux + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning)
		{

			stillRunning = gui.functions(DOWNLOADINGTWEETS, tweets);

			//Debemos hacer polling de la transferencia hasta que haya terminado
			curl_multi_perform(multiHandle, &stillRunning);

		}

		//Checkeamos errores
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return 0;
		}

		//Siempre realizamos el cleanup al final
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//con toda la informacion de los tweets que le pedimos
		j = json::parse(readString);

		//std::cout << readString << std::endl;

		try
		{
			//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
			for (auto element : j) {
				names.push_back(element["created_at"]);
				names.push_back(element["text"]);
			}
			std::cout << "Tweets retrieved from Twitter account: " << std::endl;
			saveNames(account);
		}
		catch (std::exception& e)
		{
			//Muestro si hubo un error de la libreria
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}
	else
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
	return 0;
}


//Funcion auxiliar para imprimir los tweets en pantalla una vez parseados
void getTweet::saveNames(std::string account)
{
	//std::string temp;
	for (auto c = names.begin(); c != names.end(); c++)
	{
		std::string y, m, m2, d, h, min, temp;
		y = (*c).substr(28, 2);
		m = (*c).substr(4, 3);
		d = (*c).substr(8, 2);
		h = (*c).substr(11, 2);
		min = (*c).substr(14, 2);
		//std::cout << y << std::endl;
		//std::cout << m << std::endl;
		//std::cout << d << std::endl;
		//std::cout << h << std::endl;
		//std::cout << min << std::endl;
		if (!strcmp(m.c_str(), "Jan")) m2 = "01";
		if (!strcmp(m.c_str(), "Feb")) m2 = "02";
		if (!strcmp(m.c_str(), "Mar")) m2 = "03";
		if (!strcmp(m.c_str(), "Apr")) m2 = "04";
		if (!strcmp(m.c_str(), "May")) m2 = "05";
		if (!strcmp(m.c_str(), "Jun")) m2 = "06";
		if (!strcmp(m.c_str(), "Jul")) m2 = "07";
		if (!strcmp(m.c_str(), "Aug")) m2 = "08";
		if (!strcmp(m.c_str(), "Sep")) m2 = "09";
		if (!strcmp(m.c_str(), "Oct")) m2 = "10";
		if (!strcmp(m.c_str(), "Nov")) m2 = "11";
		if (!strcmp(m.c_str(), "Dec")) m2 = "12";
		temp = d + '/' + m2 + '/' + y + " - " + h + ':' + min;
		info.push_back(temp);
		std::cout << temp << std::endl;
		c++;
		int extended = (*c).find("https"); 		//Eliminamos el URL al final para mostrar
		temp = account + ": - " + (*c).substr(0, extended) + " -";
		info.push_back(temp);
		std::cout << temp << std::endl;
	}
}

std::list<std::string> getTweet::getinfo() {
	return info;
}

//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}