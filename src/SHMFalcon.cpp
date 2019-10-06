#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sstream>

using namespace std;

namespace SHMFalcon {
    using namespace boost::interprocess;
    string STOP_LISTENING = "FALCON_STOP"; 
    namespace {
        string memoryKey;

        mapped_region stopListeningFlag; 

        void listen(){
            fprintf(stderr, "Hello");
            int v = *(int*)stopListeningFlag.get_address();
            std::stringstream strs;
            strs << v;
            fprintf(stderr, ("Vluae: " + strs.str()).c_str());
            
            while(*((int *)stopListeningFlag.get_address()) == 1) {
                cout << "Listening" << endl;
                sleep(0.1);
            }

            fflush(stdout);
            
            remove(STOP_LISTENING.c_str());
        }
    }

    int startListener(string key) {
        memoryKey = key;

        pid_t pid = fork();
        
        //Create a shared memory object.
        shared_memory_object* shm = new shared_memory_object(open_or_create, STOP_LISTENING.c_str(), read_write);
        shm->truncate(1);
        mapped_region stopListeningFlag(*shm, read_write);
        memset(stopListeningFlag.get_address(), 1, stopListeningFlag.get_size());
        
            int v = *(int*)stopListeningFlag.get_address();
            std::stringstream strs;
            strs << v;
            fprintf(stderr, ("Vluae: " + strs.str()).c_str());
        if (pid == 0){
            printf("This is child\n");
            listen();
        } else if (pid > 0) {
            printf("This is parent");
            return pid;
        }
        return pid;
    }


    void stopListener() {
        memset(stopListeningFlag.get_address(), 0, stopListeningFlag.get_size());
    }
}