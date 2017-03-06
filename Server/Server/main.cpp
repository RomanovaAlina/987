#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h> 
#include <iostream> 
#include <WS2tcpip.h> 

SOCKET Connect; 
SOCKET * Connections; 
SOCKET Listen; 

int ClientCount=0; 


void sendmessageclient(int ID)
{
	char*buffer=new char[1024];
	memset(buffer,0,sizeof(buffer)); 
	if (recv(Connections[ID],buffer,1024,NULL)) 
	{
		printf(buffer); 
		printf("\n");
		for (int i=0; i<=ClientCount; i++) 
		{
			send(Connections[i],buffer,strlen(buffer),NULL); 
		}
	}
	delete buffer;
}

int main()
{
	setlocale(LC_ALL, "russian"); 
	WSAData data;
	WORD version=MAKEWORD(2,2);
	int res = WSAStartup(version, &data); 
	if (res!=0)
	{
		return 0;
	}

	struct addrinfo socketinfo;
	struct addrinfo * result;

	Connections = (SOCKET*)calloc(64,sizeof(SOCKET)); 

	ZeroMemory(&socketinfo,sizeof(socketinfo));

	
	socketinfo.ai_family=AF_INET; 
	socketinfo.ai_flags=AI_PASSIVE; 
	socketinfo.ai_socktype=SOCK_STREAM;
	socketinfo.ai_protocol=IPPROTO_TCP;

	
	getaddrinfo(NULL,"7770",&socketinfo,&result);
	
	
	Listen=socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	bind(Listen,result->ai_addr, result->ai_addrlen); 
	listen(Listen,SOMAXCONN); 

	freeaddrinfo(result);

	printf("Starting server...");
	char message[]="Connection...##";
	if (Connect=accept(Listen,NULL,NULL))
	{
		printf("Client connected\n");
		Connections[ClientCount]=Connect; 
		send(Connections[ClientCount],message,strlen(message),NULL);
		ClientCount++;
		CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)sendmessageclient,(LPVOID)(ClientCount-1),NULL,NULL); //создаем поток, запускаем в потоке функцию SendMessageToClient, передаем ей Id 
	}
	return 1;
}
