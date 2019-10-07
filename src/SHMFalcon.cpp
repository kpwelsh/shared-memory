#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <array>

using namespace std;

using namespace boost::interprocess;
namespace SHMFalcon {

    shared_memory_object* SharedMemory;

    struct shared_data {
    private:
        mutable interprocess_mutex mutex;
        array<double, 9> SomeData;

    public:
        bool keepGoing = true;

        array<double, 9> readData(){
            this->mutex.lock();
            // Pretty sure this copies the array, and not the address.
            array<double, 9> r = this->SomeData;
            this->mutex.unlock();
            return r;
        }

        void writeData(array<double, 9> data){
            this->mutex.lock();
            this->SomeData = data;
            this->mutex.unlock();
        }
    };

    namespace {
        bool init_falcon() {
            //Put that init code here.
        }

        void pollFalcon() {
            //Put the polling here.
        }

        void listen(){
            mapped_region region(*SharedMemory, read_write);
            shared_data* sharedData = (shared_data*)region.get_address();

            while(sharedData->keepGoing) {
                pollFalcon();
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
            // We are the child process
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