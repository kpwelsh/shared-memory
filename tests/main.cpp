#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <stdio.h>
#include <iostream>
#include "../include/SHMFalcon.hpp"

using namespace std;
using namespace boost::interprocess;
int main() {
    int pid = SHMFalcon::startListener("Here is a key");
    cout << "Listening with PID " << pid << endl;
    sleep(1);
    mapped_region region(*SHMFalcon::SharedMemory, read_only);
    SHMFalcon::shared_data* sharedData = (SHMFalcon::shared_data*)region.get_address();
    array<double,9> data = sharedData->readData();
    cout << "From parent: " << data[0] << endl;;
    SHMFalcon::stopListener();
    return 0;
}