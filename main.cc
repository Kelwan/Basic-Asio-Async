
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <array>
#include <chrono>

#include "boost/asio.hpp"
#include "boost/array.hpp"
#include "boost/bind/bind.hpp"
#include "boost/shared_ptr.hpp"

using boost::asio::ip::udp;

std::string make_daytime_string() {
	std::time_t now = time(0);
	return std::ctime(&now);
}

class udp_server {
public:
	udp_server(boost::asio::io_context& io_context)
		: socket_(io_context, udp::endpoint(udp::v4(), 13))
	{
		start_receive();
	}

private:
	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer), remote_endpoint
			, [&](auto ec, std::size_t bytes) {
				handle_receive(ec, bytes);
			}
		);
	}

	void handle_receive(
		const boost::system::error_code& error
		, std::size_t)
	{
		if(error) {
			std::cout << "async_receive_from_error: " << error.message() << std::endl;
		} else if (!error) {
			if(!recv_buffer.empty()) {
				std::cout << recv_buffer.data() << std::endl;
				start_receive();
			}	
		}
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint;
	std::array<char, 128> recv_buffer;

};

class udp_send_socket {
public:
	udp_send_socket(boost::asio::io_context& io_context)
	: socket_(io_context, udp::v4())
	{
		async_send_to();
	}

private:
	udp::socket socket_;
	std::string test = "hello world!";

public:
	void async_send_to() {
		socket_.async_send_to(boost::asio::buffer(test)
		, udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 13) 
		, [] (const auto error, std::size_t) {
			if(error) {
				std::cout << "async_send_to error: " << error.message() << std::endl;
			}
		});
	}
};

int main() {

	try {
		boost::asio::io_context io_context;
		udp_server server(io_context);
		udp_send_socket send_socket(io_context);

		std::thread io_context_loop([&] {
			for(;;) {
				io_context.run_one();
			}
		});

		std::thread hello_world_10([&] {
			using namespace std::chrono_literals;
			for(int i = 0; i < 10; i++) {
				send_socket.async_send_to();
				std::this_thread::sleep_for(1s);
			}
		});

		if(io_context_loop.joinable()) {
			io_context_loop.join();
		}
		if(hello_world_10.joinable()) {
			hello_world_10.join();
		}
	}

	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}