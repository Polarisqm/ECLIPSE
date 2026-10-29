#pragma once

#include <cstdint>
#include <random>
#include <vector>

namespace eclipse {

struct Organism {
    int x = 2;
    int y = 2;
    int energy = 30;
    int age = 0;
    int food_eaten = 0;
    bool alive = true;
};

class World {
public:
    static constexpr int width = 36;
    static constexpr int height = 16;
    static constexpr int vision = 8;

    explicit World(unsigned int seed = 42);

    void step();
    void render() const;

    std::uint64_t tick() const noexcept { return tick_; }
    const Organism& organism() const noexcept { return organism_; }

private:
    std::uint64_t tick_ = 0;
    Organism organism_{};
    std::vector<std::vector<bool>> food_;
    std::vector<std::vector<bool>> rock_;
    std::mt19937 rng_;

    bool in_bounds(int x, int y) const noexcept;
    bool can_move_to(int x, int y) const noexcept;
    void grow_food();
    void move_organism();
    void eat_if_possible();
};

} // namespace eclipse
