#include "solution.cpp"    // Include the wrapper file dynamically
#include <gtest/gtest.h> // Include GoogleTest

TEST(ParseEthernetPacketTest, ValidPacketTest)
{
    unsigned char dummyPacket[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // Destination MAC
        0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, // Source MAC
        0x08, 0x00,                         // EtherType: IPv4
        // IPv4 Header
        0x45, 0x00, 0x00, 0x3C, 0x1C, 0x46, 0x40, 0x00, 0x40, 0x06, 0xB1, 0xE6,
        0xC0, 0xA8, 0x00, 0x68, 0xC0, 0xA8, 0x00, 0x01,
        // TCP Header
        0x00, 0x50, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x50, 0x02, 0x20, 0x00, 0xE5, 0x91, 0x00, 0x00,
        // Payload (example data)
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
        // CRC
        0xFB, 0xAE, 0x01, 0x9A};

    std::vector<unsigned char> packet(dummyPacket, dummyPacket + sizeof(dummyPacket));

    auto parsedFields = parseEthernetPacket(packet);

    std::cout << "CRC: " << parsedFields["CRC"] << std::endl;
    EXPECT_EQ(parsedFields["Destination MAC"], "00:11:22:33:44:55");
    EXPECT_EQ(parsedFields["Source MAC"], "66:77:88:99:aa:bb");
    EXPECT_EQ(parsedFields["EtherType"], "0x2048");
    EXPECT_EQ(parsedFields["Source IP"], "192.168.0.104");
    EXPECT_EQ(parsedFields["Destination IP"], "192.168.0.1");
    EXPECT_EQ(parsedFields["Payload Size"], "11 bytes");
    EXPECT_EQ(parsedFields["CRC Validation"], "Valid");
}

TEST(ParseEthernetPacketTest, InvalidCRC)
{
    unsigned char dummyPacket[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // Destination MAC
        0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, // Source MAC
        0x08, 0x00,                         // EtherType: IPv4
        // IPv4 Header
        0x45, 0x00, 0x00, 0x3C, 0x1C, 0x46, 0x40, 0x00, 0x40, 0x06, 0xB1, 0xE6,
        0xC0, 0xA8, 0x00, 0x68, 0xC0, 0xA8, 0x00, 0x01,
        // TCP Header
        0x00, 0x50, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x50, 0x02, 0x20, 0x00, 0xE5, 0x91, 0x00, 0x00,
        // Payload (example data)
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
        // Invalid CRC
        0xAA, 0xBB, 0xCC, 0xDD};

    std::vector<unsigned char> packet(dummyPacket, dummyPacket + sizeof(dummyPacket));
    auto parsedFields = parseEthernetPacket(packet);

    EXPECT_EQ(parsedFields["CRC Validation"], "Invalid");
}

TEST(ParseEthernetPacketTest, EmptyPacket)
{
    std::vector<unsigned char> packet;
    auto parsedFields = parseEthernetPacket(packet);

    EXPECT_EQ(parsedFields["Error"], "Packet is too short to contain an Ethernet header.");
}

TEST(ParseEthernetPacketTest, UnsupportedEtherType)
{
    unsigned char dummyPacket[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // Destination MAC
        0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, // Source MAC
        0x08, 0x06,                         // EtherType: ARP (unsupported for this test)
        // Minimal ARP Packet Data
        0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01};

    std::vector<unsigned char> packet(dummyPacket, dummyPacket + sizeof(dummyPacket));
    auto parsedFields = parseEthernetPacket(packet);

    EXPECT_EQ(parsedFields["Error"], "Unsupported EtherType. Only IPv4 is supported.");
}

TEST(ParseEthernetPacketTest, ShortPacket)
{
    unsigned char dummyPacket[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // Destination MAC
        0x66, 0x77, 0x88, 0x99              // Incomplete Ethernet Header
    };

    std::vector<unsigned char> packet(dummyPacket, dummyPacket + sizeof(dummyPacket));
    auto parsedFields = parseEthernetPacket(packet);

    EXPECT_EQ(parsedFields["Error"], "Packet is too short to contain an Ethernet header.");
}