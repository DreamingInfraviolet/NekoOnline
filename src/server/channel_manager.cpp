#include "channel_manager.h"
#include <iostream>
#include "network_codes.h"
#include "channel.h"

ChannelManager::ChannelManager(int maxChannels, int maxChannelMembers)
	: maxChannelMembers(maxChannelMembers), currentChannels(0) {}

ChannelManager::~ChannelManager()
{
}

void ChannelManager::registerShutdown(int channelId)
{
	std::lock_guard<std::mutex> guard(mxRegisterShutdown);
	channels.erase(channelId);
}

/** Takes over the handling of a connection. */
void ChannelManager::handleNewConnection(asio::ip::tcp::socket&& s)
{
	try
	{
		//Get desired channel id
		std::vector<char> stream(8);
		read(s, asio::buffer(stream));
		if (channels.size() == maxChannelMembers || ntohl(*(unsigned*)&stream[0]) != NET_EXPECT_CHANNEL)
		{
			refuseConnection(s);
			return;
		}

		int channel = ntohl(*((unsigned*)&stream[0] + 4));
		auto itChannel = channels.find(channel);
		if (itChannel == channels.end())
		{
			channels.insert(std::make_pair(channel, Channel(*this, channel)));
			itChannel = channels.find(channel);
		}
		
		itChannel->second.addConnection(std::move(s));
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << "Error handling connection: " << err.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown error handling connection" << std::endl;
	}
}

void ChannelManager::refuseConnection(asio::ip::tcp::socket& s)
{
	try
	{
		std::cout << "Refusing connection\n";
		asio::write(s, asio::buffer(std::vector<int>(1, htonl(NET_REJECT_CONNECTION))));
		s.shutdown(asio::socket_base::shutdown_type::shutdown_both);
		s.close();
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << "Error refusing connection: " << err.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown error refusing connection" << std::endl;
	}
}