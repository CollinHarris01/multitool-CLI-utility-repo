#ifndef SHA256_H
#define SHA256_H

#include <cstdint>
#include <string>
#include <vector>

class SHA256 {
public:
    SHA256();
    void update(const uint8_t* data, size_t length);
    void update(const std::vector<uint8_t>& data);
    void finalize();

    // 32-byte binary digest
    const uint8_t* digest() const { return m_digest; }

    // Hex string version (lowercase)
    std::string hexDigest() const;

private:
    void transform(const uint8_t* chunk);

    uint8_t  m_data[64];       // Input buffer
    uint32_t m_dataLength;     // Number of bytes currently in buffer
    uint64_t m_bitLength;      // Total number of bits processed

    uint32_t m_state[8];       // Hash state
    uint8_t  m_digest[32];     // Final 32-byte digest (after finalize)
};

#endif