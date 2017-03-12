#include "Server.h"
#include "Stream Handler\StreamHandler.h"
#include "CommandCallback.h"
#include "..\Common\LUA Engine\LuaAPI.h"

#define REMOTE_IP	this->tcpSocket.remote_endpoint().address().to_string().c_str()

#define PDUMP(buffer, size) { for (int i = 0; i < size; i ++) \
									printf("%02X ", buffer[i]); \
							  printf("\n"); }

boost::asio::ip::tcp::socket& SessionHandler::getSocket()
{
	return this->tcpSocket;
}

std::string SessionHandler::GetIP()
{
	return REMOTE_IP;
}

void SessionHandler::Start()
{
	printf("[+] New player connected (%s).\n", REMOTE_IP);
	this->StartHeaderReading();
}

void SessionHandler::StartHeaderReading()
{
	this->_recvBuffer.reset(new unsigned char[_server->maxWindow()]);
	//memset(this->_recvBuffer.get(), 0x00, 2048);
	boost::asio::async_read(this->tcpSocket, boost::asio::buffer(this->_recvBuffer.get(), _server->maxWindow()), boost::asio::transfer_at_least(2), boost::bind(&SessionHandler::StartBodyReading, shared_from_this(), boost::asio::placeholders::error));
}

void SessionHandler::StartBodyReading(const boost::system::error_code& error)
{
	if (!error)
	{
		try 
		{
			unsigned int packetLen = static_cast<unsigned int>(this->_recvBuffer.get()[0]);
			unsigned int packetProtocol = static_cast<unsigned int>(this->_recvBuffer.get()[4]);

			if (packetProtocol == g_ConfigManager->getInstance()->getNumber(g_ConfigManager->VPROTOCOL_VERSION))
			{
				printf("[+] Packet length received %d\n", packetLen);
				if (packetLen <= _server->maxWindow())
				{
					this->_stream = new StreamHandler(this->_recvBuffer.get(), packetLen);

					DeterminatePacket(this->_stream, this);

					//this->_recvBuffer.reset(new unsigned char[2048]);

					boost::asio::async_read(this->tcpSocket, boost::asio::buffer(this->_recvBuffer.get(), packetLen), boost::bind(&SessionHandler::ReadHandler, shared_from_this(), packetLen, boost::asio::placeholders::error));
				} else {
					printf("[!] Wrong TCP Window for %s.\n", REMOTE_IP);
					this->tcpSocket.close();
				}
			} else {
				printf("[!] Wrong protocol version for %s.\n", REMOTE_IP);
				this->tcpSocket.close();
			}
		} catch (boost::system::system_error& e) {
			printf("[!] %s:\n%s\n\n", e.what(), boost::diagnostic_information(e).c_str());
			_server->_decrementSessionCounter();
			return;
		}
	} else {
		printf("[+] Client disconnected (%s %s).\n", REMOTE_IP, error.message().c_str());
		_server->_decrementSessionCounter();
	}
}

void SessionHandler::Send(unsigned char* Packet, unsigned int Size, bool toDisconnect)
{
	boost::asio::async_write(this->tcpSocket, boost::asio::buffer(Packet, Size), boost::bind(&SessionHandler::WriteHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SessionHandler::Send(bool toDisconnect)
{
	boost::asio::async_write(this->tcpSocket, boost::asio::buffer(this->_stream->w->GetBuffer(), this->_stream->w->GetIndex()), boost::bind(&SessionHandler::WriteHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void SessionHandler::WriteHandler(const boost::system::error_code&, size_t)
{ 
	free(this->_stream);

	if (this->_queueKickOut) 
	{
		this->getSocket().close();
		_server->_decrementSessionCounter();
	}
}

void SessionHandler::ReadHandler(int Size, const boost::system::error_code& error)
{
	if (error)
	{
		printf("[!] ReadHandler Error: %s.\n", error.message().c_str());
		_server->_decrementSessionCounter();
		return;
	} else
		this->StartHeaderReading();
}

void SessionHandler::Init()
{
	this->_cache = new _sessionCache();
	_server->_incrementSessionCounter();
	this->_playerMapCache = new ConstantsHolder();
	// ------------ SESSION KEY PACKET --------------

	// ----------------------------------------------
}

Server::~Server()
{
	printf("[+] Server stopped.\n");
}

void Server::Start()
{
	_Session tSession(new SessionHandler(this->IOService));

	if (!tSession)
		printf("[!] Invalid session class (not initialized).\n");
	try 
	{
		this->Acceptor.async_accept(tSession->getSocket(), boost::bind(&Server::AcceptanceHandler, this, tSession, boost::asio::placeholders::error));
	} catch (boost::system::system_error& e) {
		printf("[!] %s:\n%s\n\n", e.what(), boost::diagnostic_information(e).c_str());
		return;
	}
}

void Server::AcceptanceHandler(_Session pSession, const boost::system::error_code& error)
{
	if (this->_currentSessions < this->_maxSessions)
	{
		if (!error)
		{
			pSession->Init();
			pSession->Start();
			this->Start();
		} else {
			printf("[!] AcceptanceHandler Error: %s.\n", error.message().c_str());
			_server->_decrementSessionCounter();
		}
	} else {
		if (!error)
			this->Start();
		else
			printf("[!] AcceptanceHandler Error: %s.\n", error.message().c_str());
	}
}

bool Server::Initialize(unsigned int maxClients)
{
	boost::system::error_code EC;
	try 
	{
		printf("[+] Accepting maximum %d sessions.\n", maxClients);
		this->Start();
		this->IOService.run();
	}
	catch (boost::system::system_error& e) {
		printf("[!] %s:\n%s\n\n", e.what(), boost::diagnostic_information(e).c_str());
		return false;
	}
	//if (!EC)
	//	return true;

	//printf("IOService error: %s.\n", EC.message().c_str());
	return false;
}

Server* _server = new Server();