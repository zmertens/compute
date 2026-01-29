#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <SFML/Network.hpp>

// Network packet types
enum class PacketType : uint8_t
{
    PLAYER_JOIN = 0,
    PLAYER_LEAVE = 1,
    PLAYER_STATE = 2,
    SCORE_UPDATE = 3,
    GAME_STATE = 4
};

// Player state packet for frequent position updates
struct PlayerStatePacket
{
    uint32_t playerId;
    float posX, posY, posZ;
    float velX, velY, velZ;
    float rotX, rotY, rotZ;
    uint32_t score;
    uint64_t timestamp;
};

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();
    
    // Connection management
    bool startServer(uint16_t port);
    bool connectToServer(const std::string& serverAddress, uint16_t port);
    void disconnect();
    bool isConnected() const;
    
    // Packet sending
    void sendPlayerState(const PlayerStatePacket& packet);
    void sendScoreUpdate(uint32_t playerId, uint32_t score);
    void sendPlayerJoin(uint32_t playerId);
    void sendPlayerLeave(uint32_t playerId);
    
    // Packet receiving
    void update(float dt);
    bool hasPlayerStateUpdates() const;
    PlayerStatePacket getNextPlayerStateUpdate();
    
    // Configuration
    void setLocalPlayerId(uint32_t id);
    uint32_t getLocalPlayerId() const;
    
    // Server/Client mode
    bool isServer() const;
    
private:
    sf::UdpSocket mUdpSocket;
    sf::TcpSocket mTcpSocket;
    sf::TcpListener mTcpListener;
    
    bool mIsServer;
    bool mIsConnected;
    uint32_t mLocalPlayerId;
    uint16_t mPort;
    std::string mServerAddress;
    
    std::vector<PlayerStatePacket> mPendingPlayerStates;
    std::vector<sf::TcpSocket*> mClientSockets;
    
    void processUdpPackets();
    void processTcpPackets();
    void acceptNewConnections();
    void broadcastToClients(sf::Packet& packet);
};

#endif // NETWORKMANAGER_HPP
