#include <iostream>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <memory>
#include <stdexcept>

class Packet {
public:
    static const uint16_t MAGIC = 0xABCD;  // Magic number for packet validation
    uint16_t magicNumber;
    uint8_t packetType;
    uint32_t sequenceNumber;
    uint32_t payloadLength;
    uint8_t* payload;

    Packet(uint8_t type, uint32_t seqNum, uint8_t* data, uint32_t length) 
        : magicNumber(MAGIC), packetType(type), sequenceNumber(seqNum), payloadLength(length) {
        if (length > 0 && data != nullptr) {
            payload = new uint8_t[length];
            std::memcpy(payload, data, length);
        } else {
            payload = nullptr;
            payloadLength = 0;
        }
    }

    ~Packet() {
        delete[] payload;
    }

    // Prevent copying
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

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
        if (!data || length < sizeof(uint16_t) + sizeof(uint8_t) + 
            sizeof(uint32_t) + sizeof(uint32_t)) {
            return nullptr;
        }

        uint32_t offset = 0;
        
        uint16_t magic;
        std::memcpy(&magic, data + offset, sizeof(magic));
        if (magic != MAGIC) return nullptr;
        offset += sizeof(magic);

        uint8_t packetType;
        std::memcpy(&packetType, data + offset, sizeof(packetType));
        offset += sizeof(packetType);

        uint32_t sequenceNumber;
        std::memcpy(&sequenceNumber, data + offset, sizeof(sequenceNumber));
        offset += sizeof(sequenceNumber);

        uint32_t payloadLength;
        std::memcpy(&payloadLength, data + offset, sizeof(payloadLength));
        offset += sizeof(payloadLength);

        if (length != offset + payloadLength) return nullptr;

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
    Protocol() : running(false) {}
    ~Protocol() {
        stop();
    }

    std::mutex mtx;
    std::condition_variable cv;
    std::queue<Packet*> packetQueue;
    bool running;

    void sendPacket(Packet* packet) {
        if (!packet) return;

        uint32_t length;
        uint8_t* data = packet->serialize(length);
        
        try {
            // Simulate network send
            std::cout << "Sending packet with sequence number " << packet->sequenceNumber << std::endl;
        } catch (...) {
            delete[] data;
            delete packet;
            throw;
        }

        delete[] data;
        delete packet;
    }

    void receivePacket(uint8_t* data, uint32_t length) {
        Packet* packet = Packet::deserialize(data, length);
        if (packet != nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            packetQueue.push(packet);
            cv.notify_one();
        }
    }

    void start() {
        running = true;
        std::thread([this]() {
            while (running) {
                Packet* packet = nullptr;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { 
                        return !packetQueue.empty() || !running; 
                    });
                    
                    if (!running) break;
                    
                    if (!packetQueue.empty()) {
                        packet = packetQueue.front();
                        packetQueue.pop();
                    }
                }
                
                if (packet) {
                    try {
                        processPacket(packet);
                    } catch (...) {
                        delete packet;
                        throw;
                    }
                    delete packet;
                }
            }
        }).detach();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            running = false;
        }
        cv.notify_all();
        
        // Clean up remaining packets
        std::lock_guard<std::mutex> lock(mtx);
        while (!packetQueue.empty()) {
            delete packetQueue.front();
            packetQueue.pop();
        }
    }

private:
    void processPacket(Packet* packet) {
        if (!packet) return;
        std::cout << "Processing packet with sequence number " << packet->sequenceNumber << std::endl;
    }
};