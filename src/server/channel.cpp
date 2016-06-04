#include "channel.h"
#include "channel_manager.h"
#include <iostream>
#include "network_codes.h"

Channel::Channel(ChannelManager& parent, int channel)
	: parent(parent), channel(channel), thread(channelFunction, this, parent.maxChannelMembers)
{

}

Channel::Channel(Channel&& c)
	: parent(c.parent), channel(c.channel), connections(std::move(c.connections)), thread(std::move(c.thread)),
	channelIsRunning(c.channelIsRunning)
{

}

Channel::~Channel()
{
	channelIsRunning = false;
	if (thread.joinable())
		thread.join();
}

void Channel::channelFunction(Channel* channel, int maxChannelMembers)
{
	while (channel->channelIsRunning)
	{
		try
		{
			for (auto& connection : channel->connections)
				asio::write(connection, asio::buffer("test!"));
		}
		catch (const std::runtime_error& err)
		{
			std::cerr << "Error in channel " << channel->channel << ": " << err.what() << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown error in channel " << channel->channel << ": " << std::endl;
		}
	}
}

void Channel::addConnection(asio::ip::tcp::socket&& s)
{
	connections.emplace_back(std::move(s));
	asio::write(s, asio::buffer(std::vector<int>(1, htonl(NET_ACCEPT_CONNECTION))));
}