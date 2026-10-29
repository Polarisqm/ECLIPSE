#include "eclipse/world.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

namespace eclipse {

World::World(unsigned int seed)
    : food_(height, std::vector<bool>(width, false)),
      rock_(height, std::vector<bool>(width, false)),
      rng_(seed) {
    // A few fixed rocks create obstacles that the organism must navigate around.
    const int rocks[][2] = {
        {8, 3}, {8, 4}, {8, 5}, {17, 8}, {18, 8}, {19, 8},
        {26, 3}, {26, 4}, {26, 5}, {29, 12}, {30, 12}
    };
    for (const auto& rock : rocks) {
        rock_[rock[1]][rock[0]] = true;
    }

    // Initial food distribution. The fixed seed makes the run reproducible.
    std::uniform_int_distribution<int> x_dist(1, width - 2);
    std::uniform_int_distribution<int> y_dist(1, height - 2);
    for (int i = 0; i < 32; ++i) {
        const int x = x_dist(rng_);
        const int y = y_dist(rng_);
        if (!rock_[y][x] && !(x == organism_.x && y == organism_.y)) {
            food_[y][x] = true;
        }
    }
}

bool World::in_bounds(int x, int y) const noexcept {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool World::can_move_to(int x, int y) const noexcept {
    return in_bounds(x, y) && !rock_[y][x];
}

void World::move_organism() {
    // The organism looks for the closest food within its vision radius.
    int target_x = -1;
    int target_y = -1;
    int best_distance = std::numeric_limits<int>::max();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!food_[y][x]) continue;
            const int distance = std::abs(x - organism_.x) + std::abs(y - organism_.y);
            if (distance <= vision && distance < best_distance) {
                best_distance = distance;
                target_x = x;
                target_y = y;
            }
        }
    }

    std::vector<std::pair<int, int>> candidates;
    if (target_x >= 0) {
        // Greedy navigation: try steps that reduce distance to the target.
        const int dx = (target_x > organism_.x) - (target_x < organism_.x);
        const int dy = (target_y > organism_.y) - (target_y < organism_.y);
        if (dx != 0) candidates.emplace_back(organism_.x + dx, organism_.y);
        if (dy != 0) candidates.emplace_back(organism_.x, organism_.y + dy);
        if (dx != 0) candidates.emplace_back(organism_.x, organism_.y + 1);
        if (dx != 0) candidates.emplace_back(organism_.x, organism_.y - 1);
        if (dy != 0) candidates.emplace_back(organism_.x + 1, organism_.y);
        if (dy != 0) candidates.emplace_back(organism_.x - 1, organism_.y);
    }

    // If no food is visible, or a rock blocks the direct path, explore randomly.
    std::shuffle(candidates.begin(), candidates.end(), rng_);
    for (const auto& [nx, ny] : candidates) {
        if (can_move_to(nx, ny)) {
            organism_.x = nx;
            organism_.y = ny;
            return;
        }
    }

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    std::shuffle(directions.begin(), directions.end(), rng_);
    for (const auto& [dx, dy] : directions) {
        if (can_move_to(organism_.x + dx, organism_.y + dy)) {
            organism_.x += dx;
            organism_.y += dy;
            return;
        }
    }
}

void World::eat_if_possible() {
    if (food_[organism_.y][organism_.x]) {
        food_[organism_.y][organism_.x] = false;
        organism_.energy = std::min(60, organism_.energy + 14);
        ++organism_.food_eaten;
    }
}

void World::grow_food() {
    // Small chance that a new food source appears each tick.
    std::uniform_int_distribution<int> chance(0, 99);
    if (chance(rng_) >= 18) return;

    std::uniform_int_distribution<int> x_dist(1, width - 2);
    std::uniform_int_distribution<int> y_dist(1, height - 2);
    for (int attempt = 0; attempt < 12; ++attempt) {
        const int x = x_dist(rng_);
        const int y = y_dist(rng_);
        if (!rock_[y][x] && !food_[y][x] && !(x == organism_.x && y == organism_.y)) {
            food_[y][x] = true;
            return;
        }
    }
}

void World::step() {
    if (!organism_.alive) return;

    ++tick_;
    ++organism_.age;
    --organism_.energy; // Living and moving costs energy.

    move_organism();
    eat_if_possible();
    grow_food();

    if (organism_.energy <= 0) {
        organism_.energy = 0;
        organism_.alive = false;
    }
}

void World::render() const {
    // ANSI escape codes clear the terminal between frames in modern terminals.
    std::cout << "\x1B[2J\x1B[H";
    std::cout << "====================================================\n";
    std::cout << " E C L I P S E  |  ORGANISMO 001  |  PROTOTIPO 0.2\n";
    std::cout << "====================================================\n";
    std::cout << "Tick: " << tick_
              << "   Edad: " << organism_.age
              << "   Energia: " << organism_.energy << "/60"
              << "   Alimentos: " << organism_.food_eaten << "\n\n";
    std::cout << "Leyenda: @ organismo   * alimento   # roca   . suelo\n\n";

    for (int y = 0; y < height; ++y) {
        std::cout << "  ";
        for (int x = 0; x < width; ++x) {
            if (organism_.x == x && organism_.y == y && organism_.alive) {
                std::cout << '@';
            } else if (rock_[y][x]) {
                std::cout << '#';
            } else if (food_[y][x]) {
                std::cout << '*';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }

    std::cout << "\nComportamiento: ";
    if (!organism_.alive) {
        std::cout << "el organismo ha muerto por falta de energia.\n";
    } else if (organism_.energy < 10) {
        std::cout << "energia critica; debe encontrar alimento pronto.\n";
    } else {
        std::cout << "busca alimento cercano y evita las rocas.\n";
    }
    std::cout << "Semilla aleatoria: 42 (ejecucion reproducible)\n";
    std::cout.flush();
}

} // namespace eclipse
