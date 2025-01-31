#include <iostream>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

class Packet {
public:
    uint16_t magicNumber;
    uint8_t packetType;
    uint32_t sequenceNumber;
    uint32_t payloadLength;
    uint8_t* payload;

    Packet(uint8_t type, uint32_t seqNum, uint8_t* data, uint32_t length) 
        : magicNumber(0xCAFE), 
          packetType(type), 
          sequenceNumber(seqNum), 
          payloadLength(length) {
        payload = new uint8_t[length];
        std::memcpy(payload, data, length);
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
        
        std::memcpy(data + offset, payload, payloadLength);
        
        return data;
    }

    static Packet* deserialize(uint8_t* data, uint32_t length) {
        if (length < sizeof(uint16_t) + sizeof(uint8_t) + 
            sizeof(uint32_t) + sizeof(uint32_t)) {
            return nullptr;
        }

        uint32_t offset = 0;
        uint16_t magic;
        std::memcpy(&magic, data + offset, sizeof(uint16_t));
        if (magic != 0xCAFE) return nullptr;
        offset += sizeof(uint16_t);

        uint8_t packetType;
        std::memcpy(&packetType, data + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        uint32_t sequenceNumber;
        std::memcpy(&sequenceNumber, data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        uint32_t payloadLength;
        std::memcpy(&payloadLength, data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        if (length != offset + payloadLength) return nullptr;

        uint8_t* payload = new uint8_t[payloadLength];
        std::memcpy(payload, data + offset, payloadLength);

        return new Packet(packetType, sequenceNumber, payload, payloadLength);
    }
};

class Protocol {
public:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<Packet*> packetQueue;
    bool running;

    Protocol() : running(true) {}

    void sendPacket(Packet* packet) {
        if (!packet) return;

        uint32_t length;
        uint8_t* data = packet->serialize(length);
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Sending packet with sequence number " << packet->sequenceNumber << std::endl;
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
        std::thread([this]() {
            while (running) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this]() { return !packetQueue.empty() || !running; });
                
                if (!running) break;

                if (!packetQueue.empty()) {
                    Packet* packet = packetQueue.front();
                    packetQueue.pop();
                    lock.unlock();
                    
                    processPacket(packet);
                    delete packet;
                }
            }
        }).detach();
    }

    void stop() {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
        cv.notify_all();
    }

    void processPacket(Packet* packet) {
        if (!packet) return;
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Processing packet with sequence number " << packet->sequenceNumber << std::endl;
    }

    ~Protocol() {
        stop();
        std::lock_guard<std::mutex> lock(mtx);
        while (!packetQueue.empty()) {
            delete packetQueue.front();
            packetQueue.pop();
        }
    }
};
