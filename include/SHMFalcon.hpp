#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
namespace SHMFalcon {

    int startListener(std::string key);

    void stopListener();

    boost::interprocess::shared_memory_object* SharedMemory; 
    
    
    struct shared_data {
    private:
        std::array<double, 9> SomeData;
        mutable boost::interprocess::interprocess_mutex mutex;

    public:
        bool keepGoing = true;
        shared_data() {
        }


        std::array<double, 9> readData(){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            
            return this->SomeData;
        }

        void writeData(std::array<double, 9> data){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            this->SomeData = data;
        }
    };
}

