#ifndef HIGHESTRANDOMWEIGHT
#define HIGHESTRANDOMWEIGHT
#include "MurmurHash3.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;


//hardcoded proxy names
//vector<string> proxy_names {"proxy1", "proxy2", "proxy3", "proxy4", "proxy5"};
char proxy1[] = "proxy1";
char proxy2[] = "proxy2";
char proxy3[] = "proxy3";
char proxy4[] = "proxy4";
char proxy5[] = "proxy5";

int HRW(const char* object_name)
{
    char oneone[strlen(object_name)];
    strcpy(oneone, object_name);
    char twotwo[strlen(object_name)];
    strcpy(twotwo, object_name);
    char threethree[strlen(object_name)];
    strcpy(threethree, object_name);
    char fourfour[strlen(object_name)];
    strcpy(fourfour, object_name);
    char fivefive[strlen(object_name)];
    strcpy(fivefive, object_name);

    
    const char *one = strcat(oneone, proxy1);
    const char *two = strcat(twotwo, proxy2);
    const char *three = strcat(threethree, proxy3);
    const char *four = strcat(fourfour, proxy4);
    const char *five = strcat(fivefive, proxy5);

    
    //random seed
    uint64_t seed = 1;
    uint64_t *out1[2];
    uint64_t *out2[2];
    uint64_t *out3[2];
    uint64_t *out4[2];
    uint64_t *out5[2];
    

    
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
    
    
    cout << "Hash1: " << out1[0] << out1[1] << endl;
    cout << "Hash2: " << out2[0] << out2[1] << endl;
    cout << "Hash3: " << out3[0] << out3[1] << endl;
    cout << "Hash4: " << out4[0] << out4[1] << endl;
    cout << "Hash5: " << out5[0] << out5[1] << endl;
    
    
    if (out1 > out2 && out1 > out3 && out1 > out4 && out1 > out5)
    {
        
        return 0;
    }
    else if (out2 > out1 && out2 > out3 && out2 > out4 && out2 > out5)
    {
        
        return 1;
    }
    else if (out3 > out1 && out3 > out2 && out3 > out4 && out3 > out5)
    {
        
        return 2;
    }
    else if (out4 > out1 && out4 > out2 && out4 > out3 && out4 > out5)
    {
        
        return 3;
    }
    else if (out5 > out1 && out5 > out2 && out5 > out3 && out5 > out4)
    {
        
        return 4;
    }
    
}

#endif
