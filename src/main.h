#ifndef _MAIN_
#define _MAIN_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <cstdio>
#include <string>
#include <fstream> 
#include <bass.h>
#include "atlstr.h"

#define FREQ				44100
#define CHANS				2
#define BUFSTEP				200000	// memory allocation unit
#define FILENAMELENGTH 		32
#define TIMEOUT_TIME		3000
#define TIMEOUT_TIME_UDP	3000
#define TIMECAST_TTL		2
#define TIMECAST_INTRVL		7
#define BUFFER_SIZE			1024
#define FREQUENCY			44100

typedef struct _SOCKET_INFORMATION
{
	WSAOVERLAPPED Overlapped;
	WSABUF DataBuf;
	SOCKET Socket;
	SOCKADDR_IN SenderAddr;
	SOCKADDR_IN DestAddr;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;


/* 
*	FUNCTION PROTOTYPES
*/
void run_server();
int init_server(char* result, int port);
void recordMic();
void play(char *filename);
void stop(char *filename);
void pause(char *filename);
BOOL InitMicDevice(int device);
BOOL CALLBACK MicrophoneCallback(HRECORD handle, const void *buffer, DWORD length, void *user);
BOOL CALLBACK MicCallback(HRECORD handle, const void *buffer, DWORD length, void *user);

DWORD WINAPI RecordThread(LPVOID params);

int multicast_connect(System::Windows::Forms::ListBox ^lb, char* ip, int port);
void run_client(char *fileName);
void resume();
void pauseSong();
void volumeChange(float val);

#endif