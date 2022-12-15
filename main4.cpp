#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Player {
    int score{};
    int win_rounds{};
};

struct Game {
    explicit Game(std::size_t nPlayers) : nPlayers_{nPlayers} {
        players_.resize(static_cast<std::size_t>(nPlayers));
    }

    inline void processLine(std::string const& line) {
        if (noMoreRounds()) {
            std::cout << "error: too many rounds\n";
            return;
        }

        auto result = playTurn(line);
        if (result == Result::ERROR) {
            return;
        }

        if (result == Result::WINNER) {
            printScores();
            clearScores();
            currentPlayer_ = 0;
            return;
        }

        if (++currentPlayer_ == nPlayers_) {
            currentPlayer_ = 0;
            printScores();
        }
    }

   private:
    enum class Result : std::uint8_t { WINNER, NO_WINNER, ERROR };
    int current_round_{1};
    std::size_t currentPlayer_{};
    std::size_t nPlayers_{};
    static constexpr int target_score_{21};
    static constexpr int total_rounds_{6};
    static constexpr int max_dice_size_{6};
    static constexpr int total_dices_{3};

    std::vector<Player> players_{};

    // ---------------------------------------------------------------------------------------
    [[nodiscard]] static auto getDices(std::string const& line) noexcept
        -> std::array<int, total_dices_> {
        std::array<int, total_dices_> ret{};
        for (std::size_t i{}; i < total_dices_; ++i) {
            ret[i] = std::atoi(&line[i * 2]);
        }
        return ret;
    }

    auto playTurn(std::string line) -> Result {
        auto dices = getDices(line);
        auto score = getScore(dices);
        if (score == 0 && line.size() > 5) {
            std::cout << "error: too many dice rolls\n";
            return Result::ERROR;
        }
        if (score == -1) {
            return Result::ERROR;
        }

        while (score != 0) {
            auto& playerScore = players_[currentPlayer_].score;
            playerScore += score;
            if (playerScore >= target_score_) {
                if (line.size() > 6) {
                    std::cout << "error: too many dice rolls\n";
                    return Result::ERROR;
                }

                ++players_[currentPlayer_].win_rounds;
                ++current_round_;
                return Result::WINNER;
            }

            try {
                line = line.substr(6);
            } catch (std::exception const&) {
                std::cout << "error: missing dice rolls\n";
                return Result::ERROR;
            }
            dices = getDices(line);
            score = getScore(dices);
            if (score == 0 && line.size() > 5) {
                std::cout << "error: too many dice rolls\n";
                return Result::ERROR;
            }
            if (score == -1) {
                return Result::ERROR;
            }
        }

        return Result::NO_WINNER;
    }

    void printScores() const noexcept {
        std::cout << "score ";
        for (auto const& p : players_) {
            std::cout << p.score << ' ';
        }
        std::cout << '\n';
    }

    void clearScores() noexcept {
        for (auto& p : players_) {
            p.score = 0;
        }
    }

    [[nodiscard]] auto noMoreRounds() const noexcept -> bool {
        for (auto const& p : players_) {
            if (p.win_rounds >= total_rounds_) {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] auto getScore(
        std::array<int, total_dices_> const& dices) const noexcept -> int {
        auto areSame = [&]() {
            return dices[0] <= max_dice_size_ && dices[0] == dices[1] &&
                   dices[0] == dices[2];
        };

        if (areSame() && dices[0] == current_round_) {
            return 21;
        }
        if (areSame()) {
            return 5;
        }

        int score{};
        for (auto d : dices) {
            if (d == 0 || d > max_dice_size_) {
                std::cout << "error: invalid dice roll\n";
                return -1;
            }
            if (d == current_round_) {
                score += 1;
            }
        }

        return score;
    }
};

auto main() -> int {
    std::unique_ptr<Game> game{nullptr};
    std::string str{};

    auto findStart = [](std::string const& line) {
        std::string const start_str{"start "};
        auto pos = line.find(start_str);
        if (pos == std::string::npos) {
            return 0;
        }
        for (std::size_t i{}; i < start_str.size(); ++i) {
            ++pos;
        }
        return std::stoi(line.substr(pos));
    };

    while (std::getline(std::cin, str)) {
        auto nPlayers = findStart(str);
        if (nPlayers != 0) {
            game = std::make_unique<Game>(nPlayers);
            continue;
        }

        if (!game) {
            std::cout << "error: missing player\n";
            continue;
        }

        game->processLine(str);
    }

    return 0;
}