#include "MurmurHash3.cpp"
#include <iostream>
#include <cstring>
#include <bitset>
using namespace std;


class bloom_filter {
    public:
        char arr[7500] = {0};
    
    
//fill bloom filter array with zeroes. Hash objects in blacklist into it.
void initialize(const char* blacklist[], int size)
{
    uint64_t *out[2];
    int index;
    int byte_index;
    int bit_index;
    
    for(int i = 0; i < size; i++)
    {
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),1,out);
        index = (unsigned long)out[0] % 60000; // this gives the bit to set to 1
        byte_index = index / 8; //this gives the index of the byte.
        bit_index = index % 8; //gives us the bit in that byte to set to 1
        this->arr[byte_index] |= (1 << bit_index); //set the bit to 1
        
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),2,out);
        index = (unsigned long)out[0] % 60000;
        byte_index = index / 8; //this gives the index of the byte.
        bit_index = index % 8; //gives us the bit in that byte to set to 1
        this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
        
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),3,out);
        index = (unsigned long)out[0] % 60000;
        byte_index = index / 8; //this gives the index of the byte.
        bit_index = index % 8; //gives us the bit in that byte to set to 1
        this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
        
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),4,out);
        index = (unsigned long)out[0] % 60000;
        byte_index = index / 8; //this gives the index of the byte.
        bit_index = index % 8; //gives us the bit in that byte to set to 1
        this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
        
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),5,out);
        index = (unsigned long)out[0] % 60000;
        byte_index = index / 8; //this gives the index of the byte.
        bit_index = index % 8; //gives us the bit in that byte to set to 1
        this->arr[byte_index] |= (1<<bit_index); //set the bit to 1
    }
}

// FIX THIS
//copies recvBuffer to arr
void accept(char* recvBuffer)
{
    for (int i = 0; i < 60000; i++)
    {
        this->arr[i] = recvBuffer[i];
    }
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
    index = (unsigned long)out[0] % 60000;
    byte_index = index / 8; //this gives the index of the byte.
    bit_index = index % 8; //gives us the bit in that byte to set to 1
    if (arr[byte_index] & (1 << bit_index)) {count++;}
    

    MurmurHash3_x64_128(object,(uint64_t)strlen(object),2,out);
    index = (unsigned long)out[0] % 60000;
    byte_index = index / 8; //this gives the index of the byte.
    bit_index = index % 8; //gives us the bit in that byte to set to 1
    if (arr[byte_index] & (1 << bit_index)) {count++;}


    MurmurHash3_x64_128(object,(uint64_t)strlen(object),3,out);
    index = (unsigned long)out[0] % 60000;
    byte_index = index / 8; //this gives the index of the byte.
    bit_index = index % 8; //gives us the bit in that byte to set to 1
    if (arr[byte_index] & (1 << bit_index)) {count++;} 


    MurmurHash3_x64_128(object,(uint64_t)strlen(object),4,out);
    index = (unsigned long)out[0] % 60000;
    byte_index = index / 8; //this gives the index of the byte.
    bit_index = index % 8; //gives us the bit in that byte to set to 1
    if (arr[byte_index] & (1 << bit_index)) {count++;} 


    MurmurHash3_x64_128(object,(uint64_t)strlen(object),5,out);
    index = (unsigned long)out[0] % 60000;
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

