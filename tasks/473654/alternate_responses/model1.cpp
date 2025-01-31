#include <iostream>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <stdexcept>

class Packet {
public:
    static const uint16_t MAGIC_NUMBER = 0xAABB;
    uint16_t magicNumber;
    uint8_t packetType;
    uint32_t sequenceNumber;
    uint32_t payloadLength;
    uint8_t* payload;

    Packet(uint8_t packetType, uint32_t sequenceNumber, uint8_t* payload, uint32_t payloadLength) 
        : magicNumber(MAGIC_NUMBER), 
          packetType(packetType), 
          sequenceNumber(sequenceNumber), 
          payloadLength(payloadLength) {
        if (payloadLength > 0 && payload != nullptr) {
            this->payload = new uint8_t[payloadLength];
            std::memcpy(this->payload, payload, payloadLength);
        } else {
            this->payload = nullptr;
            this->payloadLength = 0;
        }
    }

    ~Packet() {
        delete[] payload;
    }

    uint8_t* serialize(uint32_t& length) {
        length = sizeof(magicNumber) + sizeof(packetType) + 
                sizeof(sequenceNumber) + sizeof(payloadLength) + payloadLength;
        
        uint8_t* data = new uint8_t[length];
        uint32_t offset = 0;

        std::memcpy(data + offset, &magicNumber, sizeof(magicNumber));
        offset += sizeof(magicNumber);

        std::memcpy(data + offset, &packetType, sizeof(packetType));
        offset += sizeof(packetType);

        std::memcpy(data + offset, &sequenceNumber, sizeof(sequenceNumber));
        offset += sizeof(sequenceNumber);

        std::memcpy(data + offset, &payloadLength, sizeof(payloadLength));
        offset += sizeof(payloadLength);

        if (payloadLength > 0 && payload != nullptr) {
            std::memcpy(data + offset, payload, payloadLength);
        }

        return data;
    }

    static Packet* deserialize(uint8_t* data, uint32_t length) {
        if (data == nullptr || length < sizeof(uint16_t) + sizeof(uint8_t) + 
            sizeof(uint32_t) + sizeof(uint32_t)) {
            return nullptr;
        }

        uint32_t offset = 0;
        uint16_t magicNumber;
        std::memcpy(&magicNumber, data + offset, sizeof(magicNumber));
        if (magicNumber != MAGIC_NUMBER) {
            return nullptr;
        }
        offset += sizeof(magicNumber);

        uint8_t packetType;
        std::memcpy(&packetType, data + offset, sizeof(packetType));
        offset += sizeof(packetType);

        uint32_t sequenceNumber;
        std::memcpy(&sequenceNumber, data + offset, sizeof(sequenceNumber));
        offset += sizeof(sequenceNumber);

        uint32_t payloadLength;
        std::memcpy(&payloadLength, data + offset, sizeof(payloadLength));
        offset += sizeof(payloadLength);

        if (length != offset + payloadLength) {
            return nullptr;
        }

        uint8_t* payload = nullptr;
        if (payloadLength > 0) {
            payload = new uint8_t[payloadLength];
            std::memcpy(payload, data + offset, payloadLength);
        }

        return new Packet(packetType, sequenceNumber, payload, payloadLength);
    }
};

class Protocol {
public:
    Protocol() = default;
    ~Protocol() {
        while (!packetQueue.empty()) {
            delete packetQueue.front();
            packetQueue.pop();
        }
    }

    void sendPacket(Packet* packet) {
        if (!packet) return;

        try {
            uint32_t length;
            uint8_t* data = packet->serialize(length);
            
            // Simulate network send
            std::cout << "Sending packet with sequence number " << packet->sequenceNumber << std::endl;
            
            delete[] data;
            delete packet;
        } catch (const std::exception& e) {
            std::cerr << "Error sending packet: " << e.what() << std::endl;
            delete packet;
        }
    }

    void receivePacket(uint8_t* data, uint32_t length) {
        if (!data || length == 0) return;

        Packet* packet = Packet::deserialize(data, length);
        if (packet != nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            packetQueue.push(packet);
            cv.notify_one();
        }
    }

    void start() {
        std::thread([this]() {
            while (true) {
                Packet* packet = nullptr;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { return !packetQueue.empty(); });
                    
                    packet = packetQueue.front();
                    packetQueue.pop();
                }

                if (packet) {
                    try {
                        processPacket(packet);
                    } catch (const std::exception& e) {
                        std::cerr << "Error processing packet: " << e.what() << std::endl;
                    }
                    delete packet;
                }
            }
        }).detach();
    }

private:
    void processPacket(Packet* packet) {
        if (!packet) throw std::runtime_error("Invalid packet");
        std::cout << "Processing packet with sequence number " << packet->sequenceNumber << std::endl;
    }

    std::mutex mtx;
    std::condition_variable cv;
    std::queue<Packet*> packetQueue;
};
