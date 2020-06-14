#include <iostream>

#include "EchoServer.h"

void sig_handler(int sig);

EchoServer *p_server = nullptr;
int main()
{
	std::cout << "START!!" << std::endl;

	if ( signal(SIGTERM, sig_handler) == SIG_ERR ) {
		exit(1);
	}

	p_server = new EchoServer();
	p_server->init();
	p_server->run();
	delete p_server;

	std::cout << "END!!" << std::endl;
	return 0;
}

void sig_handler(int sig)
{
	std::cerr << "SIG:" << sig << std::endl;
	if(p_server == nullptr){
		return;
	}

	p_server->SetEnd();
}