//
// Created by paradaimu on 7/29/20.
//

#include "hmac.h"

#include "mexceptions.h"

#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace
{
std::vector<uint8_t> hexToBytes(const std::string hex) {
    std::vector<uint8_t> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

std::vector<uint8_t> stringToBytes(const std::string str) {
    return std::vector<uint8_t>{str.begin(), str.end()};
}

template< class data_type >
std::ostream& operator<<(std::ostream& output, const std::vector<data_type>& vec)
{ 
    output << std::hex;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(output, ""));

    return output;  
}

void test_md5() {
    const std::string s("Hi There");
    const std::vector<uint8_t> data(s.begin(), s.end());
    const std::vector<uint8_t> key = hexToBytes("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
    const std::vector<uint8_t> exp = hexToBytes("9294727a3638bb1c13f48ef8158bfc9d");

    cryptonite::Hmac hmac{cryptonite::HashType::MD5, key};
    hmac.update(data);
    const std::vector<uint8_t> act = hmac.finale();

    if (act != exp) {
        THROW_EXCEPTION("MD5 1. Not equal.")
    }

    std::cout << __func__ << ": done" << std::endl;
}

void test_hmac_md5_2() {
    const std::string ds("Test Using Larger Than Block-Size Key - Hash Key First");
    const std::vector<uint8_t> key = hexToBytes("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    const std::vector<uint8_t> data(ds.begin(), ds.end());
    const std::vector<uint8_t> exp = hexToBytes("6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd");

    cryptonite::Hmac hmac{cryptonite::HashType::MD5, key};
    hmac.update(data);
    const std::vector<uint8_t> act = hmac.finale();
    if (act != exp) {
        THROW_EXCEPTION("MD5 2. Not equal.")
    }

    std::cout << __func__ << ": done" << std::endl;
}

void test_hmac_sha256() {
    const std::string s("Hi There");
    const std::vector<uint8_t> data(s.begin(), s.end());
    const std::vector<uint8_t> key = hexToBytes("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
    const std::vector<uint8_t> exp = hexToBytes("b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7");

    cryptonite::Hmac hmac{cryptonite::HashType::SHA2_256, key};
    hmac.update(data);
    const std::vector<uint8_t> act = hmac.finale();

    if (act != exp) {
        THROW_EXCEPTION("Sha256_256. Not equal.")
    }

    std::cout << __func__ << ": done" << std::endl;
}

void test_hmac_sha256_string_key() {
    const std::vector<uint8_t> data{stringToBytes("Hi There")};
    const std::vector<uint8_t> key{stringToBytes("string_key")};
    const std::vector<uint8_t> exp = hexToBytes("ecc9b92820b8af64985aee3dfa7d9e0897a87e88cbcc0dddb60a21d29d152e38");

    cryptonite::Hmac hmac{cryptonite::HashType::SHA2_256, key, data};
    const std::vector<uint8_t> act = hmac.finale();

    if (act != exp) {
        THROW_EXCEPTION("Sha256_256. Not equal.")
    }

    std::cout << __func__ << ": done" << std::endl;
}

void test_hmac_sha256_string_key_update_data() {
    const std::vector<uint8_t> data{stringToBytes("Hi")};
    const std::vector<uint8_t> key{stringToBytes("string_key")};
    const std::vector<uint8_t> exp = hexToBytes("ecc9b92820b8af64985aee3dfa7d9e0897a87e88cbcc0dddb60a21d29d152e38");

    cryptonite::Hmac hmac{cryptonite::HashType::SHA2_256, key, data};
    const std::vector<uint8_t> additionalData{stringToBytes(" There")};
    hmac.update(additionalData);
    const std::vector<uint8_t> act = hmac.finale();

    if (act != exp) {
        THROW_EXCEPTION("Sha256_256. Not equal.")
    }

    std::cout << __func__ << ": done" << std::endl;
}
}

int main() {
    try {
        test_md5();
        test_hmac_md5_2();
        test_hmac_sha256();
        test_hmac_sha256_string_key();
        test_hmac_sha256_string_key_update_data();
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}