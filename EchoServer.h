#ifndef ECHOSERVER__ECHOSERVER_H_
#define ECHOSERVER__ECHOSERVER_H_

class EchoServer
{
 private:
	bool loop;

 public:
	void init();
	int run();
	void MessageWaitThred(int socket);
	void SetEnd();
};

#endif //ECHOSERVER__ECHOSERVER_H_
