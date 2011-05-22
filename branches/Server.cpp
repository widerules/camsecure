/*
 *  Server.cpp
 *  RALPH
 *
 *  Created by Ronald on 15/05/11.
 *  
 *
 */
#include "Tracker.h"

#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>



static int sock;
static struct sockaddr_in sin;
static int s;
static FILE* f;
static bool thread = false;
static bool connected = false;

int SendData(DATA* data)
{
	if (connected)
	{
		char buf[10];
			int size = fwrite(buf, 1, 10, f);
		
		printf("\n %d ",size);
		
	}
	
	return 0;
}

int closeall()
{
	
	thread = false;
	connected = false;
	fclose(f);
	close(sock);
	
	
}

int WebserverCaller()
{
	thread = true;
	
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(5000);
	bind(sock, (struct sockaddr *) &sin, sizeof(sin));
	
	listen(sock, 5);
	printf(" server listening on port %d \n", 5000);
	
	while (thread)
	{
		
		
		s = accept(sock, NULL, NULL);
		if (s < 0) break;
		
		f = fdopen(s, "r+");
		connected = true;
	}
	
	
	fclose(f);
	close(sock);
	return 0;
}
