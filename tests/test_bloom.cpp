#include "bloom.cpp"
#include <iostream>
#include <cstring>

using namespace std;

int main(){
    
    const char* blacklist[] = {"Jacob", "Richard"};
    
    bloom_filter test;
    test.initialize(blacklist,2);
    
    if(test.check("Richard") == 1)
    {
        cout << "Richard is on the blacklist." << endl;
    }
    else
    {
        cout << "Richard is not on the blacklist." << endl;
    }
    if(test.check("Jacob") == 1)
    {
        cout << "Jacob is on the blacklist." << endl;
    }
    else
    {
        cout << "Jacob is not on the blacklist." << endl;
    }
    if(test.check("Hello") == 1)
    {
        cout <<"Hello is on the blacklist." << endl;
    }
    else
    {
        cout << "Hello is not on the blacklist." << endl;
    }
    
    return 0;
}
