#pragma once
#include "settings.h"

class State;

namespace Config
{
    /**A class to contain various camera settings.*/
    class ClientSettings : public Settings
    {

    public:
		/**Initialises the variables from file.*/
		void initialise() override
		{
			imageSource = initialiseVar("imageSource", std::string("pet.png"));
			serverIp = initialiseVar("serverIp", std::string("127.0.0.1"));
			serverPort = initialiseVar("serverPort", 59534);
		}
        friend class State;

		std::string imageSource;
		std::string serverIp;
		int serverPort;

    };
  
}