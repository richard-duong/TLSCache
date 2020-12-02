#include "MurmurHash3.cpp"
#include <iostream>
#include <cstring>
using namespace std;


class bloom_filter {
    public:
        char arr[7500];
    
    
//fill bloom filter array with zeroes. Hash objects in blacklist into it.
void initialize(const char* blacklist[], int size)
{
    uint64_t *out[2];
    int index;
    
    for(int i = 0; i < size; i++)
    {
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),1,out);
        index = (unsigned long)out[0] % 7500;
        this->arr[index] = 1;
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),2,out);
        index = (unsigned long)out[0] % 7500;
        this->arr[index] = 1;
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),3,out);
        index = (unsigned long)out[0] % 7500;
        this->arr[index] = 1;
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),4,out);
        index = (unsigned long)out[0] % 7500;
        this->arr[index] = 1;
        MurmurHash3_x64_128(blacklist[i],(uint64_t)strlen(blacklist[i]),5,out);
        index = (unsigned long)out[0] % 7500;
        this->arr[index] = 1;
    }
    return;
}

//copies recvBuffer to arr
void accept(char* recvBuffer)
{
    for (int i = 0; i < 7500; i++)
    {
        this->arr[i] = recvBuffer[i];
    }
    return;
}


//returns 1 if object is in blacklist, 0 if not.
int check(const char* object)
{
    uint64_t *out[2];
    int count = 0;
    int index;
    MurmurHash3_x64_128(object,(uint64_t)strlen(object),1,out);
    index = (unsigned long)out[0] % 7500;
    if (this->arr[index] == 1) {count++;}
    MurmurHash3_x64_128(object,(uint64_t)strlen(object),2,out);
    index = (unsigned long)out[0] % 7500;
    if (this->arr[index] == 1) {count++;}
    MurmurHash3_x64_128(object,(uint64_t)strlen(object),3,out);
    index = (unsigned long)out[0] % 7500;
    if (this->arr[index] == 1) {count++;}
    MurmurHash3_x64_128(object,(uint64_t)strlen(object),4,out);
    index = (unsigned long)out[0] % 7500;
    if (this->arr[index] == 1) {count++;}
    MurmurHash3_x64_128(object,(uint64_t)strlen(object),5,out);
    index = (unsigned long)out[0] % 7500;
    if (this->arr[index] == 1) {count++;}
    
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

