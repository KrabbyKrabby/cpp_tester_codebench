#include <iostream>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

// Packet class
class Packet {
public:
    uint16_t magicNumber;
    uint8_t packetType;
    uint32_t sequenceNumber;
    uint32_t payloadLength;
    uint8_t* payload;

    // Constructor
    Packet(uint8_t packetType, uint32_t sequenceNumber, uint8_t* payload, uint32_t payloadLength) 
        : packetType(packetType), sequenceNumber(sequenceNumber), payloadLength(payloadLength) {
        this->payload = new uint8_t[payloadLength];
        std::memcpy(this->payload, payload, payloadLength);
    }

    // Destructor to prevent memory leaks
    ~Packet() {
        delete[] payload;
    }

    // Serialize the packet into a byte array
    uint8_t* serialize(uint32_t& length) {
        uint8_t* data = new uint8_t[10 + payloadLength];  // 10 bytes for header
        std::memcpy(data, &magicNumber, 2);  // copy magic number
        std::memcpy(data + 2, &packetType, 1);  // copy packet type
        std::memcpy(data + 3, &sequenceNumber, 4);  // copy sequence number
        std::memcpy(data + 7, &payloadLength, 4);  // copy payload length
        std::memcpy(data + 11, payload, payloadLength);  // copy payload
        length = 11 + payloadLength;
        return data;
    }

    // Deserialize a byte array into a packet
    static Packet* deserialize(uint8_t* data, uint32_t length) {
        if (length < 11) {
            return nullptr;  // invalid packet
        }
        uint8_t packetType;
        uint32_t sequenceNumber;
        uint32_t payloadLength;
        std::memcpy(&packetType, data + 2, 1);
        std::memcpy(&sequenceNumber, data + 3, 4);
        std::memcpy(&payloadLength, data + 7, 4);
        if (length != 11 + payloadLength) {
            return nullptr;  // invalid packet
        }
        uint8_t* payload = new uint8_t[payloadLength];
        std::memcpy(payload, data + 11, payloadLength);
        return new Packet(packetType, sequenceNumber, payload, payloadLength);
    }
};

// Protocol class
class Protocol {
public:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<Packet*> packetQueue;

    // Send a packet
    void sendPacket(Packet* packet) {
        uint32_t length;
        uint8_t* data = packet->serialize(length);
        // Simulate network send
        std::cout << "Sending packet with sequence number " << packet->sequenceNumber << std::endl;
        delete[] data;
        delete packet;
    }

    // Receive a packet
    void receivePacket(uint8_t* data, uint32_t length) {
        Packet* packet = Packet::deserialize(data, length);
        if (packet != nullptr) {
            // Add packet to queue
            std::lock_guard<std::mutex> lock(mtx);
            packetQueue.push(packet);
            cv.notify_all();
        }
    }

    // Start the protocol
    void start() {
        std::thread([this]() {
            while (true) {
                Packet* packet;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]{ return !packetQueue.empty(); });
                    packet = packetQueue.front();
                    packetQueue.pop();
                }
                processPacket(packet);
                delete packet;
            }
        }).detach();
    }

    // Process a packet
    void processPacket(Packet* packet) {
        // Simulate packet processing
        std::cout << "Processing packet with sequence number " << packet->sequenceNumber << std::endl;
    }
};