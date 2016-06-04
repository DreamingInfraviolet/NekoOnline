#include <stdexcept>
#include <string>
#include <iostream>
#include <atomic>
#define ASIO_MSVC 1
#define ASIO_STANDALONE 1
#include <asio.hpp>
#include "settings.h"
#include "server_settings.h"

//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

using asio::ip::tcp;
class chat_message;


//----------------------------------------------------------------------

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class channel
{
public:
	void join(chat_participant_ptr participant)
	{
		participants_.insert(participant);
		for (auto msg : recent_msgs_)
			participant->deliver(msg);
	}

	void leave(chat_participant_ptr participant)
	{
		participants_.erase(participant);
	}

	void deliver(const chat_message& msg)
	{
		recent_msgs_.push_back(msg);
		while (recent_msgs_.size() > max_recent_msgs)
			recent_msgs_.pop_front();

		for (auto participant : participants_)
			participant->deliver(msg);
	}

private:
	std::set<chat_participant_ptr> participants_;
	enum { max_recent_msgs = 100 };
	chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class user_session
	: public chat_participant,
	public std::enable_shared_from_this<user_session>
{
public:
	user_session(tcp::socket socket, channel& channel)
		: socket_(std::move(socket)),
		channel_(channel)
	{
	}

	void start()
	{
		channel_.join(shared_from_this());
		do_read_header();
	}

	void deliver(const chat_message& msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			do_write();
		}
	}

private:
	void do_read_header()
	{
		auto self(shared_from_this());
		asio::async_read(socket_,
			asio::buffer(read_msg_.data(), chat_message::header_length),
			[this, self](asio::error_code ec, std::size_t /*length*/)
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();
			}
			else
			{
				channel_.leave(shared_from_this());
			}
		});
	}

	void do_read_body()
	{
		auto self(shared_from_this());
		asio::async_read(socket_,
			asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](asio::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				channel_.deliver(read_msg_);
				do_read_header();
			}
			else
			{
				channel_.leave(shared_from_this());
			}
		});
	}

	void do_write()
	{
		auto self(shared_from_this());
		asio::async_write(socket_,
			asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
			[this, self](asio::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				std::cerr << "Failed reading data from client";
				channel_.leave(shared_from_this());
			}
		});
	}

	tcp::socket socket_;
	channel& channel_;
};

//----------------------------------------------------------------------

class Server
{
public:
	Server(asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: acceptor_(io_service, endpoint),
		socket_(io_service)
	{
		accept();
	}

private:
	void accept()
	{
		acceptor_.async_accept(socket_,
			[this](asio::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<user_session>(std::move(socket_), room_)->start();
			}
			else
			{
				std::cerr << "Failed accepting connection: " << ec.message() << "\n";
			}

			std::shared_ptr<int> ptr;

			accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
	channel room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	FileSystem::IniParser iniParser;
	Config::ServerSettings settings;

	try
	{
		if (!iniParser.parseFile("config.ini"))
			throw std::runtime_error("Failed parsing config.ini");

		if (!settings.attachParser(&iniParser, "GLOBAL"))
			throw std::runtime_error("Failed attaching parser.");
		settings.initialise();

		asio::io_service io_service;

		tcp::endpoint endpoint(tcp::v4(), settings.port);

		Server server(io_service, endpoint);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}


/*
#include "server_settings.h"
#include "channel_manager.h"

std::atomic<bool> running = true;
std::atomic<bool> errorFlag = false;


int main(int argc, char** argv)
{
	using asio::ip::tcp;
	enum networkMessage { MSG_CHANNEL, MSG_DISCONNECT, MSG_REPEAT };

	std::cout << "Running neko server...\n";
	asio::io_service service;
	std::unique_ptr<ChannelManager> manager;
	std::unique_ptr<tcp::acceptor> acceptor;
	FileSystem::IniParser iniParser;
	Config::ServerSettings settings;

	try
	{
		if (!iniParser.parseFile("config.ini"))
			throw std::runtime_error("Failed parsing config.ini");

		if (!settings.attachParser(&iniParser, "GLOBAL"))
			throw std::runtime_error("Failed attaching parser.");
		settings.initialise();

		manager = std::unique_ptr<ChannelManager>(new ChannelManager(settings.maxChannels, settings.maxChannelMembers));
		acceptor = std::unique_ptr<tcp::acceptor>(new tcp::acceptor(service, tcp::endpoint(tcp::v4(), settings.port)));
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << "ERROR initialising: " << err.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "UNKNOWN ERROR INITIALISING" << std::endl;
		return 2;
	}

	while (running)
	{
		try
		{
			tcp::socket socket(service);
			acceptor->accept(socket);
			std::cout << "Accepting new connection\n";
			manager->handleNewConnection(std::move(socket));
		}
		catch (const std::runtime_error& err)
		{
			std::cerr << "ERROR in main loop: " << err.what() << std::endl;
			return 3;	
		}
		catch (...)
		{
			std::cerr << "UNKNOWN ERROR IN MAIN LOOP" << std::endl;
			return 4;
		}
	}
	
	if (errorFlag)
	{
		std::cout << "Attempting graceful shutdown\n";
		return 5;
	}
	else
	{
		std::cout << "Gracefully shutting down neko server\n";
		return 0;
	}
}
*/