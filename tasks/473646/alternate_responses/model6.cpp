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

unsigned int calculateCRC32(const std::vector<unsigned char> &data, unsigned int polynomial)
{
    unsigned int crc = 0xFFFFFFFF;
    for (const auto &byte : data)
    {
        crc ^= byte;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ polynomial;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

std::map<std::string, std::string> parseEthernetPacket(const std::vector<unsigned char> &packet)
{
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
        unsigned int crc = *reinterpret_cast<const unsigned int *>(&packet[packet.size() - 4]);
        parsedFields["CRC"] = "0x" + std::to_string(ntohl(crc));

        // Calculate CRC32 for validation
        std::vector<unsigned char> data(packet.begin(), packet.begin() + packet.size() - 4);
        unsigned int calculatedCRC = calculateCRC32(data, 0x04C11DB7);
        if (calculatedCRC == ntohl(crc))
        {
            parsedFields["CRC Validation"] = "Valid";
        }
        else
        {
            parsedFields["CRC Validation"] = "Invalid";
        }
    }

    return parsedFields;
}
