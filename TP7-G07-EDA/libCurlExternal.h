#ifndef libCurlExternal_h

#define	libCurlExternal_h
#include <list>
#include <iostream>

class getTweet{
public:
	int getTweets(void);
private:
	void printNames();										////CREO QUE ESTO LO DEBERIAMOS BORRAR
	size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);
	std::list<std::string> names;
};

#endif // !libCurlExternal