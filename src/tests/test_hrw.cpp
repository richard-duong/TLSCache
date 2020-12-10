#include "../components/HRW.cpp"
#include <iostream>
using namespace std;


int main(){
    
    char object[] = "object80sdfffffffffffffffff0";
    char result[strlen(object) +6];
    
    
    cout << "Object will go to " << HRW(object) << endl;

    
    return 0;
}
