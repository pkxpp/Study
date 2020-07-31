////  Hello World server
//
//#include <zmq.h>
//#include <stdio.h>
////#include <unistd.h>
//#include <string.h>
//#include <assert.h>
//#include <windows.h>
//
//int main(void)
//{
//    //  Socket to talk to clients
//    void* context = zmq_ctx_new();
//    void* responder = zmq_socket(context, ZMQ_REP);
//    int rc = zmq_bind(responder, "tcp://*:5555");
//    assert(rc == 0);
//
//    while (1) {
//        char buffer[10];
//        zmq_recv(responder, buffer, 10, 0);
//        printf("Received Hello\n");
//        //sleep(1);          //  Do some 'work'
//        Sleep(1000);
//        zmq_send(responder, "World", 5, 0);
//    }
//    return 0;
//}

//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//

//////////////////////////////////////////////////////////////////////////
// 下面用的c++封装的头文件zmq.hpp
#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif

int main() {
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv(request);
        std::cout << "Received Hello" << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        zmq::message_t reply(5);
        memcpy(reply.data(), "World", 5);
        socket.send(reply, static_cast <zmq::send_flags>(0));
    }
    return 0;
}