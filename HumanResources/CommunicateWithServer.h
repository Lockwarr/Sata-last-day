#pragma once
#include "stdafx.h"
#include "Structures.h"

#include <afxsock.h>

#define DEFAULT_PORT "34040"
#define DEFAULT_BUFLEN 512

template<class T>
class CCommunicateWithServer
{
public:
	CCommunicateWithServer()
	{
	}

private:
	SOCKET m_ConnectSocket = INVALID_SOCKET;
	int m_iResult;
public:
	virtual void Connect()
	{

		WSADATA wsData;
		WORD version = MAKEWORD(2, 2);

		int wsOk = WSAStartup(version, &wsData);
		if (wsOk != 0)
		{
			AfxMessageBox("Can't initialilze winsock. Quitting");
			return;
		}

		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		int m_iResult;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;


		// Resolve the server address and port
		m_iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
		if (m_iResult != 0)
		{
			WSACleanup();
			return;
		}

		// Attempt to connect to the first address returned by
		// the call to getaddrinfo
		ptr = result;

		ptr = ptr->ai_next; //  ? ?  

							// Create a SOCKET for connecting to server
		m_ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (m_ConnectSocket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		m_iResult = connect(m_ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (m_iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
		}

		freeaddrinfo(result);

		if (m_ConnectSocket == INVALID_SOCKET)
		{
			AfxMessageBox("Unable to connect to server!\n");
			WSACleanup();
			return;
		}

	}

	virtual void SendRequest(REQUEST recRequest, T & recReceived)
	{

		CRequestBase oRequestBase;

		oRequestBase.nCommandID = 154;
		oRequestBase.asd = "asdaff";

		std::ofstream os("data.xml");
		cereal::XMLOutputArchive archive(os);

		archive(CEREAL_NVP(oRequestBase));


		char buffer[512];

		//	strncpy_s(buffer, strAsConstChar, sizeof(buffer));

			// Send an initial buffer	
		m_iResult = send(m_ConnectSocket, buffer, int(strlen(buffer)), 0);
		if (m_iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			WSACleanup();
			return;
		}

		printf("Bytes Sent: %ld\n", m_iResult);

		// shutdown the connection for sending since no more data will be sent
		// the client can still use the m_ConnectSocket  for receiving data
		m_iResult = shutdown(m_ConnectSocket, SD_SEND);
		if (m_iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			WSACleanup();
			return;
		}

		// Receive data until the server closes the connection

	//	if (recv(m_ConnectSocket, buffer, sizeof(recReceived), 0))
	//		memcpy(&recReceived, buffer, sizeof(recReceived));


	}

	virtual void CloseConnection()
	{
		CString end = "end";
		char buffer[sizeof(end)] = { 0 };
		send(m_ConnectSocket, buffer, sizeof(end), 0);
		// shutdown the send half of the connection since no more data will be sent
		m_iResult = shutdown(m_ConnectSocket, SD_SEND);
		if (m_iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			WSACleanup();
			return;
		}

		// cleanup
		closesocket(m_ConnectSocket);
		WSACleanup();

	}



};