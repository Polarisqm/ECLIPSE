# ECLIPSE — Artificial Life & Emergent Intelligence Simulator

ECLIPSE is an educational artificial-life simulator. This prototype is the first observable step: a single organism lives in a small 2D world, searches for food, avoids rocks, spends energy and can die if it does not eat.

## Prototype 0.2

- `@` organism
- `*` food
- `#` rock
- `.` empty ground

The organism searches for the closest food within a limited vision radius. When food is not visible or a rock blocks its way, it explores. Movement costs energy; eating restores energy. Food can regrow over time. The random seed is fixed at `42` to make runs reproducible.

This is a rule-based agent, not a neural network and not conscious. Reproduction, genetics, multiple organisms and learning are future milestones.

## Build on Windows

Install Visual Studio Build Tools or Visual Studio with **Desktop development with C++** and CMake tools, plus CMake if it is not already available.

From PowerShell in this project folder:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\eclipse.exe
```

Depending on the CMake generator, the executable may instead be at `build\eclipse.exe`.

## Build on Linux/macOS

```bash
cmake -S . -B build
cmake --build build --config Release
./build/eclipse
```

A modern terminal with ANSI escape-code support gives the best animation. Press Ctrl+C to stop early. The program also stops after 180 ticks or when the organism dies.

## Git workflow

After testing the prototype:

```bash
git add .
git commit -m "feat: add first food-seeking organism"
git push
```

## Scientific note

The fixed random seed makes this particular run repeatable. One run is not enough to draw general conclusions about survival or behavior. Later versions will support multiple seeds, configuration files and machine-readable metrics.
