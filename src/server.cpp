#include "server_menu.h"

HWND ghwnd;
LPSOCKET_INFORMATION server_SocketInfo;
u_long  lTTL               = TIMECAST_TTL;
BASS_CHANNELINFO info;
WAVEFORMATEX wf;
HRECORD rec;
DWORD chan, rchan, reclen, p;
DWORD outStream;
BOOL _recording;
char *recbuf=NULL;
short buf[1024];
int input;
DWORD outdev[2];	// output devices
DWORD latency[2];	// latencies
int stream;
FILE *fp;
HANDLE sendEvent;

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
	nRet = setsockopt(server_SocketInfo->Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR) {
		System::Windows::Forms::MessageBox::Show("setsockopt() IP_MULTICAST_LOOP failed. Error: " + WSAGetLastError());
		return -1;
	}

	/* Assign our destination address */
	server_SocketInfo->DestAddr.sin_family		= AF_INET;
	server_SocketInfo->DestAddr.sin_addr.s_addr = inet_addr(ipAddress);
	server_SocketInfo->DestAddr.sin_port		= htons(port);

	return 0;
}

void run_server(){
	DWORD Flags = 0;
	char streamDataBuffer[BUFFER_SIZE];

	while(1){
		if(BASS_ChannelIsActive(chan) == BASS_ACTIVE_PLAYING ){
			DWORD readLength =	BASS_ChannelGetData(chan, streamDataBuffer, BUFFER_SIZE);
			server_SocketInfo->DataBuf.buf = (CHAR*)streamDataBuffer;
			server_SocketInfo->DataBuf.len = readLength;
			if(sendto(server_SocketInfo->Socket, (char*)streamDataBuffer, server_SocketInfo->DataBuf.len, Flags, (struct sockaddr*)&(server_SocketInfo->DestAddr), sizeof(server_SocketInfo->DestAddr)) == SOCKET_ERROR){
				if (WSAGetLastError() != WSA_IO_PENDING){
					System::Windows::Forms::MessageBox::Show("sendto error. Error: " + WSAGetLastError());
					return;
				}
			}
		}
	}

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: play
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
-- INTERFACE:void play(char *filename)
--				char* filename: filename path of the song
--
-- RETURNS: Returns 0 upon success
--
-- NOTES:
-- This will create a stream of the file and play the file.
------------------------------------------------------------------------------------------------------------------*/
void play(char *filename)
{
	if (filename!=NULL)
	{
		chan = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_STREAM_DECODE);
		BASS_ChannelPlay(chan, FALSE);
	}else
		BASS_ChannelPlay(chan, FALSE);
}

void stop(char *filename)
{
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelStop(chan); // stop the music
	}
}

void pause(char *filename){
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelPause(chan);	//pause the music
	}
}

void playPause(char *filename){
	if (filename!=NULL && chan != NULL)
	{
		BASS_ChannelPlay(chan, FALSE); // play the stream (continue from current position)
	}
}

void record()
{
	if(!_recording)
	{
		InitDevice(-1);
		StartRecording();
	}else
		StopRecording();
}

DWORD WINAPI PlayThread(LPVOID params){
	while(1){
		Sleep(1000);
		SetEvent(sendEvent);
	}
}


BOOL InitDevice(int device)
{
	BASS_RecordFree(); // free current device (and recording channel) if there is one
	// initalize new device
	if (!BASS_RecordInit(device)) {
		//Error("Can't initialize recording device");
		return FALSE;
	}
	{ // get list of inputs
		int c;
		const char *i;
		for (c=0;i=BASS_RecordGetInputName(c);c++) {
			if (!(BASS_RecordGetInput(c,NULL)&BASS_INPUT_OFF)) { // this one is currently "on"
				input=c;
			}
		}
	}
	return TRUE;
}

// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{

	BASS_StreamPutData(outStream, buffer, length);
	// increase buffer size if needed
	if ((reclen%BUFSTEP)+length>=BUFSTEP) {
		recbuf=(char*)realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
		if (!recbuf) {
			rchan=0;
			//Error("Out of memory!");
			//MESS(10,WM_SETTEXT,0,"Record");
			return FALSE; // stop recording
		}
	}
	// buffer the data
	memcpy(recbuf+reclen,buffer,length);
	reclen+=length;
	return TRUE; // continue recording
}

void StartRecording()
{
	WAVEFORMATEX *wf;

	_recording = true;

	outStream = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

	if (recbuf) { // free old recording
		BASS_StreamFree(chan);
		chan=0;
		free(recbuf);
		recbuf=NULL;
		//EnableWindow(DLGITEM(11),FALSE);
		//EnableWindow(DLGITEM(12),FALSE);
		// close output device before recording incase of half-duplex device
		BASS_Free();
	}
	// allocate initial buffer and make space for WAVE header
	recbuf=(char*)malloc(BUFSTEP);
	reclen=44;
	// fill the WAVE header
	memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0",20);
	memcpy(recbuf+36,"data\0\0\0\0",8);
	wf=(WAVEFORMATEX*)(recbuf+20);
	wf->wFormatTag=1;
	wf->nChannels=CHANS;
	wf->wBitsPerSample=16;
	wf->nSamplesPerSec=FREQ;
	wf->nBlockAlign=wf->nChannels*wf->wBitsPerSample/8;
	wf->nAvgBytesPerSec=wf->nSamplesPerSec*wf->nBlockAlign;
	// start recording
	rchan=BASS_RecordStart(FREQ,CHANS,0,RecordingCallback,0);
	if (!rchan) {
		//Error("Couldn't start recording");
		free(recbuf);
		recbuf=0;
		return;
	}
}

void StopRecording()
{
	_recording = false;

	BASS_ChannelStop(rchan);
	rchan=0;

	// complete the WAVE header
	*(DWORD*)(recbuf+4)=reclen-8;
	*(DWORD*)(recbuf+40)=reclen-44;

	// create a stream from the recording
	if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,BASS_STREAM_DECODE))
		printf("sfd");
	else 
		BASS_Free();
}