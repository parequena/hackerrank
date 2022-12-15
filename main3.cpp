#include <iostream>
#include <sstream>
#include <string>
#include <vector>

auto main() -> int {
    auto fillPath = [](std::vector<std::string>& path) {
        std::string str;
        std::getline(std::cin, str);
        std::istringstream parser{str};

        while (std::getline(parser, str, '/')) {
            if (str == ".") {
                continue;
            }
            if (str == "..") {
                path.pop_back();
                continue;
            }

            path.emplace_back(str);
        }
    };

    std::vector<std::string> firstPath;
    std::vector<std::string> secondPath;

    fillPath(firstPath);
    fillPath(secondPath);

    std::size_t r{0};
    while (r < firstPath.size() && r < secondPath.size() &&
           firstPath[r] == secondPath[r]) {
        ++r;
    }

    for (std::size_t i = r; i < firstPath.size(); ++i) {
        std::cout << "../";
    }
    for (std::size_t i = r; i < secondPath.size(); ++i) {
        std::cout << secondPath[i];
        if (i != secondPath.size() - 1) {
            std::cout << "/";
        }
    }

    return 0;
}