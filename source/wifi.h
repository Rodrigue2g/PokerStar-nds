/**
 * @file wifi.h
 * @author Rodrigue de Guerre
 * @brief 
 * @version 0.1
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef WIFI_H
#define WIFI_H

#include <nds.h>
#include <dswifi9.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SSID	"POKER-STAR-NDS"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	ACK,    //0x00 : Acknowledge
	ERR     //0x01 : Error in response (other nds recieved an unexpected message)
} NetworkResponse;

//WiFi initialization
int initWiFi();
//Socket initialization
int openSocket();
//Receive data trough the socket
int receiveData(char* data_buff, int bytes);
//Send data through the socket
int sendData(char* data_buff, int bytes);
//Close the Socket
void closeSocket();
//Disconnect the WiFi
void disconnectFromWiFi();

#ifdef __cplusplus
}
#endif
#endif // WIFI_H
