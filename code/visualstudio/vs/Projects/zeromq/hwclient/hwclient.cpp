//  Hello World client
//#include <zmq.h>
//#include <string.h>
//#include <stdio.h>
////#include <unistd.h>
//#include <windows.h>
//
//int main(void)
//{
//	printf("Connecting to hello world server…\n");
//	void* context = zmq_ctx_new();
//	void* requester = zmq_socket(context, ZMQ_REQ);
//	zmq_connect(requester, "tcp://localhost:5555");
//
//	int request_nbr =0;
//	//for (request_nbr = 0; request_nbr != 10; request_nbr++) {
//	while(true){
//		char buffer[10];
//		printf("Sending Hello %d…\n", request_nbr);
//		zmq_send(requester, "Hello", 5, 0);
//		// 这里是不是阻塞的？看了打印应该试阻塞的，因为试收到服务器的world，才发送的下一个Sending Hello ...
//		zmq_recv(requester, buffer, 10, 0);
//		printf("Received World %d\n", request_nbr);
//		++request_nbr;
//	}
//	zmq_close(requester);
//	zmq_ctx_destroy(context);
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
// 下面用的c++封装的zmq.hpp
//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);

	std::cout << "Connecting to hello world server…" << std::endl;
	socket.connect("tcp://localhost:5555");

	//  Do 10 requests, waiting each time for a response
	for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		zmq::message_t request(5);
		memcpy(request.data(), "Hello", 5);
		std::cout << "Sending Hello " << request_nbr << "…" << std::endl;
		socket.send(request, static_cast <zmq::send_flags>(0));

		//  Get the reply.
		zmq::message_t reply;
		socket.recv(reply);
		std::cout << "Received World " << request_nbr << std::endl;
	}
	return 0;
}