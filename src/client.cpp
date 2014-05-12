#include "client_menu.h"
#include "main.h"

LPSOCKET_INFORMATION SocketInfo;
LPSOCKET_INFORMATION SocketInfo2;
DWORD inStream;
char streamDataBuffer[1000000];
char dataBuffer[40*BUFFER_SIZE];
BOOL _clientrecording;
DWORD chanClient;
int inputClient;
char* globalClientIP;
int globalClientPort;
char GlobalClientMicBuffer[BUFFER_SIZE]  = "";
int	 GlobalClientMicBufferIndex	= 0;
FILE *fps;
LPSOCKET_INFORMATION server_SocketInfoClient;
float value = 0;

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
void run_client(char *fileName){ 
	DWORD Flags = 0, RecvBytes = 0, sizeOfBuffer = BUFFER_SIZE;
	int SenderAddrSize = sizeof (SocketInfo2->SenderAddr);
	WAVEFORMATEX wf;
	BASS_CHANNELINFO info;

	ZeroMemory(&(SocketInfo2->Overlapped), sizeof(WSAOVERLAPPED));  
	SocketInfo2->DataBuf.len = sizeOfBuffer;
	SocketInfo2->DataBuf.buf = (CHAR*)dataBuffer;
	SocketInfo2->Overlapped.hEvent = WSACreateEvent();

	if(fileName != NULL)
	{
		fps = fopen(fileName, "ab+");
		// write WAV header
		BASS_ChannelGetInfo(inStream,&info);
		wf.wFormatTag=1;
		wf.nChannels=info.chans;
		wf.wBitsPerSample=(info.flags&BASS_SAMPLE_8BITS?8:16);
		wf.nBlockAlign=wf.nChannels*wf.wBitsPerSample/8;
		wf.nSamplesPerSec=info.freq;
		wf.nAvgBytesPerSec=wf.nSamplesPerSec*wf.nBlockAlign;

		fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0",20,1,fps);
		fwrite(&wf,16,1,fps);
		fwrite("data\0\0\0\0",8,1,fps);
	}

	while (1) {
		SocketInfo2->Overlapped.hEvent = WSACreateEvent();
		if (WSARecvFrom(SocketInfo2->Socket, &(SocketInfo2->DataBuf), 1, &RecvBytes, &Flags, (SOCKADDR*) &(SocketInfo2->SenderAddr), 
			&SenderAddrSize,&(SocketInfo2->Overlapped), NULL) == SOCKET_ERROR){
				if (WSAGetLastError() != WSA_IO_PENDING){
					System::Windows::Forms::MessageBox::Show("IO Pending:. No more data is being received. Error: " + WSAGetLastError());
					exit(1);
				}
		}
	
		if (WSAWaitForMultipleEvents(1, &(SocketInfo2->Overlapped.hEvent), TRUE, WSA_INFINITE, TRUE) == WSA_WAIT_TIMEOUT){
			System::Windows::Forms::MessageBox::Show("Wait For Multiple Events error: " + WSAGetLastError());
			exit(1);
		}
		WSAResetEvent(SocketInfo2->Overlapped.hEvent);

		if (RecvBytes > 0 ){
			if (BASS_StreamPutData(inStream, dataBuffer, RecvBytes) == -1){
				System::Windows::Forms::MessageBox::Show("Unable to put the data in the stream. Error: " + BASS_ErrorGetCode());
				exit(1);
			}
			if(fileName != NULL)
			{
				fwrite(dataBuffer, sizeof(char), RecvBytes, fps);
				fflush(fps);
			}
		}
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
	
	char* optVal = "2048";
	WSADATA stWSAData;
	WSAStartup(0x0202, &stWSAData);

	if ((SocketInfo2 = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL){
		System::Windows::Forms::MessageBox::Show("GlobalAlooc error. Error: " + WSAGetLastError());
		return -1;
	} 

	/* Get a datagram socket */
	if ((SocketInfo2->Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		System::Windows::Forms::MessageBox::Show("Invalid Socket. Error: " + WSAGetLastError());
		WSACleanup();
		return -1;
	}

	fFlag = TRUE;
	if ((nRet = setsockopt(SocketInfo2->Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)))== SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("setsockopt() SO_REUSEADDR failed. Error:" + WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	SocketInfo2->SenderAddr.sin_family      = AF_INET;
	SocketInfo2->SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SocketInfo2->SenderAddr.sin_port        = htons(port);

	if ((nRet = bind(SocketInfo2->Socket, (struct sockaddr*) &(SocketInfo2->SenderAddr), sizeof(SocketInfo2->SenderAddr))) == SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("Invalid Socket: Error: " + WSAGetLastError());
		return -1;
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr(ip);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	if ((nRet = setsockopt(SocketInfo2->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq))) == SOCKET_ERROR) {
		System::Windows::Forms::MessageBox::Show("setsockopt error. Error: " + WSAGetLastError());
		return -1;
	}

	if ((nRet = setsockopt(SocketInfo2->Socket, SOL_SOCKET, SO_RCVBUF, optVal, sizeof(optVal))) == SOCKET_ERROR){
		System::Windows::Forms::MessageBox::Show("Invalid Socket: Error: " + WSAGetLastError());
		return -1;
	}

	inStream = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
	if (!BASS_ChannelPlay(inStream, FALSE)){
		System::Windows::Forms::MessageBox::Show("Unable to play from the stream. Error: " + BASS_ErrorGetCode());
	}
	return 0;

}


void recordMic()
{
		SOCKADDR_IN mcAddr;
		struct ip_mreq stMreq;  
		mcAddr.sin_family =      AF_INET;
		mcAddr.sin_addr.s_addr = inet_addr(globalClientIP);
		mcAddr.sin_port =        htons(globalClientPort);
		globalClientIP = "234.234.234.7";
		globalClientPort = 8195;
		int nRet;
		BOOL  fFlag;
		
		WSADATA stWSAData;
		WSAStartup(0x0202, &stWSAData);

		if(!_clientrecording)
		{
			if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL){
				System::Windows::Forms::MessageBox::Show("GlobalAlooc error. Error: " + WSAGetLastError());
				return;
			} 

		/* Get a datagram socket */
		if ((SocketInfo->Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
			System::Windows::Forms::MessageBox::Show("Invalid Socket. Error: " + WSAGetLastError());
			WSACleanup();
			return;
		}
			/* Join the multicast group */
		if ((stMreq.imr_multiaddr.s_addr = inet_addr(globalClientIP)) == INADDR_NONE){
			System::Windows::Forms::MessageBox::Show("Unable to get IP Address. Error: " + GetLastError());
			return;
		}

		stMreq.imr_interface.s_addr = INADDR_ANY;
	
		if ((nRet = setsockopt(SocketInfo->Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == SOCKET_ERROR)) {
			System::Windows::Forms::MessageBox::Show("setsockopt() IP_ADD_MEMBERSHIP address: " +  " failed. Error: " + WSAGetLastError());
			return;
		} 

		/* Disable loopback */
		fFlag = FALSE;
		if ((nRet = setsockopt(SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)) {
			System::Windows::Forms::MessageBox::Show("setsockopt() IP_MULTICAST_LOOP failed. Error: " + WSAGetLastError());
			return;
		}

		/* Assign our destination address */
		SocketInfo->DestAddr.sin_family		= AF_INET;
		SocketInfo->DestAddr.sin_addr.s_addr = inet_addr(globalClientIP);
		SocketInfo->DestAddr.sin_port		= htons(globalClientPort);
		
		InitMicDevice(0);
		if (chanClient == 0){
			chanClient = BASS_StreamCreate(FREQUENCY, CHANS, 0, STREAMPROC_PUSH, 0); 
			BASS_ChannelPlay(chanClient, FALSE);
		}
	
		HRECORD record = BASS_RecordStart(FREQUENCY, 2, 0, MicCallback, 0);
		
	}
}

BOOL InitMicDevice(int device){
	BASS_RecordFree(); // free current device (and recording channel) if there is one
	// initalize new device
	if (!BASS_RecordInit(device)) {
		System::Windows::Forms::MessageBox::Show("Can't initialize recording device");
		return FALSE;
	}
	 // get list of inputs
	int c;
	const char *i;
	for (c=0;i=BASS_RecordGetInputName(c);c++) {
		if (!(BASS_RecordGetInput(c,NULL)&BASS_INPUT_OFF)) { // this one is currently "on"
			inputClient=c;
		}
	}
	
	return TRUE;
}

BOOL CALLBACK MicCallback(HRECORD handle, const void *buffer, DWORD length, void *user) {

	for(DWORD i = 0; i < length; i++) {
		GlobalClientMicBuffer[GlobalClientMicBufferIndex++] = *((char*)buffer + i);
		if (GlobalClientMicBufferIndex == BUFFER_SIZE) {
			sendto(SocketInfo->Socket, GlobalClientMicBuffer, BUFFER_SIZE, 0, (const sockaddr *)&(SocketInfo->DestAddr), sizeof(SocketInfo->DestAddr));
			GlobalClientMicBufferIndex = 0;
		}
	}

	return TRUE;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: pauseSong
--
-- DATE: April 10, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: void pauseSong()
--
-- RETURNS: R-
--
-- NOTES:
-- This function will pause the song.
------------------------------------------------------------------------------------------------------------------*/
void pauseSong(){
	if (inStream != NULL){
		BASS_ChannelPause(inStream);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: resume
--
-- DATE: April 10, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: void resume()
--
-- RETURNS: -
--
-- NOTES:
-- This function will resume the song.
------------------------------------------------------------------------------------------------------------------*/
void resume(){
	if (inStream != NULL)
		BASS_ChannelPlay(inStream, FALSE);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: volumeChange
--
-- DATE: April 10, 2014
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski
--			 Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle/Mat Siwoski
--
-- INTERFACE: void  volumeChange(float val)
--				float val: the volume of the slider
--
-- RETURNS: -
--
-- NOTES:
-- This function will adjust the volume of the music
------------------------------------------------------------------------------------------------------------------*/
void volumeChange(float val){
	
	BASS_ChannelGetAttribute(inStream, BASS_ATTRIB_VOL, &value);
	value = val/10;

	if (!BASS_ChannelSetAttribute(inStream, BASS_ATTRIB_VOL,value)){
		System::Windows::Forms::MessageBox::Show("Cannot Change Volume "+  BASS_ErrorGetCode());
	}
	if (!BASS_ChannelPlay(inStream, FALSE))
		printf("Can't play sample");
}