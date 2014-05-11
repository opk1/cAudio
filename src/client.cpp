#include "client_menu.h"
#include "main.h"

LPSOCKET_INFORMATION SocketInfo;
DWORD inStream;
char streamDataBuffer[1000000];
char dataBuffer[40*BUFFER_SIZE];

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PlayerThread
--
-- DATE: April 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: DWORD WINAPI PlayerThread(LPVOID params)
--				LPVOID params: parameter
--
-- RETURNS: Returns 0 upon success
--
-- NOTES:
-- This function plays the instream channel on a separate thread.
------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI PlayerThread(LPVOID params){
	BASS_ChannelPlay(inStream, FALSE);
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: run_client
--
-- DATE: April 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: void run_client(System::Windows::Forms::ListBox ^lb)
--				System::Windows::Forms::ListBox ^lb: List box 
--
-- RETURNS: Returns 0 upon success
--
-- NOTES:
-- This function begins the client. It will receive the UDP packets from the multicast.
-- A separate thread will begin that will execute the song. As the packets are received,
-- the packets will be put into the buffer, then into the channel.
------------------------------------------------------------------------------------------------------------------*/
void run_client(System::Windows::Forms::ListBox ^lb){ 
	int q;
	DWORD Flags = 0, RecvBytes = 0, sizeOfBuffer = BUFFER_SIZE;
	int SenderAddrSize = sizeof (SocketInfo->SenderAddr);
	
	ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));  
	SocketInfo->BytesSEND	= 0;
	SocketInfo->BytesRECV	= 0;
	SocketInfo->DataBuf.len = sizeOfBuffer;
	SocketInfo->DataBuf.buf = (CHAR*)dataBuffer;
	SocketInfo->Overlapped.hEvent = WSACreateEvent();
	SocketInfo->pktsRcvd	= 0;
	
	CreateThread(NULL, 0, PlayerThread, NULL, 0, NULL);
	while (1) {
		SocketInfo->Overlapped.hEvent = WSACreateEvent();
		if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(SocketInfo->SenderAddr), 
			&SenderAddrSize,&(SocketInfo->Overlapped), NULL) == SOCKET_ERROR){
				if ((q = WSAGetLastError()) != WSA_IO_PENDING){
					System::Windows::Forms::MessageBox::Show("IO Pending:. No more data is being received. Error: " + WSAGetLastError());
					exit(1);
				}
				q++;
		}
	
		if (RecvBytes > 0 ){
			BASS_StreamPutData(inStream, dataBuffer, RecvBytes);
		}
		if (WSAWaitForMultipleEvents(1, &(SocketInfo->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
			System::Windows::Forms::MessageBox::Show("Wait For Multiple Events error: " + WSAGetLastError());
			exit(1);
		}
		WSAResetEvent(SocketInfo->Overlapped.hEvent);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: multicast_connect
--
-- DATE: April 02, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port)
--				System::Windows::Forms::ListBox ^lb: List box 
--				char* ip: IP address
--				int port: port number
--
-- RETURNS: Returns 0 upon success
--
-- NOTES:
-- This function connects the client to the multicast IP address.
------------------------------------------------------------------------------------------------------------------*/
int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port){
	int nRet;
	BOOL  fFlag;
	struct ip_mreq stMreq;
	WSADATA stWSAData;
	char* optVal = "4096";

	WSAStartup(0x0202, &stWSAData);

	if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL){
		System::Windows::Forms::MessageBox::Show("GlobalAlooc error. Error: " + WSAGetLastError());
		return -1;
	} 

	/* Get a datagram socket */
	if ((SocketInfo->Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		System::Windows::Forms::MessageBox::Show("Invalid Socket. Error: " + WSAGetLastError());
		WSACleanup();
		return -1;
	}

	fFlag = TRUE;
	if ((nRet = setsockopt(SocketInfo->Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)))== SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("setsockopt() SO_REUSEADDR failed. Error:" + WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	SocketInfo->SenderAddr.sin_family      = AF_INET;
	SocketInfo->SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SocketInfo->SenderAddr.sin_port        = htons(port);

	if ((nRet = bind(SocketInfo->Socket, (struct sockaddr*) &(SocketInfo->SenderAddr), sizeof(SocketInfo->SenderAddr))) == SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("Invalid Socket: Error: " + WSAGetLastError());
		return -1;
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(ip);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	if ((nRet = setsockopt(SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq))) == SOCKET_ERROR) {
		System::Windows::Forms::MessageBox::Show("setsockopt error. Error: " + WSAGetLastError());
		return -1;
	}

	if ((nRet = setsockopt(SocketInfo->Socket, SOL_SOCKET, SO_RCVBUF, optVal, sizeof(optVal))) == SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("Invalid Socket: Error: " + WSAGetLastError());
		return -1;
	}

	inStream = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
	return 0;

}



/*
int multicast_disconnect()
{

stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
stMreq.imr_interface.s_addr = INADDR_ANY;
nRet = setsockopt(hSocket, 
IPPROTO_IP, 
IP_DROP_MEMBERSHIP, 
(char *)&stMreq, 
sizeof(stMreq));
if (nRet == SOCKET_ERROR) {
printf (
"setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
achMCAddr, WSAGetLastError());
} 

closesocket(hSocket);
WSACleanup();
}*/