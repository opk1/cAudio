#include "server_menu.h"

LPSOCKET_INFORMATION server_SocketInfo;
u_long  lTTL               = TIMECAST_TTL;
DWORD chan, reclen;
BOOL _recording;
char *recbuf = NULL;
int input;
char* globalIP;
int globalPort;

char GlobalMicBuffer[BUFFER_SIZE]  = "";
int	 GlobalMicBufferIndex	 = 0;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: init_server
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
-- INTERFACE: int init_server(char* ipAddress, int port)
--				char* ipAddress: The IP address of the server
--				int port: The port number of the server.
--
-- RETURNS: Returns 0 upon success
--
-- NOTES:
-- This function initiates the server. it will begin the server, setting the server to
-- multicast the data out. 
------------------------------------------------------------------------------------------------------------------*/
int init_server(char* ipAddress, int port){
	int nRet;
	BOOL  fFlag;
	WSADATA stWSAData;
	SOCKADDR_IN stLclAddr;
	struct ip_mreq stMreq;        /* Multicast interface structure */
	CreateEvent(NULL, FALSE, TRUE, NULL);
	WSAStartup(0x0202, &stWSAData);
	globalIP	= ipAddress;
	globalPort	= port;
	if ((server_SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL){
		System::Windows::Forms::MessageBox::Show("GlobalAlloc Error: " + WSAGetLastError());
		return -1;
	} 

	fFlag = FALSE;

	if ((server_SocketInfo->Socket = socket(AF_INET, SOCK_DGRAM, 0))== INVALID_SOCKET) {
		System::Windows::Forms::MessageBox::Show("socket() failed. Error: " + WSAGetLastError());
		return -1;
	}

	stLclAddr.sin_family      = AF_INET; 
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any interface */
	stLclAddr.sin_port        = 0;                 /* any port */
	if ((nRet = bind(server_SocketInfo->Socket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr)) == SOCKET_ERROR)) {
		System::Windows::Forms::MessageBox::Show("Bind() on port " + port + " failed. Error: " +WSAGetLastError());
		return -1;
	}

	/* Join the multicast group */
	if ((stMreq.imr_multiaddr.s_addr = inet_addr(ipAddress)) == INADDR_NONE){
		System::Windows::Forms::MessageBox::Show("Unable to get IP Address. Error: " + GetLastError());
		return -1;
	}

	stMreq.imr_interface.s_addr = INADDR_ANY;
	
	if ((nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == SOCKET_ERROR)) {
		System::Windows::Forms::MessageBox::Show("setsockopt() IP_ADD_MEMBERSHIP address: " +  " failed. Error: " + WSAGetLastError());
		return -1;
	} 

	/* Set IP TTL to traverse up to multiple routers */
	if ((nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&lTTL, sizeof(lTTL)) == SOCKET_ERROR)) {
		System::Windows::Forms::MessageBox::Show("setsockopt() IP_MULTICAST_TTL failed. Error: " + WSAGetLastError());
		return -1;
	}

	/* Disable loopback */
	fFlag = FALSE;
	if ((nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)) {
		System::Windows::Forms::MessageBox::Show("setsockopt() IP_MULTICAST_LOOP failed. Error: " + WSAGetLastError());
		return -1;
	}

	/* Assign our destination address */
	server_SocketInfo->DestAddr.sin_family		= AF_INET;
	server_SocketInfo->DestAddr.sin_addr.s_addr = inet_addr(ipAddress);
	server_SocketInfo->DestAddr.sin_port		= htons(port);

	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: run_server
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
-- INTERFACE:  void run_server()
--
-- RETURNS: -
--
-- NOTES:  While the server is running, it constantly checks the stream 'chan' to see if it is active. If the
--         channel is recording or playing audio, the server will get data from the channel and stream it via
--         multicast.
------------------------------------------------------------------------------------------------------------------*/
void run_server(){
	DWORD Flags = 0, readLength = 0;
	char streamDataBuffer[BUFFER_SIZE];

	while(1){
		/* if audio is being played or recorded */
		if(BASS_ChannelIsActive(chan) == BASS_ACTIVE_PLAYING ){
			if(( readLength =BASS_ChannelGetData(chan, streamDataBuffer, BUFFER_SIZE)) == -1){
				System::Windows::Forms::MessageBox::Show("Channel is empty. Error: " + BASS_ErrorGetCode());
					return;
			} 

			if(sendto(server_SocketInfo->Socket, (char*)streamDataBuffer, readLength, Flags, (struct sockaddr*)&(server_SocketInfo->DestAddr), sizeof(server_SocketInfo->DestAddr)) == SOCKET_ERROR){
				if (WSAGetLastError() != WSA_IO_PENDING){
					System::Windows::Forms::MessageBox::Show("sendto error. Error: " + WSAGetLastError());
					return;
				}
			}
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- If a file is specified, a stream is created and played. The BASS_STREAM_DECODE flag is set to be able to get
-- data from the channel while it is playing.
------------------------------------------------------------------------------------------------------------------*/
void play(char *filename)
{
	if (filename!=NULL)
	{
		chan = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_STREAM_DECODE);
		BASS_ChannelPlay(chan, FALSE);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- If the channel is valid, it is stopped. This will stop the playing of audio resetting the position of the
-- channel.
------------------------------------------------------------------------------------------------------------------*/
void stop(char *filename)
{
	if (filename!=NULL && chan != NULL)
	{
		/* stop the music */
		BASS_ChannelStop(chan);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- If the channel is valid, it is paused. This will stop the playing of audio saving the position of the
-- channel to optionaly resume later.
------------------------------------------------------------------------------------------------------------------*/
void pause(char *filename){
	if (filename!=NULL && chan != NULL)
	{
		/* pause the music */
		BASS_ChannelPause(chan);
	}
}

