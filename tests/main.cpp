#include "../include/SHMFalcon.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;
int main() {
    int worked = SHMFalcon::startListener("Here is a key");
    for (int i = 0; i < 100; i++){
        cout << "|" << i << endl;
        sleep(1);
    }
    //SHMFalcon::stopListener();
    return 0;
}