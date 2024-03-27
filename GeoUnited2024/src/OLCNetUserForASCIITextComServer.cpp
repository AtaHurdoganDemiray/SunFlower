#include "stdafx.h"
#include "OLCNetUserForASCIITextComServer.h"

	//OLCNetUserForASCIITextComServer::OLCNetUserForASCIITextComServer(uint16_t nPort) 
	//	: olc::net::server_interface<CustomMsgTypes>(nPort)
	//{

	//}
	//bool OLCNetUserForASCIITextComServer::OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	//{
	//	olc::net::message<CustomMsgTypes> msg;
	//	msg.header.id = CustomMsgTypes::ServerAccept;
	//	client->Send(msg);
	//	return true;
	//}
	//// Called when a client appears to have disconnected
	//void OLCNetUserForASCIITextComServer::OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	//{
	//	std::cout << "Removing client [" << client->GetID() << "]\n";
	//}

	//// Called when a message arrives
	//void OLCNetUserForASCIITextComServer::OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg)
	//{
	//	switch (msg.header.id)
	//	{
	//	case CustomMsgTypes::ServerPing:
	//	{
	//		std::cout << "[" << client->GetID() << "]: Server Ping\n";

	//		// Simply bounce message back to client
	//		client->Send(msg);
	//	}
	//	break;

	//	case CustomMsgTypes::MessageAll:
	//	{
	//		std::cout << "[" << client->GetID() << "]: Message All\n";

	//		// Construct a new message and send it to all clients
	//		olc::net::message<CustomMsgTypes> msg;
	//		msg.header.id = CustomMsgTypes::ServerMessage;
	//		msg << client->GetID();
	//		MessageAllClients(msg, client);

	//	}
	//	break;
	//	}
	//}
	//void OLCNetUserForASCIITextComServer::ConstructThread(uint16_t port)
	//{
	//	std::function<int()> myfptr = [port]() 
	//		{
	//			OLCNetUserForASCIITextComServer server(port);
	//			server.Start();
	//			while (1)
	//			{
	//				server.Update(-1, true);
	//			}
	//			return 0;
	//		};
	//
	//	std::thread* mythr = new std::thread(myfptr);
	//}
