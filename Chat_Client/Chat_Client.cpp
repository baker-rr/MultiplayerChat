#include<winsock2.h> // winsock2的头文件
#include<iostream>
using  namespace std;

//链接dll的lib
#pragma comment(lib, "ws2_32.lib")

int  main()
{

	// 加载winsock2的环境
	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup  error：" << GetLastError() << endl;
		return 0;
	}

	// 1.创建流式套接字
	SOCKET  sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sSocket == INVALID_SOCKET)
	{
		cout << "socket  error：" << GetLastError() << endl;
		return 0;
	}

	// 2.链接服务器
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	int len = sizeof(sockaddr_in);
	if (connect(sSocket, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "connect  error：" << GetLastError() << endl;
		return 0;
	}

	// 3.接收服务端的消息
	char buf[100] = { 0 };
	recv(sSocket, buf, 100, 0);
	cout << buf << endl;

	// 4.随时给服务端发消息
	int  ret = 0;
	do
	{
		char buf[100];
		cout << "请输入聊天内容:";
		cin >> buf;
		ret = send(sSocket, buf, 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0);


	// 5.关闭监听套接字
	closesocket(sSocket);

	// 清理winsock2的环境
	WSACleanup();

	return 0;
}
