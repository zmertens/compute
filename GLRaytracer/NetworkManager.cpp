#include "NetworkManager.hpp"
#include <iostream>

NetworkManager::NetworkManager()
    : mIsServer(false)
    , mIsConnected(false)
    , mLocalPlayerId(0)
    , mPort(0)
{
    // Set UDP socket to non-blocking mode
    mUdpSocket.setBlocking(false);
}

NetworkManager::~NetworkManager()
{
    disconnect();
    
    // Clean up client sockets
    for (auto* socket : mClientSockets)
    {
        delete socket;
    }
    mClientSockets.clear();
}

bool NetworkManager::startServer(uint16_t port)
{
    mPort = port;
    mIsServer = true;
    
    // Bind UDP socket
    if (mUdpSocket.bind(port) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind UDP socket to port " << port << std::endl;
        return false;
    }
    
    // Listen on TCP socket for reliable messages
    if (mTcpListener.listen(port) != sf::Socket::Done)
    {
        std::cerr << "Failed to listen on TCP port " << port << std::endl;
        return false;
    }
    
    mTcpListener.setBlocking(false);
    mIsConnected = true;
    
    std::cout << "Server started on port " << port << std::endl;
    return true;
}

bool NetworkManager::connectToServer(const std::string& serverAddress, uint16_t port)
{
    mServerAddress = serverAddress;
    mPort = port;
    mIsServer = false;
    
    // Bind UDP socket to any available port
    if (mUdpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind UDP socket" << std::endl;
        return false;
    }
    
    // Connect to server via TCP
    sf::IpAddress serverIp(serverAddress);
    if (mTcpSocket.connect(serverIp, port) != sf::Socket::Done)
    {
        std::cerr << "Failed to connect to server at " << serverAddress << ":" << port << std::endl;
        return false;
    }
    
    mTcpSocket.setBlocking(false);
    mIsConnected = true;
    
    std::cout << "Connected to server at " << serverAddress << ":" << port << std::endl;
    return true;
}

void NetworkManager::disconnect()
{
    if (mIsConnected)
    {
        if (!mIsServer)
        {
            // Send leave message before disconnecting
            sendPlayerLeave(mLocalPlayerId);
            mTcpSocket.disconnect();
        }
        else
        {
            mTcpListener.close();
        }
        
        mUdpSocket.unbind();
        mIsConnected = false;
    }
}

bool NetworkManager::isConnected() const
{
    return mIsConnected;
}

void NetworkManager::sendPlayerState(const PlayerStatePacket& packet)
{
    if (!mIsConnected) return;
    
    sf::Packet sfPacket;
    sfPacket << static_cast<uint8_t>(PacketType::PLAYER_STATE);
    sfPacket << packet.playerId;
    sfPacket << packet.posX << packet.posY << packet.posZ;
    sfPacket << packet.velX << packet.velY << packet.velZ;
    sfPacket << packet.rotX << packet.rotY << packet.rotZ;
    sfPacket << packet.score;
    sfPacket << packet.timestamp;
    
    if (mIsServer)
    {
        // Broadcast to all clients
        broadcastToClients(sfPacket);
    }
    else
    {
        // Send to server
        sf::IpAddress serverIp(mServerAddress);
        mUdpSocket.send(sfPacket, serverIp, mPort);
    }
}

void NetworkManager::sendScoreUpdate(uint32_t playerId, uint32_t score)
{
    if (!mIsConnected) return;
    
    sf::Packet packet;
    packet << static_cast<uint8_t>(PacketType::SCORE_UPDATE);
    packet << playerId << score;
    
    if (mIsServer)
    {
        broadcastToClients(packet);
    }
    else
    {
        mTcpSocket.send(packet);
    }
}

void NetworkManager::sendPlayerJoin(uint32_t playerId)
{
    if (!mIsConnected) return;
    
    sf::Packet packet;
    packet << static_cast<uint8_t>(PacketType::PLAYER_JOIN);
    packet << playerId;
    
    if (mIsServer)
    {
        broadcastToClients(packet);
    }
    else
    {
        mTcpSocket.send(packet);
    }
}

