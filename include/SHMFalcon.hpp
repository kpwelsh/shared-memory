#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstdlib>
#include <cstring>

namespace SHMFalcon {

    bool startListener(std::string key);

    void stopListener();
}

