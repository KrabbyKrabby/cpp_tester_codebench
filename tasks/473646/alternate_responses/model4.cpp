#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <arpa/inet.h>
#include <sstream>

struct EthernetHeader
{
    unsigned char destMac[6];
    unsigned char srcMac[6];
    unsigned short etherType;
};

struct IPv4Header
{
    unsigned char versionIHL;
    unsigned char typeOfService;
    unsigned short totalLength;
    unsigned short identification;
    unsigned short flagsFragmentOffset;
    unsigned char timeToLive;
    unsigned char protocol;
    unsigned short headerChecksum;
    unsigned int srcIp;
    unsigned int destIp;
};

struct TCPHeader
{
    unsigned short srcPort;
    unsigned short destPort;
    unsigned int sequenceNumber;
    unsigned int acknowledgmentNumber;
    unsigned char dataOffsetReserved;
    unsigned char flags;
    unsigned short windowSize;
    unsigned short checksum;
    unsigned short urgentPointer;
};

// CRC32 lookup table
uint32_t crc32_table[256];

// Initialize the CRC32 lookup table
void init_crc32_table() {
    const uint32_t polynomial = 0x04C11DB7;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i << 24;
        for (int j = 0; j < 8; j++) {
            if (c & 0x80000000) {
                c = (c << 1) ^ polynomial;
            } else {
                c = (c << 1);
            }
        }
        crc32_table[i] = c;
    }
}

// Calculate CRC32
uint32_t calculate_crc32(const unsigned char* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (size_t i = 0; i < length; i++) {
        uint8_t pos = (crc >> 24) ^ data[i];
        crc = (crc << 8) ^ crc32_table[pos];
    }
    
    return crc;
}

std::string macToString(const unsigned char *mac)
{
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i)
    {
        if (i != 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)mac[i];
    }
    return oss.str();
}

std::string ipToString(unsigned int ip)
{
    return std::to_string((ip >> 24) & 0xFF) + "." +
           std::to_string((ip >> 16) & 0xFF) + "." +
           std::to_string((ip >> 8) & 0xFF) + "." +
           std::to_string(ip & 0xFF);
}

std::map<std::string, std::string> parseEthernetPacket(const std::vector<unsigned char> &packet)
{
    static bool crc_table_initialized = false;
    if (!crc_table_initialized) {
        init_crc32_table();
        crc_table_initialized = true;
    }

    std::map<std::string, std::string> parsedFields;

    if (packet.size() < sizeof(EthernetHeader))
    {
        parsedFields["Error"] = "Packet is too short to contain an Ethernet header.";
        return parsedFields;
    }

    const EthernetHeader *ethHeader = reinterpret_cast<const EthernetHeader *>(packet.data());
    parsedFields["Destination MAC"] = macToString(ethHeader->destMac);
    parsedFields["Source MAC"] = macToString(ethHeader->srcMac);
    parsedFields["EtherType"] = "0x" + std::to_string(ntohs(ethHeader->etherType));

    if (ntohs(ethHeader->etherType) != 0x0800)
    {
        parsedFields["Error"] = "Unsupported EtherType. Only IPv4 is supported.";
        return parsedFields;
    }

    if (packet.size() < sizeof(EthernetHeader) + sizeof(IPv4Header))
    {
        parsedFields["Error"] = "Packet is too short to contain an IPv4 header.";
        return parsedFields;
    }

    const IPv4Header *ipHeader = reinterpret_cast<const IPv4Header *>(packet.data() + sizeof(EthernetHeader));
    parsedFields["Source IP"] = ipToString(ntohl(ipHeader->srcIp));
    parsedFields["Destination IP"] = ipToString(ntohl(ipHeader->destIp));
    parsedFields["Protocol"] = std::to_string(ipHeader->protocol);

    if (ipHeader->protocol != 6)
    {
        parsedFields["Error"] = "Unsupported Protocol. Only TCP is supported.";
        return parsedFields;
    }

    if (packet.size() < sizeof(EthernetHeader) + sizeof(IPv4Header) + sizeof(TCPHeader))
    {
        parsedFields["Error"] = "Packet is too short to contain a TCP header.";
        return parsedFields;
    }

    const TCPHeader *tcpHeader = reinterpret_cast<const TCPHeader *>(
        packet.data() + sizeof(EthernetHeader) + sizeof(IPv4Header));
    parsedFields["Source Port"] = std::to_string(ntohs(tcpHeader->srcPort));
    parsedFields["Destination Port"] = std::to_string(ntohs(tcpHeader->destPort));

    // Extract payload and CRC if present
    size_t headerSize = sizeof(EthernetHeader) + sizeof(IPv4Header) + sizeof(TCPHeader);
    size_t payloadSize = packet.size() - headerSize - 4; // Excluding CRC (4 bytes)

    if (payloadSize > 0)
    {
        parsedFields["Payload Size"] = std::to_string(payloadSize) + " bytes";
    }
    else
    {
        parsedFields["Payload Size"] = "No payload.";
    }

    if (packet.size() >= headerSize + 4)
    {
        unsigned int received_crc = *reinterpret_cast<const unsigned int *>(&packet[packet.size() - 4]);
        received_crc = ntohl(received_crc);
        parsedFields["CRC"] = "0x" + std::to_string(received_crc);

        // Calculate CRC32 for the packet data (excluding the CRC field)
        uint32_t calculated_crc = calculate_crc32(packet.data(), packet.size() - 4);
        
        // Compare calculated CRC with received CRC
        parsedFields["CRC Validation"] = (calculated_crc == received_crc) ? "Valid" : "Invalid";
    }

    return parsedFields;
}
