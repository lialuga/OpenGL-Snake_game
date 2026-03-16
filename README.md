# OpenGL Snake Game (macOS, GLFW)

Snake jatek OpenGL + GLFW alapon, kiegeszitve fal-szerkeszto moddal es Catmull-Clark jellegu simitassal.

## Fobb funkciok

- Wrap-around mozgas a palya szelen.
- Jo/rossz etel mechanika.
- 3 eletes jateklogika.
- Szerkesztheto fal az inditas elott:
  - pontok lerakasa,
  - pontok huzasa,
  - simitas (J), visszavonas (N),
  - Enterrel jatek inditas.
- Fal-utkozes detektalas a kigyo fejenel (NDC pont-szakasz tavolsag).

## Konyvtarak

- OpenGL (rendszer)
- GLFW
- GLM
- CMake

Homebrew telepites (ha hianyzik):

```bash
brew install glfw glm cmake
```

## Build es futtatas

A projekt gyokerbol:

```bash
cmake -S . -B build
cmake --build build -j
./build/Snake
```

## Vezerles

### Szerkesztesi fazis (WallPhase::EDITING)

- Bal klikk: uj kontrol pont hozzaadasa
- Jobb klikk + huzas: legkozelebbi pont mozgatasa
- J: egy szint Catmull-Clark jellegu simitas
- N: simitas visszavonasa egy szinttel
- Enter: fal fixalasa, atlepes jatek fazisba

### Jatek fazis (WallPhase::PLAYING)

- Nyil gombok: mozgas
- ESC: kilepes
- R: ujrainditas (game over utan)

## Catmull-Clark jellegu simitas (2D gorbere)

Egy simitasi lepesben:

- Belsso pontok:
  - P'i = (1/8) * P(i-1) + (6/8) * Pi + (1/8) * P(i+1)
- El-kozeppontok:
  - E(i,i+1) = 0.5 * Pi + 0.5 * P(i+1)
- Vegpontok fixen maradnak

Igy N pontbol egy lepes utan 2N - 1 pont lesz.

## Projekt felépites

- `src/main.cpp`: ablak, callbackek, fo ciklus
- `src/Game.h/.cpp`: jatekallapot, faziskezeles, etel/snake logika
- `src/Snake.h/.cpp`: kigyo mozgas, novekedes, onutkozes
- `src/Wall.h/.cpp`: fal szerkesztes, simitas, rajz, utkozes
- `src/Renderer.h/.cpp`: kirajzolas, effektek, HUD-szeru vizualis allapotok
- `src/Shader.h/.cpp`: shader betoltes/kezeles
- `src/shaders/*`: GLSL shader fajlok

## Megjegyzes

macOS alatt az OpenGL API deprecated warningokat adhat forditaskor; ez varhato es nem feltetlenul hiba.
