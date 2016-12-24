#include "ODSocket.h"
#include<stdio.h>

//#ifdef WIN32
//#include "StdAfx.h"
//#pragma comment(lib, "wsock32")
//#endif


ODSocket::ODSocket(SOCKET sock)
{
	m_sock = sock;
}

ODSocket::~ODSocket()
{
	Close();
	Clean();
}

//void ODSocket::ResetFDSet(fd_set & fdRead, fd_set & fdWrite, fd_set & fdExcept, SOCKET sdListen, const ConnectionList & conns)
//{
//	FD_ZERO(&fdRead);
//	FD_ZERO(&fdWrite);
//	FD_ZERO(&fdExcept);
//	FD_SET(sdListen, &fdRead);
//	FD_SET(sdListen, &fdExcept);
//	ConnectionList::const_iterator it = conns.begin();
//	for (; it != conns.end(); ++it)
//	{
//		Connection* pConn = *it;
//		if (pConn->bytes < BUFFERSIZE)
//		{
//			FD_SET(pConn->hSocket, &fdRead);
//		}
//		if (pConn->bytes > 0)
//		{
//			FD_SET(pConn->hSocket, &fdWrite);
//		}
//		FD_SET(pConn->hSocket, &fdExcept);
//	}
//}
//
//int ODSocket::CheckAccept(const fd_set & fdRead, const fd_set & fdExcept, SOCKET sdListen, ConnectionList & conns)
//{
//	int lastErr = 0;
//	if (FD_ISSET(sdListen, &fdExcept))
//	{
//		int errlen = sizeof(lastErr);
//		getsockopt(sdListen, SOL_SOCKET, SO_ERROR, (char*)&lastErr,&errlen);
//		std::cout << "I/O error" << lastErr << std::endl;
//		return SOCKET_ERROR;
//	}
//	if (FD_ISSET(sdListen, &fdRead))
//	{
//		if (conns.size() >= FD_SETSIZE - 1)
//		{
//			return 0;
//		}
//		sockaddr_in client;
//		int size = sizeof(sockaddr_in);
//		SOCKET sd = accept(sdListen, (sockaddr*)&client, &size);
//		lastErr = WSAGetLastError();
//		if (sd == INVALID_SOCKET && lastErr != WSAEWOULDBLOCK)
//		{
//			std::cout << "accept error" << lastErr << std::endl;
//			return SOCKET_ERROR;
//		}
//		if (sd != INVALID_SOCKET)
//		{
//			u_long noBlock = 1;
//			if (ioctlsocket(sd, FIONBIO, &noBlock) == SOCKET_ERROR)
//			{
//				std::cout << "ioctlsocket error" << WSAGetLastError() << std::endl;
//				return SOCKET_ERROR;
//			}
//			conns.push_back(new Connection(sd));
//		}
//	}
//	return 0;
//}
//
//void ODSocket::CheckConn(const fd_set & fdRead, const fd_set & fdWrite, const fd_set & fdExcept, ConnectionList & conns)
//{
//	ConnectionList::iterator it = conns.begin();
//	while (it != conns.end())
//	{
//		Connection* pConn = *it;
//		bool ok = true;
//		if (FD_ISSET(pConn->hSocket, &fdExcept))
//		{
//			ok = false;
//			int lastErr;
//			int errlen = sizeof(lastErr);
//			getsockopt(pConn->hSocket, SOL_SOCKET, SO_ERROR, (char*)&lastErr, &errlen);
//			std::cout << "I/O error" << lastErr << std::endl;
//		}
//		else
//		{
//			if (FD_ISSET(pConn->hSocket, &fdRead))
//			{
//				ok = TryRead(pConn);
//			}
//			else if(FD_ISSET(pConn->hSocket, &fdWrite))
//			{
//				ok = TryWrite(pConn);
//			}
//		}
//		if (!ok)
//		{
//			closesocket(pConn->hSocket);
//			delete pConn;
//			it =conns.erase(it);
//		}
//		else
//			++it;
//	}
//}
//
//bool ODSocket::TryRead(Connection * pConn)
//{
//	int ret = recv(pConn->hSocket, pConn->Buffer + pConn->bytes, BUFFERSIZE - pConn->bytes, 0);
//	if (ret > 0)
//	{
//		pConn->bytes += ret;
//		return true;
//	}
//	else if (ret == 0)
//	{
//		std::cout << "connection close by peer" << std::endl;
//		PassiveShutdown(pConn->hSocket, pConn->Buffer, pConn->bytes);
//		return false;
//	}
//	else
//	{
//		int lastErr = WSAGetLastError();
//		if (lastErr == WSAEWOULDBLOCK)
//		{
//			return true;
//		}
//		std::cout << "recv error" << lastErr << std::endl;
//		return false;
//	}
//}
//
//bool ODSocket::TryWrite(Connection * pConn)
//{
//	int ret = send(pConn->hSocket, pConn->Buffer, pConn->bytes, 0);
//	if (ret > 0)
//	{
//		pConn->bytes -= ret;
//		if (pConn->bytes > 0)
//		{
//			memmove(pConn->Buffer, pConn->Buffer + ret, pConn->bytes);
//		}
//		return true;
//	}
//	else if (ret == 0)
//	{
//		std::cout << "connection close by peer" << std::endl;
//		PassiveShutdown(pConn->hSocket, pConn->Buffer, pConn->bytes);
//		return false;
//	}
//	else
//	{
//		int lastErr = WSAGetLastError();
//		if (lastErr == WSAEWOULDBLOCK)
//		{
//			return true;
//		}
//		std::cout << "send error" << lastErr << std::endl;
//		return false;
//	}
//}
//
//bool ODSocket::PassiveShutdown(SOCKET sd, const char * buff, int len)
//{
//	if (buff != NULL && len > 0)
//	{
//		u_long noBlock = 1;
//		if (ioctlsocket(sd, FIONBIO, &noBlock) == SOCKET_ERROR)
//		{
//			std::cout << "ioctlsocket error" << WSAGetLastError() << std::endl;
//			return false;
//		}
//		int nSend = 0;
//		while (nSend < len)
//		{
//			int temp = send(sd, &buff[nSend], len - nSend, 0);
//			if (temp > 0)
//			{
//				nSend += temp;
//			}
//			else if (temp == SOCKET_ERROR)
//			{
//				std::cout << "send error" << WSAGetLastError() << std::endl;
//				return false;
//			}
//			else
//			{
//				std::cout << "Connection closed unexceptedly by peer" << std::endl;
//				break;
//			}
//		}
//	}
//		if (shutdown(sd, 1) == SOCKET_ERROR)
//		{
//			std::cout << "shutdown error" << WSAGetLastError() << std::endl;
//			return false;
//		}
//		return true;
//}

