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
        std::array<double, 3> FalconPositions;
        std::array<double, 3> EEForces;
        mutable boost::interprocess::interprocess_mutex mutex;

    public:
        bool keepGoing = true;
        shared_data() {
        }

        std::array<double, 3> readPositions(){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            return this->FalconPositions;
        }

        void writePositions(std::array<double, 3> data){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            this->FalconPositions = data;
        }

        std::array<double, 3> readEEForces(){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            return this->EEForces;
        }
        
        void writeEEForces(std::array<double, 3> data){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            this->EEForces = data;
        }

    };
}

