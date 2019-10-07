#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <array>

namespace SHMFalcon {

    bool startListener(std::string key);

    void stopListener();

    boost::interprocess::shared_memory_object* SharedMemory; 
    
    struct shared_data {
        std::array<double, 9> readData();
        bool keepGoing;
        void writeData(std::array<double, 9> data);
    };
}

