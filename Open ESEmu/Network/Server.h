#pragma once
#ifndef _SERVER_H_
#define _SERVER_H_

#include <boost\exception\diagnostic_information.hpp>
#include <boost\asio.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\shared_array.hpp>
#include <boost\bind.hpp>
#include <string>
#include <vector>
#include "Session Cache\SessionCache.h"
#include "Stream Handler\StreamHandler.h"
#include "..\Common\Data\Constants\Constants.h"
//#include "..\Common\Cryptography\DES.h"
//#include "..\Common\Cryptography\HMAC.h"

#define NOCRYPTO	0
#define NAES		1


class SessionHandler: public boost::enable_shared_from_this<SessionHandler>
{
private:
	std::string			_accountName;
	unsigned long		_accountID;
	std::string			_sessionToken;
	bool				_queueKickOut = false;
	_sessionCache*		_cache;
	StreamHandler*		_stream;
	ConstantsHolder*	_playerMapCache;
	//DES*				_crypto;
	//vHMAC*				_hash;

	boost::shared_array<unsigned char>	_recvBuffer;
	boost::asio::ip::tcp::socket		tcpSocket;

public:

	SessionHandler(boost::asio::io_service& IOService) : tcpSocket(IOService) 
	{ 
		this->_accountID = 0; 
		this->_accountName = "";
		this->_sessionToken = "";
		
		//this->_crypto = new DES((char*)g_ConfigManager->getInstance()->getString(g_ConfigManager->DEF_DES).c_str());
		//this->_hash	  = new vHMAC((char*)g_ConfigManager->getInstance()->getString(g_ConfigManager->DEF_HMAC).c_str());
	}

	boost::asio::ip::tcp::socket& getSocket();
	void Init();
	void Start();
	void StartHeaderReading();
	void StartBodyReading(const boost::system::error_code& error);
	void ReadHandler(int Size, const boost::system::error_code& error);
	void Send(unsigned char* Packet, unsigned int Size, bool toDisconnect);
	void Send(bool toDisconnect);
	void WriteHandler(const boost::system::error_code& error, size_t bytes_transferred);
	std::string GetIP();

	_sessionCache* getSessionCache()
	{
		return this->_cache;
	}
};

class Server: boost::noncopyable
{
private:
	typedef boost::shared_ptr<SessionHandler> _Session;
	unsigned int _currentSessions = 0;
	unsigned int _maxSessions = 1;
	unsigned int _maxTCPWindow = 2048;

public:
	boost::asio::io_service IOService;
	boost::asio::ip::tcp::acceptor Acceptor;

	Server() : Acceptor(IOService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9400))
	{
		this->_maxTCPWindow = g_ConfigManager->getInstance()->getNumber(g_ConfigManager->MAX_TCP_WINDOW);
	}

	~Server();
	void Start();
	void AcceptanceHandler(_Session pSession, const boost::system::error_code& error);
	bool Initialize(unsigned int maxClients);

	void _incrementSessionCounter()
	{
		this->_currentSessions++;
		printf("[+] Current sessions: %d.\n", this->_currentSessions);
	}

	void _decrementSessionCounter()
	{
		this->_currentSessions--;
	}

	unsigned int maxWindow()
	{
		return this->_maxTCPWindow;
	}

};

extern Server* _server;

#endif