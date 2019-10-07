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
        double SomeData[9];

    public:
        mutable boost::interprocess::interprocess_mutex mutex;
        shared_data() {
        }

        bool keepGoing = true;

        double* readData(){
            std::cout << "there" << std::endl;
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            double* r = new double[9]{0,0,0,0,0,0,0,0,0};
            for(int i = 0; i < 9; i++) {
                r[i] = this->SomeData[i];
            }
            return r;
        }

        void writeData(double* data){
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mutex);
            for(int i = 0; i < 9; i++) {
                this->SomeData[i] = data[i];
            }
        }
    };
}

