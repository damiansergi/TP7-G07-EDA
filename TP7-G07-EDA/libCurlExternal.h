#ifndef libCurlExternal_h

#define	libCurlExternal_h
#include <list>
#include <iostream>

class getTweet {
public:
	int getTweets(std::string account, int number_of_tweets);
	std::list<std::string> getinfo();
private:
	void saveNames(std::string account);
	std::list<std::string> names;
	std::list<std::string> info;
};

#endif // !libCurlExternal