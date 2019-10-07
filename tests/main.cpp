#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
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
    sharedData->mutex.try_lock();
    SHMFalcon::stopListener();
    return 0;
}