int ODSocket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData {
	WORD wVersion;								//winsock version
	WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
	char szDescription[WSADESCRIPTION_LEN+1];
	char szSystemStatus[WSASYSSTATUS_LEN+1];
	unsigned short iMaxSockets;
	unsigned short iMaxUdpDg;
	char FAR * lpVendorInfo;
	}WSADATA, *LPWSADATA;
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret) {
		//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif

	return 0;
}
//this is just for windows
int ODSocket::Clean()
{
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

 ODSocket& ODSocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET ()
{
	return m_sock;
}
//create a socket object win/lin is the same
// af:
bool ODSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	u_long noBlock = 1;//设置非阻塞
	ioctlsocket(m_sock, FIONBIO, &noBlock);
	if (m_sock == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool ODSocket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	fd_set wtfds;
	timeval _timout;
	FD_ZERO(&wtfds);
	_timout.tv_sec = 3;
	_timout.tv_usec = 0;
	FD_SET(m_sock, &wtfds);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));//return ?
	if (!ret)
	{
		printf("connect immediately\n");
	}
	else if (ret<0 && WSAGetLastError() == WSAEWOULDBLOCK)
	{
		int iRet1 = select(0, NULL, &wtfds, NULL, &_timout);
		if (iRet1 < 0)
		{
			perror("connect error\n");
			return false;
		}
		else if (!iRet1)
		{
			perror("timeout error\n");
			return false;
		}
		else
		{
			if (FD_ISSET(m_sock, &wtfds))
			{
				printf("connect success\n");
			}
		}
	}
	/*if (ret == SOCKET_ERROR) {
		return false;
	}*/
	return true;
}

bool ODSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
		return false;

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}
//for server
bool ODSocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if (sock == SOCKET_ERROR)
	{
		return false;
	}
	u_long noBlock = 1;
	ioctlsocket(sock, FIONBIO, &noBlock);
	s = sock;
	if (fromip != NULL)
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
	return true;
}

int ODSocket::Send(const char* buf, int len, int flags)
{
	return send(m_sock, buf, len, flags);
	/*int bytes;
	int count = 0;
   
	while (count < len) {

		 bytes = send(m_sock, buf + count, len - count, flags);
	if (bytes == -1)
		return -1;
	else if (bytes == 0)
		return 0;
		count += bytes;
	}
	return count;*/
}

int ODSocket::Recv(char* buf, int len, int flags)
{
	return (recv(m_sock, buf, len, flags));
}

int ODSocket::SendTo(char * buf, int len, int flags,char* to,int port)
{
	sockaddr_in svraddr;
	socklen_t length = sizeof(svraddr);
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = (to == NULL)? INADDR_BROADCAST:inet_addr(to);//指定IP或广播地址
	svraddr.sin_port = htons(port);
	int bytes;
	bytes = sendto(m_sock, buf, len, 0, (sockaddr*)&svraddr,length);
	return bytes;
}

int ODSocket::RecvFrom(char * buf, int len, int flags, std::string &from, int &port)
{
    sockaddr_in svraddr;
	socklen_t length = sizeof(svraddr);
	int bytes;
	bytes = recvfrom(m_sock, buf, len, flags, (sockaddr*)&svraddr, &length);
	from = inet_ntoa(svraddr.sin_addr);
	port = ntohl(svraddr.sin_port);
	return bytes;
}

int ODSocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (0);
#endif
}

bool ODSocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;

	sprintf(ip,
		"%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0],
		(unsigned char)p->h_addr_list[0][1],
		(unsigned char)p->h_addr_list[0][2],
		(unsigned char)p->h_addr_list[0][3]);

	return true;
}
