#pragma once
#include "settings.h"

class State;

namespace Config
{
	/**A class to contain various camera settings.*/
	class ServerSettings : public Settings
	{

	public:
		/**Initialises the variables from file.*/
		void initialise() override
		{
			maxChannels = initialiseVar("maxChannels", 50);
			maxChannelMembers = initialiseVar("maxChannelMembers", 50);
			port = initialiseVar("port", 59534);
		}

		int maxChannels;
		int maxChannelMembers;
		int port;

	};

}