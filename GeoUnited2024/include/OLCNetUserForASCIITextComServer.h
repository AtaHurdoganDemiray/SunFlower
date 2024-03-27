#ifndef __OLCNetUser_h__
#define __OLCNetUser_h__

#include <thread>
//#include <olc_net.h>

//enum class CustomMsgTypes : uint32_t
//{
//	ServerAccept,
//	ServerDeny,
//	ServerPing,
//	MessageAll,
//	ServerMessage,
//};

//class OLCNetUserForASCIITextComServer : public olc::net::server_interface<CustomMsgTypes> , public std::thread
//{
//
//
//private:
//	static std::thread m_thr;
//	uint16_t m_port;
//
//public:
//	OLCNetUserForASCIITextComServer(uint16_t port);
//	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client);
//	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client);
//	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg);
//	static void ConstructThread(uint16_t port);
//};

#endif
