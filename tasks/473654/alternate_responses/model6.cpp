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
    Packet(uint8_t packetType, uint32_t sequenceNumber, uint8_t* payload, uint32_t payloadLength) {
        this->magicNumber = 0x1234;  // A unique identifier for the packet
        this->packetType = packetType;
        this->sequenceNumber = sequenceNumber;
        this->payloadLength = payloadLength;
        this->payload = new uint8_t[payloadLength];
        std::memcpy(this->payload, payload, payloadLength);
    }

    // Destructor to prevent memory leaks
    ~Packet() {
        delete[] payload;
    }

    // Serialize the packet into a byte array
    uint8_t* serialize(uint32_t& length) {
        length = sizeof(magicNumber) + sizeof(packetType) + sizeof(sequenceNumber) + sizeof(payloadLength) + payloadLength;
        uint8_t* data = new uint8_t[length];
        uint8_t* ptr = data;
        std::memcpy(ptr, &magicNumber, sizeof(magicNumber));
        ptr += sizeof(magicNumber);
        std::memcpy(ptr, &packetType, sizeof(packetType));
        ptr += sizeof(packetType);
        std::memcpy(ptr, &sequenceNumber, sizeof(sequenceNumber));
        ptr += sizeof(sequenceNumber);
        std::memcpy(ptr, &payloadLength, sizeof(payloadLength));
        ptr += sizeof(payloadLength);
        std::memcpy(ptr, payload, payloadLength);
        return data;
    }

    // Deserialize a byte array into a packet
    static Packet* deserialize(uint8_t* data, uint32_t length) {
        if (length < sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t)) {
            return nullptr;
        }
        uint16_t magicNumber;
        uint8_t packetType;
        uint32_t sequenceNumber;
        uint32_t payloadLength;
        uint8_t* ptr = data;
        std::memcpy(&magicNumber, ptr, sizeof(magicNumber));
        ptr += sizeof(magicNumber);
        std::memcpy(&packetType, ptr, sizeof(packetType));
        ptr += sizeof(packetType);
        std::memcpy(&sequenceNumber, ptr, sizeof(sequenceNumber));
        ptr += sizeof(sequenceNumber);
        std::memcpy(&payloadLength, ptr, sizeof(payloadLength));
        ptr += sizeof(payloadLength);
        if (length != sizeof(magicNumber) + sizeof(packetType) + sizeof(sequenceNumber) + sizeof(payloadLength) + payloadLength) {
            return nullptr;
        }
        return new Packet(packetType, sequenceNumber, ptr, payloadLength);
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
            std::lock_guard<std::mutex> lock(mtx);
            packetQueue.push(packet);
            cv.notify_one();
        }
    }

    // Start the protocol
    void start() {
        std::thread([this]() {
            while (true) {
                Packet* packet;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { return !packetQueue.empty(); });
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