void NetworkManager::sendPlayerLeave(uint32_t playerId)
{
    if (!mIsConnected) return;
    
    sf::Packet packet;
    packet << static_cast<uint8_t>(PacketType::PLAYER_LEAVE);
    packet << playerId;
    
    if (mIsServer)
    {
        broadcastToClients(packet);
    }
    else
    {
        mTcpSocket.send(packet);
    }
}

void NetworkManager::update(float dt)
{
    if (!mIsConnected) return;
    
    if (mIsServer)
    {
        acceptNewConnections();
    }
    
    processUdpPackets();
    processTcpPackets();
}

bool NetworkManager::hasPlayerStateUpdates() const
{
    return !mPendingPlayerStates.empty();
}

PlayerStatePacket NetworkManager::getNextPlayerStateUpdate()
{
    if (mPendingPlayerStates.empty())
    {
        return PlayerStatePacket();
    }
    
    PlayerStatePacket packet = mPendingPlayerStates.front();
    mPendingPlayerStates.erase(mPendingPlayerStates.begin());
    return packet;
}

void NetworkManager::setLocalPlayerId(uint32_t id)
{
    mLocalPlayerId = id;
}

uint32_t NetworkManager::getLocalPlayerId() const
{
    return mLocalPlayerId;
}

bool NetworkManager::isServer() const
{
    return mIsServer;
}

void NetworkManager::processUdpPackets()
{
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short senderPort;
    
    while (mUdpSocket.receive(packet, sender, senderPort) == sf::Socket::Done)
    {
        uint8_t packetType;
        packet >> packetType;
        
        if (static_cast<PacketType>(packetType) == PacketType::PLAYER_STATE)
        {
            PlayerStatePacket statePacket;
            packet >> statePacket.playerId;
            packet >> statePacket.posX >> statePacket.posY >> statePacket.posZ;
            packet >> statePacket.velX >> statePacket.velY >> statePacket.velZ;
            packet >> statePacket.rotX >> statePacket.rotY >> statePacket.rotZ;
            packet >> statePacket.score;
            packet >> statePacket.timestamp;
            
            mPendingPlayerStates.push_back(statePacket);
            
            // If server, relay to other clients
            if (mIsServer)
            {
                sf::Packet relayPacket;
                relayPacket << static_cast<uint8_t>(PacketType::PLAYER_STATE);
                relayPacket << statePacket.playerId;
                relayPacket << statePacket.posX << statePacket.posY << statePacket.posZ;
                relayPacket << statePacket.velX << statePacket.velY << statePacket.velZ;
                relayPacket << statePacket.rotX << statePacket.rotY << statePacket.rotZ;
                relayPacket << statePacket.score;
                relayPacket << statePacket.timestamp;
                
                broadcastToClients(relayPacket);
            }
        }
    }
}

void NetworkManager::processTcpPackets()
{
    if (mIsServer)
    {
        // Process packets from all connected clients
        for (auto* clientSocket : mClientSockets)
        {
            sf::Packet packet;
            if (clientSocket->receive(packet) == sf::Socket::Done)
            {
                uint8_t packetType;
                packet >> packetType;
                
                // Handle different packet types
                // For now, just relay to other clients
                broadcastToClients(packet);
            }
        }
    }
    else
    {
        // Process packets from server
        sf::Packet packet;
        if (mTcpSocket.receive(packet) == sf::Socket::Done)
        {
            uint8_t packetType;
            packet >> packetType;
            
            // Handle server messages
            // Implementation depends on packet type
        }
    }
}

void NetworkManager::acceptNewConnections()
{
    sf::TcpSocket* newClient = new sf::TcpSocket();
    
    if (mTcpListener.accept(*newClient) == sf::Socket::Done)
    {
        newClient->setBlocking(false);
        mClientSockets.push_back(newClient);
        std::cout << "New client connected. Total clients: " << mClientSockets.size() << std::endl;
    }
    else
    {
        delete newClient;
    }
}

void NetworkManager::broadcastToClients(sf::Packet& packet)
{
    for (auto* clientSocket : mClientSockets)
    {
        clientSocket->send(packet);
    }
}
