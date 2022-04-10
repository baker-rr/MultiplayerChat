
#include <winsock2.h> 
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

// stdcall的线程处理函数
DWORD WINAPI ThreadFun(LPVOID lpThreadParameter);

int main()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "WSAStartup Error:" << WSAGetLastError() << endl;
		return 0;
	}

	// 1. 创建流式套接字
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sSocket == INVALID_SOCKET)
	{
		cout << "socket error:" << WSAGetLastError() << endl;
		return 0;
	}

	// 2. 绑定端口和ip
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	int len = sizeof(sockaddr_in);
	if (bind(sSocket, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "bind Error:" << WSAGetLastError() << endl;
		return 0;
	}

	// 3. 监听
	listen(sSocket, 5);

	// 主线程循环接收客户端的连接
	while (true)
	{
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		// 4.接受成功返回与client通讯的Socket
		SOCKET cSocket = accept(sSocket, (SOCKADDR*)&addrClient, &len);
		if (cSocket != INVALID_SOCKET)
		{
			// 创建线程，并且传入与client通讯的套接字
			HANDLE hThread = CreateThread(NULL, 0, ThreadFun, (LPVOID)cSocket, 0, NULL);
			CloseHandle(hThread); // 关闭对线程的引用
		}

	}

	// 6.关闭监听套接字
	closesocket(sSocket);

	// 清理winsock2的环境
	WSACleanup();

	return 0;
}

DWORD WINAPI ThreadFun(LPVOID lpThreadParameter)
{
	// 5.与客户端通讯，发送或者接受数据
	SOCKET cSocket = (SOCKET)lpThreadParameter;

	cout << "欢迎" << cSocket << "进入聊天室！" << endl;

	// 发送数据
	char buf[100] = { 0 };
	sprintf(buf, "欢迎 %d 进入聊天室！", cSocket);
	send(cSocket, buf, 100, 0);

	// 循环接收客户端数据
	int ret = 0;
	do
	{
		char buf2[100] = { 0 };
		ret = recv(cSocket, buf2, 100, 0);

		cout << cSocket << " 说：" << buf2 << endl;

	} while (ret != SOCKET_ERROR && ret != 0);

	cout << cSocket << "离开了聊天室！";

	return 0;
}
