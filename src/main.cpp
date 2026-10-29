#include "eclipse/world.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    eclipse::World world{42};
    constexpr int max_ticks = 180;
    constexpr auto frame_delay = std::chrono::milliseconds(180);

    std::cout << "ECLIPSE: primer experimento de vida artificial\n";
    std::cout << "Iniciando mundo... pulsa Ctrl+C para detener.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(900));

    while (world.tick() < max_ticks && world.organism().alive) {
        world.step();
        world.render();
        std::this_thread::sleep_for(frame_delay);
    }

    world.render();
    std::cout << "\nExperimento terminado. Pulsa Enter para cerrar...";
    std::cin.get();
    return 0;
}
