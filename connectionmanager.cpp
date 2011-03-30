#include "connectionmanager.h"

int CConnectionManager::mReferences = 0;
int CConnectionManager::mSocket = -1;
map<int, CConnection *> CConnectionManager::mConnections;
map<struct sockaddr_in, string> CConnectionManager::mGames;
map<string, time_t> CConnectionManager::mGameAges;

CConnectionManager::CConnectionManager()
{
   if(0 == mReferences++) {
      mSocket = socket(PF_INET, SOCK_DGRAM, getprotobyname("udp")->p_proto);
      if(mSocket != -1) {
	 int on = 1;
	 setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	 struct sockaddr_in addr;
	 memset(&addr, 0, sizeof(addr));
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(42424);
	 addr.sin_addr.s_addr = INADDR_ANY;
	 bind(mSocket, (struct sockaddr *)&addr, sizeof(addr));
      }
   }
}

CConnectionManager::CConnectionManager(const CConnectionManager &source)
{
   *this = source;
}

CConnectionManager::~CConnectionManager()
{
   if(--mReferences) {
      for(map<int, CConnection *>::iterator i = mConnections.begin(); i != mConnections.end(); i++) {
	 delete i->second;
      }
      mConnections.clear();
      mGames.clear();
      mGameAges.clear();
      if(mSocket >= 0) {
	 close(mSocket);
	 mSocket = -1;
      }
   }
}

CConnectionManager &CConnectionManager::operator=(const CConnectionManager &source)
{
   mReferences++;
   return *this;
}

CConnectionManager CConnectionManager::get_Instance()
{
   return CConnectionManager();
}

void CConnectionManager::Pulse()
{
   fd_set read;
   struct timeval tv;
   int result;
   int max = mSocket;
   char packetId;
   struct sockaddr_in addr;
   socklen_t len;

   FD_ZERO(&read);
   
   FD_SET(mSocket, &read);
   for(map<int, CConnection *>::iterator i = mConnections.begin(); i != mConnections.end(); i++) {
      if(i->second != NULL) {
	 FD_SET(i->first, &read);
	 if(max > i->first) max = i->first;
      }
   }
   
   tv.tv_sec = 0;
   tv.tv_usec = 10;
   
   result = select(max + 1, &read, NULL, NULL, &tv);
   
   if (result > 0) {
      if(FD_ISSET(mSocket, &read)) {
	 len = sizeof(addr);
	 if(recvfrom(mSocket, &packetId, 1, MSG_PEEK, (struct sockaddr *)&addr, &len) > 0) {
	    switch(packetId) {
	     case UPID_GAME_INFO_LITE:
	     case UPID_GAME_INFO:
	     default:
		 //throw the packet away
		 recvfrom(mSocket, &packetId, 1, 0, (struct sockaddr *)&addr, &len);
	       break;
	    }
	 }
      }
      for(map<int, CConnection *>::iterator i = mConnections.begin(); i != mConnections.end(); i++) {
	 if(i->second != NULL && FD_ISSET(mSocket, &read)) {
	    len = sizeof(addr);
	    if(recvfrom(mSocket, &packetId, 1, MSG_PEEK, (struct sockaddr *)&addr, &len) > 0) {
	       switch(packetId) {
		case UPID_GAME_INFO_LITE:
		case UPID_GAME_INFO:
		default:
		    //throw the packet away
		    recvfrom(mSocket, &packetId, 1, 0, (struct sockaddr *)&addr, &len);
		  break;
	       }
	    }
	 }
      }
   }
}
