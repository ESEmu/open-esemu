--		:::::::::: ::::::::  :::::::::: ::::    ::::  :::    ::: 
--		:+:       :+:    :+: :+:        +:+:+: :+:+:+ :+:    :+: 
--		+:+       +:+        +:+        +:+ +:+:+ +:+ +:+    +:+ 
--		+#++:++#  +#++:++#++ +#++:++#   +#+  +:+  +#+ +#+    +:+ 
--		+#+              +#+ +#+        +#+       +#+ +#+    +#+ 
--		#+#       #+#    #+# #+#        #+#       #+# #+#    #+# 
--		########## ########  ########## ###       ###  ########  
--
--
--			    .__________      .__.__     _____ _______  ____ 
--			  __| _\_____  ___  _|__|  |   /  |  |\   _  \/_   |
--			 / __ |  _(__  \  \/ |  |  |  /   |  |/  /_\  \|   |
--			/ /_/ | /       \   /|  |  |_/    ^   \  \_/   |   |
--			\____ |/______  /\_/ |__|____\____   | \_____  |___|
--			     \/       \/                  |__|       \/     
--									
--									Created:   12/03/2017 - d3vil401
--									Last Edit: 12/03/2017 - d3vil401

--  __                 
-- (_   _  ._    _  ._ 
-- __) (/_ | \/ (/_ |  

ServerIP 			= "127.0.0.1"	-- Not really used for now, yet will be used soon.
ServerPort 			= 1				-- Not used for now, will be added later.
ServerName 			= "d3vil401"	-- Used for internal naming and core synchronization reference.
TestServer 			= "yes"			-- Enable test server functionalities?
MaxClients 			= 200			-- Maximum clients allowed here.
vProtocol			= 1				-- 1 has never been implemented, also from the official server files log results 2.
MaxTCPWindow		= 2048			-- Leave it as it is.

--  _                                      
-- /  ._   ._ _|_  _   _  ._ _. ._  |_     
-- \_ | \/ |_) |_ (_) (_| | (_| |_) | | \/ 
--      /  |           _|       |       /  

cDefaultDES 			= "\xC7\xD8\xC4\xBF\xB5\xE9\xC0\xFD"
cDefaultHMAC			= "\xC0\xD3\xBD\xC3\xB7\xCE\xB8\xB8"

cPRNGFile			= "d3vil401.seed"
cPRNGSize			= 1024				-- default generator takes 1024 bytes, if you have a custom seed file, edit this.
cPRNGEncrypted		= "no"				-- not implemented.
cPRNGKey			= "testtest"

--  __             __.         
-- /  ` _ ._. _   (__   .._  _.
-- \__.(_)[  (/,  .__)\_|[ )(_.
--                    ._|      

CSyncEnable			= "yes"
-- If you plan to use this core as master, tip is making it a "empty" core (no game/login/whatever, just synchronization core).
CSyncRole			= 1 			-- ROLE_MASTER = 1 <--------------------\
									-- ROLE_PROXY  = 2 <-- NOT IMPLEMENTED --\
									-- ROLE_CLIENT = 3						 |
--																			 |
-- Please always specify the core type (CORRECTLY).							 |
CSyncType			= 1 			-- TYPE_RELAY = 1 <-- NOT IMPLEMENTED <--/
									-- TYPE_LOGIN = 2
									-- TYPE_GAME  = 3

CSyncIP				= "127.0.0.1"			-- If MASTER can be ignored, if client to what server connect.
CSyncPort			= 8271					-- If MASTER what port use, if client what port connect to.
CSyncKey			= "d3vil401d3vil401" 	-- Initial encryption key 
											--   (Diffie Hellman packet will change it after session creation, DO NOT SHARE).
									
--    ___ ___ _     _   _  __ ___    __                 
-- |_| |   | |_)   |_) |_ (_   |    (_   _  ._    _  ._ 
-- | | |   | |     | \ |_ __)  |    __) (/_ | \/ (/_ |  

RESTAPIEnable 		= "yes"					-- HTTP API enabling.
RESTAPIPort			= 3401				   --|--- HTTP API Server Port.
RESTMaxSessions		= 5					   --|--- Maximum sessions for the server.
RESTWhiteListEnable = "yes"				   --|--- Only accept whitelisted IPs.
RESTWhiteList 		= "httpwl.lst"			   --|--- Whitelist location.
RESTEnableSSL		= "no"					 --|--- Enable HTTP SSL (HTTPS)
RESTPrivateKey		= "https-master.pem"	   --|--- Private Key name inside HTTP .\data\HTTPs\
RESTPrivateCert		= "https-cert.pem"		   --|--- Private Key Certificate name inside HTTP .\data\HTTPs\
RESTAuthType		= 1						-- 1. Basic HTTP Auth Type.
											-- 2. Seeded token only (RSA Public Key).
RESTAuthPKey		= "rest-private.pem"	-- Used to verify pubkey (auth token) inside .\data\restful\
RESTAuthPKPass		= "testtest"			-- Passphrase of the private, if there's any otherwise keep it "".

--  _                             
-- | \  _. _|_  _. |_   _.  _  _  
-- |_/ (_|  |_ (_| |_) (_| _> (/_ 
                                

DBHost 				= "127.0.0.1"	-- Database IP.
DBPort 				= 3306			-- Database Port.
DBUser 				= "root"		-- Username.
DBPass 				= ""			-- Password.
DBDatabase 			= "OpenESEmu"	-- Database to select.
DBSSL  				= "no"				 ---\- Use SSL Encryption.
DBCACert			= "ca.pem"			   --|--- CA Certification.
DBPubCert			= "client-cert.pem"	   --|--- Client Public Certification.
DBPubKey			= "client-key.pem"	   --|--- Client Public Key.

--  __               
-- /__  _. ._ _   _  
-- \_| (_| | | | (/_ 

ClientVersion 		= "5372"
