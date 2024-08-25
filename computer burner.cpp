#include <stdc++.h>
#include "winsock2.h"
#include <Ws2tcpip.h>
#include <windows.h>
#include <random>
#include <functional>
#include <ctype.h>
#include <fstream>
#include<thread>
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

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		//cout<<"...";
		return;
	}

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

	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) {
		//printf("杩炴帴澶辫触:%d", WSAGetLastError());
		return;
	}
	else {
		recv(sockClient, buff, sizeof(buff), 0);
	}

	char buffs[10000] = "nothing";
	send(sockClient, buffs, sizeof(buffs), 0);

	for (int i = 0; i < 10000; i++)
		buffs[i] = 'c';

	while(1){
		send(sockClient, buffs, sizeof(buffs), 0);
	}
}bool IsNotPrint(char c)
{
	return !isprint(c) && c != 0;
}
signed main(int argc, char* argv[]) {
	//if (argc != 3)cout << "error thread number";
	//numthread = atoi(argv[1]);
	//Sleep(1000);
	if (argc > 6)goto startt;
	
	if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1] , "--help") == 0)) {
		cout << "usage:\ncomputer-burner.exe [--thread/-t <num>] [--wait/-w <num>] [--help/-h]\n";
		cout << "--thread or -t <num> : create <num> treads, defult 400\n";
		cout << "--wait or -w <num> : wait <num>ms before start defult 0\n";
		cout << "--help or -h : show this help message\n";
		cout << "for more imformation ,visit github: https://github.com/DDEEII/computer-burner\n";
		return 0;
	}if (argc >= 3) {
		if (strcmp(argv[1] ,"--tread")==0 || strcmp(argv[1] , "-t") == 0) {
			for (int i = 1; i < strlen(argv[2]); i++) {
				if (!isdigit(argv[2][i])) {
					cout << "error thread number,using defult number:400\n";
					goto startt;
				}
			}numthread = atoi(argv[2]);
			cout << "using thread number:"<<argv[2]<<endl;
		}else if (strcmp(argv[1], "--wait") == 0 || strcmp(argv[1] , "-w") == 0) {
			for (int i = 0; i < strlen(argv[2]); i++) {
				if (!isdigit(argv[2][i])) {
					cout << "error waiting time,start without waiting\n";
					goto startt;
				}
			}wait = atoi(argv[2]);
			cout << "wait " << argv[2] <<"ms before starting"<< endl;
		}if (argc == 5) {
			if (strcmp(argv[3], "--tread") == 0 || strcmp(argv[3] , "-t") == 0) {
				for (int i = 0; i < strlen(argv[4]); i++) {
					if (!isdigit(argv[4][i])) {
						cout << "error thread number,using defult number:400\n";
						goto startt;
					}
				}numthread = atoi(argv[4]);
				cout << "using thread number:" << argv[4] << endl;
			}if (strcmp(argv[3], "--wait") == 0 || strcmp(argv[3], "-w") == 0) {
				for (int i = 0; i < strlen(argv[4]); i++) {
					if (!isdigit(argv[4][i])) {
						cout << "error waiting time,start without waiting\n";
						goto startt;
					}
				}wait = atoi(argv[4]);
				cout << "wait " << argv[4] << "ms before starting" << endl;
			}
		}
	}if (wait < 0 || numthread < 0||argc == 1 || argc == 2) cout << "using defult: 400 threads,start without waiting";
	
startt:
	system("pause");
	for (int i = 0; i < numthread; i++) {
		mythread[i] = thread(show, i);
		ctcp[i] = thread(tcp, i);
		//Sleep(50);
	}

	for (int i = 0; i < numthread; i++) {
		mythread[i].join();
		ctcp[i].join();
	}

	return 0;
}
