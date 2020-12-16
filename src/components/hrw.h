#ifndef __HRW_H__
#define __HRW_H__
#include "MurmurHash3.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

// Custom Libraries
#include "defs.h"

using namespace std;


//hardcoded proxy names
//vector<string> proxy_names {"proxy1", "proxy2", "proxy3", "proxy4", "proxy5"};
string proxy1 = "proxy1";
string proxy2 = "proxy2";
string proxy3 = "proxy3";
string proxy4 = "proxy4";
string proxy5 = "proxy5";

int HRW(string object_name)
{

	string p1 = object_name + proxy1;
	string p2 = object_name + proxy2;
	string p3 = object_name + proxy3;
	string p4 = object_name + proxy4;
	string p5 = object_name + proxy5;
    
    char *one = new char[p1.size() + 1];
	char *two = new char[p2.size() + 1];
	char *three = new char[p3.size() + 1];
	char *four = new char[p4.size() + 1];
	char *five = new char[p5.size() + 1];
	
	strcpy(one, p1.c_str());
	strcpy(two, p2.c_str());
	strcpy(three, p3.c_str());
	strcpy(four, p4.c_str());
	strcpy(five, p5.c_str());
    
    //random seed
    uint64_t seed = 1;
    uint64_t out1[2];
    uint64_t out2[2];
    uint64_t out3[2];
    uint64_t out4[2];
    uint64_t out5[2];
    
    
    //hashing the object with all proxies to find highest weight
    MurmurHash3_x64_128(one, (uint64_t)strlen(one), seed, out1);
    // cout << "key: " << one << endl;
    // cout << "Hash1: " << out1[0] << out1[1] << endl;
    MurmurHash3_x64_128(two, (uint64_t)strlen(two), seed, out2);
    // cout << "key: " << two << endl;
    // cout << "Hash2: " << out2[0] << out2[1] << endl;
    MurmurHash3_x64_128(three, (uint64_t)strlen(three), seed, out3);
    // cout << "key: " << three << endl;
    // cout << "Hash3: " << out3[0] << out3[1] << endl;
    MurmurHash3_x64_128(four, (uint64_t)strlen(four), seed, out4);
    // cout << "key: " << four << endl;
    // cout << "Hash4: " << out4[0] << out4[1] << endl;
    MurmurHash3_x64_128(five, (uint64_t)strlen(five), seed, out5);
    // cout << "key: " << five << endl;
    // cout << "Hash5: " << out5[0] << out5[1] << endl;
    
	delete[] one;
	delete[] two;
	delete[] three;
	delete[] four;
	delete[] five; 


    if (out1[0] > out2[0] && out1[0] > out3[0] && out1[0] > out4[0] && out1[0] > out5[0])
    {
        
        return 0;
    }
    else if (out2[0] > out1[0] && out2[0] > out3[0] && out2[0] > out4[0] && out2[0] > out5[0])
    {
        
        return 1;
    }
    else if (out3[0] > out1[0] && out3[0] > out2[0] && out3[0] > out4[0] && out3[0] > out5[0])
    {
        
        return 2;
    }
    else if (out4[0] > out1[0] && out4[0] > out2[0] && out4[0] > out3[0] && out4[0] > out5[0])
    {
        
        return 3;
    }
    else if (out5[0] > out1[0] && out5[0] > out2[0] && out5[0] > out3[0] && out5[0] > out4[0])
    {
        
        return 4;
    }
    
}

#endif
