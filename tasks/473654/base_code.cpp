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
        //ToDo: Initilize the member functions
    }

    // Destructor to prevent memory leaks
    ~Packet() {
        //ToDo : Delete the payload
    }

    // Serialize the packet into a byte array
    uint8_t* serialize(uint32_t& length) {
        //ToDO: Implement the serialize support code.
        return data;
    }

    // Deserialize a byte array into a packet
    static Packet* deserialize(uint8_t* data, uint32_t length) {
       //ToDo: Deserialize need to take case edge and corner cases, also positive and negative cases.
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
        //ToDo: implement the missing initialization
        // Simulate network send
        std::cout << "Sending packet with sequence number " << packet->sequenceNumber << std::endl;
        delete[] data;
        delete packet;
    }

    // Receive a packet
    void receivePacket(uint8_t* data, uint32_t length) {
        Packet* packet = Packet::deserialize(data, length);
        if (packet != nullptr) {
         //ToDo: implement the missing code after recviing the package
        }
    }

    // Start the protocol
    void start() {
        std::thread([this]() {
            while (true) {
                //ToDo: implement multithreading support with creashing the application
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