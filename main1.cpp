#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string_view>
#include <iomanip>
#include <tuple>

using namespace std;

struct Point2D
{
    double x{};
    double y{};
    friend std::ostream& operator<<(std::ostream& os, Point2D const& rhc);
};


std::ostream& operator<<(std::ostream& os, Point2D const& rhc)
{
    os << rhc.x << ' ' << rhc.y << '\n';
    return os;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    double planetMass{};
    Point2D meteorPos{};
    Point2D meteorSpeed{};
    std::cin >> planetMass;
    std::cin >> meteorPos.x >> meteorPos.y;
    std::cin >> meteorSpeed.x >> meteorSpeed.y;

    auto splitLine = [](std::string_view str) {
        // auto words = std::ranges::view::split(str, ' '); // <ranges> not in this compiler
        auto const p = str.find(' ');
        double deltaTime = std::stod(str.substr(0, p).data());
        int nTimes = std::stoi(str.substr(p + 1).data());
        return std::tuple{deltaTime, nTimes};
    };
    
    std::string line{};
    while(std::getline(std::cin, line))
    {
        if(line.empty()) { continue; }
        auto [ deltaTime, nTimes ] = splitLine(line);

        for(int i{}; i < nTimes; ++i)
        {
            double const dist = std::sqrt(meteorPos.x * meteorPos.x + meteorPos.y * meteorPos.y);
            double const radsq3 = dist * dist * dist;

            /*
                Ag = (G * M / r^2);
                mod_vector_each_dir = rx/r;
                mod_vector_each_dir = ry/r;

                (G * M / r^2) * (rx/r) -> G * M * rx / r^3
                (G * M / r^2) * (ry/r) -> G * M * ry / r^3
            */

            Point2D meteorAcc{};
            meteorAcc.x = (meteorPos.x == 0.0 && meteorPos.y == 0.0) ? 0.0 : -planetMass * meteorPos.x / radsq3;
            meteorAcc.y = (meteorPos.x == 0.0 && meteorPos.y == 0.0) ? 0.0 : -planetMass * meteorPos.y / radsq3;

            meteorSpeed.x += meteorAcc.x * deltaTime;
            meteorSpeed.y += meteorAcc.y * deltaTime;

            meteorPos.x += meteorSpeed.x * deltaTime;
            meteorPos.y += meteorSpeed.y * deltaTime;

            std::cout << std::fixed << std::setprecision(2) << meteorPos;
        }
        line = "";
    }
    
    return 0;
}