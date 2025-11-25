#include "../include/sha256.h"

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <vector>

// SHA-256 constants
static const uint32_t k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// Helper macros
#define ROTR(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define EP1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))


SHA256::SHA256()
{
    m_dataLength = 0;
    m_bitLength = 0;

    // Initial SHA-256 hash values
    m_state[0] = 0x6a09e667;
    m_state[1] = 0xbb67ae85;
    m_state[2] = 0x3c6ef372;
    m_state[3] = 0xa54ff53a;
    m_state[4] = 0x510e527f;
    m_state[5] = 0x9b05688c;
    m_state[6] = 0x1f83d9ab;
    m_state[7] = 0x5be0cd19;
}

void SHA256::update(const std::vector<uint8_t>& data) {
    update(data.data(), data.size());
}

void SHA256::update(const uint8_t* data, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        m_data[m_dataLength] = data[i];
        m_dataLength++;

        if (m_dataLength == 64) {
            transform(m_data);
            m_bitLength += 512;
            m_dataLength = 0;
        }
    }
}

void SHA256::finalize()
{
    uint32_t i = m_dataLength;

    // Add the 0x80 byte
    if (m_dataLength < 56) {
        m_data[i++] = 0x80;
        while (i < 56) m_data[i++] = 0x00;
    } else {
        m_data[i++] = 0x80;
        while (i < 64) m_data[i++] = 0x00;
        transform(m_data);
        std::memset(m_data, 0, 56);
    }

    // Append total bit length
    m_bitLength += m_dataLength * 8;
    m_data[63] = static_cast<uint8_t>(m_bitLength);
    m_data[62] = static_cast<uint8_t>(m_bitLength >> 8);
    m_data[61] = static_cast<uint8_t>(m_bitLength >> 16);
    m_data[60] = static_cast<uint8_t>(m_bitLength >> 24);
    m_data[59] = static_cast<uint8_t>(m_bitLength >> 32);
    m_data[58] = static_cast<uint8_t>(m_bitLength >> 40);
    m_data[57] = static_cast<uint8_t>(m_bitLength >> 48);
    m_data[56] = static_cast<uint8_t>(m_bitLength >> 56);

    transform(m_data);

    // Produce final big-endian digest
    for (int j = 0; j < 4; ++j) {
        m_digest[j     ] = (m_state[0] >> (24 - j*8)) & 0xff;
        m_digest[j +  4] = (m_state[1] >> (24 - j*8)) & 0xff;
        m_digest[j +  8] = (m_state[2] >> (24 - j*8)) & 0xff;
        m_digest[j + 12] = (m_state[3] >> (24 - j*8)) & 0xff;
        m_digest[j + 16] = (m_state[4] >> (24 - j*8)) & 0xff;
        m_digest[j + 20] = (m_state[5] >> (24 - j*8)) & 0xff;
        m_digest[j + 24] = (m_state[6] >> (24 - j*8)) & 0xff;
        m_digest[j + 28] = (m_state[7] >> (24 - j*8)) & 0xff;
    }
}

std::string SHA256::hexDigest() const
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 32; i++)
        oss << std::setw(2) << static_cast<unsigned>(m_digest[i]);
    return oss.str();
}

void SHA256::transform(const uint8_t* chunk)
{
    uint32_t w[64];

    // Prepare message schedule
    for (int i = 0; i < 16; ++i) {
        w[i]  = (chunk[i*4    ] << 24);
        w[i] |= (chunk[i*4 + 1] << 16);
        w[i] |= (chunk[i*4 + 2] << 8);
        w[i] |= (chunk[i*4 + 3]);
    }

    for (int i = 16; i < 64; ++i)
        w[i] = SIG1(w[i-2]) + w[i-7] + SIG0(w[i-15]) + w[i-16];

    uint32_t a = m_state[0];
    uint32_t b = m_state[1];
    uint32_t c = m_state[2];
    uint32_t d = m_state[3];
    uint32_t e = m_state[4];
    uint32_t f = m_state[5];
    uint32_t g = m_state[6];
    uint32_t h = m_state[7];

    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + EP1(e) + CH(e,f,g) + k[i] + w[i];
        uint32_t t2 = EP0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
    m_state[4] += e;
    m_state[5] += f;
    m_state[6] += g;
    m_state[7] += h;
}