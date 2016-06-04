#pragma once
#include <map>
#include <mutex>
#define ASIO_MSVC 1
#define ASIO_STANDALONE 1
#include <asio.hpp>
class Channel;

class ChannelManager
{
	//Map of id to thread
	std::map<int, Channel> channels;
	int maxChannelMembers;
	int currentChannels;

	friend class Channel;

	std::mutex mxRegisterShutdown;
public:
	ChannelManager(int maxChannels, int maxChannelMembers);

	~ChannelManager();

	void registerShutdown(int channelId);

	/** Takes over the handling of a connection. */
	void handleNewConnection(asio::ip::tcp::socket&& s);

private:
	void refuseConnection(asio::ip::tcp::socket& s);
};