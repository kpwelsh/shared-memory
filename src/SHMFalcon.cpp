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
#include "falcon/core/FalconDevice.h"
#include "falcon/firmware/FalconFirmwareNovintSDK.h"
#include "falcon/util/FalconCLIBase.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"
#include "falcon/kinematic/stamper/StamperUtils.h"
#include "falcon/kinematic/FalconKinematicStamper.h"
#include "falcon/core/FalconGeometry.h"
#include "falcon/gmtl/gmtl.h"
#include "falcon/util/FalconFirmwareBinaryNvent.h"

using namespace libnifalcon;
using namespace std;
using namespace boost::interprocess;
using namespace StamperKinematicImpl;
namespace SHMFalcon {
    namespace {
	FalconKinematic* falconKinematic;
	std::shared_ptr<FalconFirmware> falconFirmware;
	FalconDevice m_falconDevice;
        bool init_falcon() {
          cout <<"Setting up LibUSB\n";
	  m_falconDevice.setFalconFirmware<FalconFirmwareNovintSDK>(); //Set Firmware

	  if(!m_falconDevice.open(0)) //Open falcon @ index
	  {
	    cout << "Failed to find Falcon\n";
	    return false;
	  }
	  else
	  {
	    cout << "Falcon Found\n";
	  }
	  //There's only one kind of firmware right now, so automatically set that.
	  //m_falconDevice.setFalconFirmware<FalconFirmwareNovintSDK>();
	  //Next load the firmware to the device

	  bool skip_checksum = false;
	  //See if we have firmware
	  bool firmware_loaded = m_falconDevice.isFirmwareLoaded();
          if(!firmware_loaded)
	  {
	    cout << "Loading firmware\n";
	    uint8_t* firmware_block;
	    long firmware_size;
	    {
	      firmware_block = const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE);
	      firmware_size = NOVINT_FALCON_NVENT_FIRMWARE_SIZE;

	      for(int i = 0; i < 50; ++i)       //Attempt to load firmware 50 times
	      {
	        if(!m_falconDevice.getFalconFirmware()->loadFirmware(skip_checksum, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE)))
	        {
	          cout << "Firmware loading try failed\n";
	        }
	        else
	        {
	          firmware_loaded = true;
	          break;
	        }
	      }
	    }
	  }

	  else if(!firmware_loaded)
	  {
	    cout << "No firmware loaded to device, and no firmware specified to load (--nvent_firmware, --test_firmware, etc...). Cannot continue\n";
	    return false;
	  }

	  if(!firmware_loaded || !m_falconDevice.isFirmwareLoaded())
	  {
	    cout << "No firmware loaded to device, cannot continue\n";
	    return false;
	  }
	  m_falconDevice.getFalconFirmware()->setHomingMode(true); //Set homing mode (keep track of encoders !needed!)
	  cout << "Homing Set\n";
	  m_falconDevice.runIOLoop(); //read in data

	  bool stop = false;
	  bool homing = false;
	  bool homing_reset = false;
	  while(!stop)
	  {
	    if(!m_falconDevice.runIOLoop()) continue;
	    if(!m_falconDevice.getFalconFirmware()->isHomed())
	    {
	      if(!homing)
	      {
	        m_falconDevice.getFalconFirmware()->setLEDStatus(libnifalcon::FalconFirmware::RED_LED);
	        cout << "Falcon not currently homed. Move control all the way out then push straight all the way in.\n";
	      }
	      homing = true;
	    }

	    if(homing && m_falconDevice.getFalconFirmware()->isHomed())
		stop = true;
          }
	  m_falconDevice.runIOLoop();
	  falconFirmware = m_falconDevice.getFalconFirmware();
          falconKinematic = new FalconKinematicStamper(true);
	  return true;
        }

        void pollFalcon() {
	    array<double, 3> pos = {0,0,0};
	    falconFirmware->runIOLoop();
	    std::array<int, 3> encd = falconFirmware->getEncoderValues();
            falconKinematic->getPosition(encd, pos);
	    mapped_region region(*SharedMemory, read_write);
	    shared_data* sharedData = (shared_data*)region.get_address();
	    sharedData->writePositions(pos);
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
	if (!init_falcon()){
	    return -1;
	}
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
