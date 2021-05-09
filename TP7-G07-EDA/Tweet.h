#pragma once

#include <iostream>

#define MAXIMUM_USERNAME_LENGTH 15
#define MAXIMUM_TWEETS_DIGITS 3

class Tweet {

public:

private:

	std::string message;
	std::string user;
	std::string date;

		//Despues vemos si nos sirve o no
	bool retweet;
	int likes;
	int comments;
};