#include <gtest/gtest.h>
#include <cstring>
#include "solution.cpp"


// Positive test cases
TEST(PacketTest, SerializeAndDeserializePositive) {
    uint8_t payload[] = {0x01, 0x02, 0x03, 0x04};
    uint32_t payloadLength = sizeof(payload);

    Packet packet(0x01, 1234, payload, payloadLength);
    uint32_t serializedLength;
    uint8_t* serializedData = packet.serialize(serializedLength);

    ASSERT_NE(serializedData, nullptr);

    Packet* deserializedPacket = Packet::deserialize(serializedData, serializedLength);
    ASSERT_NE(deserializedPacket, nullptr);

    EXPECT_EQ(deserializedPacket->magicNumber, 0x1234);
    EXPECT_EQ(deserializedPacket->packetType, 0x01);
    EXPECT_EQ(deserializedPacket->sequenceNumber, 1234);
    EXPECT_EQ(deserializedPacket->payloadLength, payloadLength);
    EXPECT_EQ(std::memcmp(deserializedPacket->payload, payload, payloadLength), 0);

    delete[] serializedData;
    delete deserializedPacket;
}

// Negative test cases
TEST(PacketTest, DeserializeInvalidData) {
    uint8_t invalidData[] = {0x01, 0x02}; // Insufficient data for a packet
    uint32_t invalidLength = sizeof(invalidData);

    Packet* deserializedPacket = Packet::deserialize(invalidData, invalidLength);
    EXPECT_EQ(deserializedPacket, nullptr);
}

// Corner and edge cases
TEST(PacketTest, ZeroLengthPayload) {
    uint8_t payload[] = {};
    uint32_t payloadLength = 0;

    Packet packet(0x01, 5678, payload, payloadLength);
    uint32_t serializedLength;
    uint8_t* serializedData = packet.serialize(serializedLength);

    ASSERT_NE(serializedData, nullptr);

    Packet* deserializedPacket = Packet::deserialize(serializedData, serializedLength);
    ASSERT_NE(deserializedPacket, nullptr);

    EXPECT_EQ(deserializedPacket->payloadLength, 0);

    delete[] serializedData;
    delete deserializedPacket;
}

// Maximum and minimum values
TEST(PacketTest, MaxValues) {
    uint8_t payload[255];
    std::fill(payload, payload + sizeof(payload), 0xFF);
    uint32_t payloadLength = sizeof(payload);

    Packet packet(0xFF, UINT32_MAX, payload, payloadLength);
    uint32_t serializedLength;
    uint8_t* serializedData = packet.serialize(serializedLength);

    ASSERT_NE(serializedData, nullptr);

    Packet* deserializedPacket = Packet::deserialize(serializedData, serializedLength);
    ASSERT_NE(deserializedPacket, nullptr);

    EXPECT_EQ(deserializedPacket->magicNumber, 0x1234);
    EXPECT_EQ(deserializedPacket->packetType, 0xFF);
    EXPECT_EQ(deserializedPacket->sequenceNumber, UINT32_MAX);
    EXPECT_EQ(deserializedPacket->payloadLength, payloadLength);
    EXPECT_EQ(std::memcmp(deserializedPacket->payload, payload, payloadLength), 0);

    delete[] serializedData;
    delete deserializedPacket;
}

TEST(PacketTest, MinValues) {
    uint8_t payload[] = {};
    uint32_t payloadLength = 0;

    Packet packet(0x00, 0, payload, payloadLength);
    uint32_t serializedLength;
    uint8_t* serializedData = packet.serialize(serializedLength);

    ASSERT_NE(serializedData, nullptr);

    Packet* deserializedPacket = Packet::deserialize(serializedData, serializedLength);
    ASSERT_NE(deserializedPacket, nullptr);

    EXPECT_EQ(deserializedPacket->magicNumber, 0x1234);
    EXPECT_EQ(deserializedPacket->packetType, 0x00);
    EXPECT_EQ(deserializedPacket->sequenceNumber, 0);
    EXPECT_EQ(deserializedPacket->payloadLength, 0);

    delete[] serializedData;
    delete deserializedPacket;
}

// Protocol integration tests
TEST(ProtocolTest, SendAndReceivePacket) {
    Protocol protocol;
    protocol.start();

    uint8_t payload[] = {0x0A, 0x0B, 0x0C};
    uint32_t payloadLength = sizeof(payload);
    Packet* packet = new Packet(0x02, 4321, payload, payloadLength);

    uint32_t serializedLength;
    uint8_t* serializedData = packet->serialize(serializedLength);

    protocol.receivePacket(serializedData, serializedLength);

    // Verify packet processing
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Allow thread to process

    delete[] serializedData;
}