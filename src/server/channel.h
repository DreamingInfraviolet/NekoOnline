#pragma once
#include <vector>
#include <thread>
#define ASIO_MSVC 1
#define ASIO_STANDALONE 1
#include <asio.hpp>

class ChannelManager;
class Channel
{
	ChannelManager& parent;
	int channel = -1;
	std::vector <asio::ip::tcp::socket> connections;
	std::thread thread;
	bool channelIsRunning;

public:

	Channel(ChannelManager& parent, int channel);
	Channel(const Channel&) = delete;
	Channel(Channel&&);

	~Channel();

	static void channelFunction(Channel* channel, int maxChannelMembers);

	void addConnection(asio::ip::tcp::socket&& s);
};