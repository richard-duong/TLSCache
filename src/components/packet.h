#ifndef __PACKET_H__
#define __PACKET_H__

// standard libraries
#include <cstring>


// custom libraries
#include "defs.h"


class packet{

private:
	string buffer;

public:
	packet()
	{
		this->buffer = "";
	}

	void receive(char* recvBuffer, int size)
	{
		this->buffer.assign(recvBuffer, size);
		string prefix = this->buffer.substr(0, 3);
		if(PREFIXES.find(prefix) == PREFIXES.end()){
			this->buffer = "";
		}

	}

	string getPrefix()
	{
		return this->buffer.substr(0, 3);	
	}

	string getMsg()
	{
		return this->buffer.substr(3);
	}

	const char* c_str()
	{
		return this->buffer.c_str();
	}


	void put(string objname)
	{
		this->buffer = "PUT" + objname;	
	}

	void get(string objname)
	{
		this->buffer = "GET" + objname;
	}

	void non()
	{
		this->buffer = "NON";
	}

	void den()
	{
		this->buffer = "DEN";
	}

};

#endif
