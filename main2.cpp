#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace { 
constexpr int df_offset { 10 };
constexpr int df_max { 256 };
} // namespace

struct KeyInfo
{
    std::size_t vector_lenght{};
    std::size_t vector_iterations{};
    std::string key{};
};

auto main() -> int
{
    auto encodedData = []()
    {
        std::string encoded{};
        std::string str{};

        while(true)
        {
            std::getline(std::cin, str);
            if(str[0] == '-') { return encoded; }
            encoded += str + ' ';
        }
    }();

    auto constexpr hexToDec = [](char h)
    {
        if( h >= 'A' && h <= 'F') { return h - 'A' + df_offset; }
        if (h >= 'a' && h <= 'f') { return h - 'a' + df_offset; };
        return h - '0';
    };

    auto get_encoded_vector = [&hexToDec](std::string const& encoded_data)
    {
        std::istringstream parser { encoded_data };
        std::vector<std::uint8_t> encoded_vector{};
        encoded_vector.reserve(encoded_data.size() / 2);
        std::string str{};
        while(std::getline(parser, str, ' '))
        {
            for(std::size_t i{}, j{1}; i < str.size(); i+=2, j+=2)
            {
                auto const c1 = str[i];
                auto const c2 = str[j];

                auto const val1 = hexToDec(c1) * 16;
                auto const val2 = hexToDec(c2);

                encoded_vector.emplace_back(val1 + val2);
            }
        }

        return encoded_vector;
    };

    auto encoded_vector = get_encoded_vector(encodedData);

    auto get_encode_info = []()
    {
        std::string str{};
        std::string const key{};
        std::getline(std::cin, str);
        std::istringstream parser { str };
        KeyInfo key_info{};
        parser >> key_info.vector_lenght >> key_info.vector_iterations >> key_info.key;
        return key_info;
    };

    KeyInfo const info = get_encode_info();
    std::array<std::uint8_t, df_max> S2{};
    auto fillS2 = [&S2](std::string const& key)
    {
        std::size_t i{};
        for(auto const k : key)
        {
            S2[i] = static_cast<std::uint8_t>(k);
            ++i;
        }
        return i;
    };

    std::size_t const last_s2_idx = fillS2(info.key);
    auto copy_encoded_key = [&S2, &last_s2_idx, &encoded_vector](std::size_t const vector_lenght)
    {
        auto end = encoded_vector.begin();
        for(std::size_t i{}; i < vector_lenght; ++i)
        {
            S2[last_s2_idx + i] = encoded_vector[i];
            ++end;
        }

        encoded_vector.erase(encoded_vector.begin(), end);
        return last_s2_idx + vector_lenght;
    };

    std::size_t const last_s2_size = copy_encoded_key(info.vector_lenght);
    auto repeat_secret_key = [&last_s2_size, &S2](std::string const& key)
    {
        for(std::size_t i { last_s2_size }; i < df_max; i+= key.size())
        {
            std::size_t j { 0 };
            for(auto const k : key)
            {
                S2[i + j] = static_cast<std::uint8_t>(k);
                ++j;
            }
        }
    };

    repeat_secret_key(info.key);

    std::array<std::uint8_t, df_max> S{};
    [[maybe_unused]] auto fillS = [&S]()
    {
        for(std::size_t i{}; i < df_max; ++i) { S[i] = static_cast<std::uint8_t>(i); }
        return true;
    }();


    [[maybe_unused]] auto print = [&]()
    {
        std::cout << "S: ";
        for(auto const s : S) { std::cout << static_cast<int>(s) << ' '; }
        std::cout << "\n";
        return true;
    }();

    auto randomize_contents = [&S, &S2](std::size_t const vector_iterations)
    {
        for (std::size_t n = 0, j = 0; n < vector_iterations; ++n)
        {
            for (std::size_t i = 0; i < df_max; ++i)
            {
                j = (j + S[i] + S2[i]) % df_max;
                std::swap(S[i], S[j]);
            }
        }
    };

    randomize_contents(info.vector_iterations);
    std::string out_data{};
    std::size_t i{};
    std::size_t j{};
    for (auto b : encoded_vector)
    {
        i = (i + 1) % df_max;
        j = (j + S[i]) % df_max;
        std::swap(S[i], S[j]);
        std::size_t const k = (S[i] + S[j]) % df_max;
        out_data += static_cast<char>(b ^ S[k]);
    }

    std::cout << out_data << "\n";

    return 0;
}