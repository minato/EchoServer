#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include "EchoServer.h"

void EchoServer::init()
{
	loop = true;
	return;
}

int EchoServer::run()
{
	int waitSocket;
	struct sockaddr_in addr;
	struct sockaddr_in client;

	// ソケット作成
	waitSocket = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(waitSocket, (struct sockaddr *)&addr, sizeof(addr));
	listen(waitSocket, 1);

	fd_set fds, readfds;
	FD_ZERO(&readfds);
	FD_SET(waitSocket, &readfds);
    int maxfd = waitSocket;

	struct timeval  tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	// 接続待ち受け
	while (loop){
		memcpy(&fds, &readfds, sizeof(fd_set));
		int ret = select(maxfd + 1, &fds, NULL, NULL, &tv);
		if(ret == 0){
			//　タイムアウト
			continue;
		}

		if(ret < 0){
			// シグナル受信、シグナルハンドラ側に任せているので処理続行
			continue;
		}

		// コネクション待ち
		if(FD_ISSET(waitSocket, &fds))
		{
			int len = sizeof(client);
			int sock = accept(waitSocket, (struct sockaddr*)&client, &len);
			if (sock == -1)
			{
				std::cerr << "接続エラー" << std::endl;
				continue;
			}
			std::thread th(&EchoServer::MessageWaitThred, this, sock);

			th.detach();
		}
	}

	//　終了処理
	close(waitSocket);
}

void EchoServer::MessageWaitThred(int socket)
{
	char buf[32];
	int bufsize = sizeof(buf);

	// 受信処理
	fd_set fds, readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);
	int maxfd = socket;
	struct timeval  tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	while (loop)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));
		int ret = select(maxfd + 1, &fds, NULL, NULL, &tv);
		if(ret == 0){
			//　タイムアウト
			continue;
		}

		if(ret < 0){
			// シグナル受信、シグナルハンドラ側に任せているので処理続行
			continue;
		}

		memset(buf, 0, bufsize);
		int recv_size = recv(socket, buf, bufsize, 0);
		if(recv_size == 0)
		{
			// コネクション切断
			break;
		}

		// 受信データ処理
		int send_size = send(socket, buf, bufsize, 0);
		if(send_size == -1)
		{
			// コネクション切断
			break;
		}
	}
	// 接続の解除
	close(socket);
}

void EchoServer::SetEnd()
{
	loop = false;
}