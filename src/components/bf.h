#ifndef __BF_H__
#define __BF_H__


#include <iostream>
#include <string>
#include <cstring>
#include <bitset>
#include <unordered_set>

#include "MurmurHash3.h"
#include "defs.h"
using namespace std;

class bloom_filter {
public:
	char* arr; 

	bloom_filter():arr(nullptr){}

	// reset/deallocate the array to appropriate size
	void reset(int size)
	{
		if(this->arr)
		{
			delete[] this->arr;
		}

		if(size > 0)
		{
			this->arr = new char[FILTER_SIZE / 8]{0};
		}
	}

	~bloom_filter(){
		reset(0);
	}
    
	//fill bloom filter array with zeroes. Hash objects in blacklist into it.
	void initialize(char* blacklist[], int size)
	{
		uint64_t out[2];
		int index;
		int byte_index;
		int bit_index;

		reset(size);
		
		for(int i = 0; i < size; i++)
		{
			MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),1,out);
			index = (unsigned long)out[0] % FILTER_SIZE; // this gives the bit to set to 1
			byte_index = index / 8; //this gives the index of the byte.
			bit_index = index % 8; //gives us the bit in that byte to set to 1
			this->arr[byte_index] |= (1 << bit_index); //set the bit to 1
			
			MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),2,out);
			index = (unsigned long)out[0] % FILTER_SIZE;
			byte_index = index / 8; //this gives the index of the byte.
			bit_index = index % 8; //gives us the bit in that byte to set to 1
			this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
			
			MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),3,out);
			index = (unsigned long)out[0] % FILTER_SIZE;
			byte_index = index / 8; //this gives the index of the byte.
			bit_index = index % 8; //gives us the bit in that byte to set to 1
			this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
			
			MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),4,out);
			index = (unsigned long)out[0] % FILTER_SIZE;
			byte_index = index / 8; //this gives the index of the byte.
			bit_index = index % 8; //gives us the bit in that byte to set to 1
			this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
			
			MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),5,out);
			index = (unsigned long)out[0] % FILTER_SIZE;
			byte_index = index / 8; //this gives the index of the byte.
			bit_index = index % 8; //gives us the bit in that byte to set to 1
			this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
		}
	}


	// overloaded initialize for the specific structure we're using
	void initialize(const unordered_set<string>& blacklist)
	{

		// transform set into cstring array for bloom filter
		int size = blacklist.size();
		char** cstr_blacklist = new char*[size];

		int i = 0;
		for(auto item: blacklist)
		{
			cstr_blacklist[i] = new char[item.size() + 1];
			cstr_blacklist[i][item.size()] = '\0';
			strcpy(cstr_blacklist[i], item.c_str());
			++i;
		}

		// call previous initialize function
		this->initialize(cstr_blacklist, size);

		// delete/free allocated blacklist
		for(int i = 0; i < size; ++i)
		{
			delete[] cstr_blacklist[i];
		}

		delete[] cstr_blacklist;

	}


	//returns 1 if object is in blacklist, 0 if not.
	int check(const char* object)
	{
		uint64_t *out[2];
		int count = 0;
		int index;
		int byte_index;
		int bit_index;
		
		MurmurHash3_x64_128(object,(uint64_t)strlen(object),1,out);
		index = (unsigned long)out[0] % FILTER_SIZE;
		byte_index = index / 8; //this gives the index of the byte.
		bit_index = index % 8; //gives us the bit in that byte to set to 1
		if (arr[byte_index] & (1 << bit_index)) {count++;}
		

		MurmurHash3_x64_128(object,(uint64_t)strlen(object),2,out);
		index = (unsigned long)out[0] % FILTER_SIZE;
		byte_index = index / 8; //this gives the index of the byte.
		bit_index = index % 8; //gives us the bit in that byte to set to 1
		if (arr[byte_index] & (1 << bit_index)) {count++;}


		MurmurHash3_x64_128(object,(uint64_t)strlen(object),3,out);
		index = (unsigned long)out[0] % FILTER_SIZE;
		byte_index = index / 8; //this gives the index of the byte.
		bit_index = index % 8; //gives us the bit in that byte to set to 1
		if (arr[byte_index] & (1 << bit_index)) {count++;} 


		MurmurHash3_x64_128(object,(uint64_t)strlen(object),4,out);
		index = (unsigned long)out[0] % FILTER_SIZE;
		byte_index = index / 8; //this gives the index of the byte.
		bit_index = index % 8; //gives us the bit in that byte to set to 1
		if (arr[byte_index] & (1 << bit_index)) {count++;} 


		MurmurHash3_x64_128(object,(uint64_t)strlen(object),5,out);
		index = (unsigned long)out[0] % FILTER_SIZE;
		byte_index = index / 8; //this gives the index of the byte.
		bit_index = index % 8; //gives us the bit in that byte to set to 1
		if (arr[byte_index] & (1 << bit_index)) {count++;}    


		if(count == 5)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

};

#endif
