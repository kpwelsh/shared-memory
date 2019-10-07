#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <array>
#include "SHMFalcon.hpp"

using namespace std;
using namespace boost::interprocess;

namespace SHMFalcon {
    namespace {
        bool init_falcon() {
            //Put that init code here.
        }

        void pollFalcon() {
            //Put the polling here.
        }

        void feedbackFalcon(array<double, 3> EEForces) {
            // Give some force feedback.
        }

        void listen(){
            mapped_region region(*SharedMemory, read_write);
            shared_data* sharedData = (shared_data*)region.get_address();
            while(sharedData->keepGoing) {
                pollFalcon();
                feedbackFalcon(sharedData->readEEForces());
            }
            remove(SharedMemory->get_name());
        }
    }

    int startListener(string key) {
        SharedMemory = new shared_memory_object(open_or_create, key.c_str(), read_write);
        SharedMemory->truncate(sizeof(shared_data));
        mapped_region region(*SharedMemory, read_write);
        new (region.get_address()) shared_data;

        pid_t pid = fork();
        if (pid == 0) {
            listen();
            exit(0);
        }
        return pid;
    }

    void stopListener() {
        mapped_region region(*SharedMemory, read_write);
        ((shared_data*)region.get_address())->keepGoing = false;
    }
}