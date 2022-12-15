#include <map>
#include <set>
#include <list>
#include <cmath>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

constexpr std::size_t k_max = 256;

int main()
{
    // https://ruletheweb.co.uk/blog/2014/04/ciphersaber/
    constexpr auto hexToIn = [](char h) -> int
    {
        if (h >= 'A' && h <= 'F') return h - 'A' + 10;
        if (h >= 'a' && h <= 'f') return h - 'a' + 10;
        return h - '0';
    };

    std::string str, encodedData;
    while(true)
    {
        std::getline(std::cin, str); // Read encrypted data.
        if (str[0] == '-') break;
        encodedData += str + " ";
    }
    
        std::istringstream parser{ encodedData };
    std::vector<uint8_t> encVect; // Encrypted data in bytes.

    // Convert data to byte vector.
    while (std::getline(parser, str, ' '))
    {
        std::size_t s1 = 0, s2 = 1;

        while (s1 < str.size())
        {
            encVect.emplace_back(hexToIn(str[s1]) * 16 + hexToIn(str[s2]));
            s1 += 2;
            s2 += 2;
        }
    }

    // std::getline(std::cin, str); // ----
    std::getline(std::cin, str); // Info
    std::istringstream parser2{ str };
    std::size_t vLenght{ 0 }, kIter{ 0 }; // Vector lenght, kIterations
    parser2 >> vLenght >> kIter >> str;

    // Create two 256-byte arrays called S and S2.
    std::vector<uint8_t> S, S2;     S.reserve(k_max);   S2.reserve(k_max);
    // Initialize S by filling it with all the values from 0 to 255 (i.e., S[0] = 0, S[1] = 1, S[2] = 2, and so on.)
    for (std::size_t i = 0; i < k_max; ++i) S.emplace_back(i);
    // Copy the secret key to the bytes at the start of S2.
    for (const auto k : str) S2.emplace_back(k);

    // Encrypting: you should then generate ten bytes of random data(called the initialization vector). Write a copy of these ten bytes to your output file.
    // Decrypting: read these X bytes back in from the start of the binary data.
    for (std::size_t i = 0; i < vLenght; ++i) S2.emplace_back(encVect[i]);
    auto end = encVect.begin();
    for(std::size_t i{}; i < vLenght; ++i)
    {
        ++end;
    }
    encVect.erase(encVect.begin(), end); // Erase it from string.

    // Append the initialization vector to S2, directly after the secret key.Then fill up the remainder of S2 by repeating the secret keyand initialization vector until you have set all 256 positions in S2.
    const std::size_t oriSizeS2{ S2.size() };
    std::size_t p{ 0 };
    while (S2.size() != k_max)
    {
        S2.emplace_back(S2[p]);
        ++p;
        if (p == oriSizeS2) p = 0;
    }

    // Now we have to randomize the contents of S based on the contents of S2.This is done by swapping bytes in S according to the following method, using the value of num_rounds you chose earlier :
    for (std::size_t n = 0, j = 0; n < kIter; ++n)
    {
        for (std::size_t i = 0; i < k_max; ++i)
        {
            j = (j + S[i] + S2[i]) % k_max;
            std::swap(S[i], S[j]);
        }
    }

    S2.clear();

    std::string data;
    // Use S to generate a pseudo-random stream of bytes to combine with the input data (using exclusive-or (XOR) operations). Since this is a symmetric cipher, the procedure is exactly the same for encryption and decryption:
    std::size_t i = 0, j = 0;
    for (auto b : encVect)
    {
        i = (i + 1) % k_max;
        j = (j + S[i]) % k_max;
        std::swap(S[i], S[j]);
        std::size_t k = (S[i] + S[j]) % k_max;
        data += static_cast<char>(b ^ S[k]);
    }

    std::cout << data << "\n";

    return 0;
}