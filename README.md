# Snake (OpenGL + GLFW)

Ez egy egyszeru Snake jatek C++-ban, OpenGL-lel kirajzolva. A klasszikus jatekhoz kepest van benne egy kulon fal-szerkeszto szakasz is: inditas elott fel lehet rajzolni a falat pontokbol, es lehet rajta simitani is.

## Mit tud a jatek

- A kigyo atfordul a palya szelein.
- Van jo es rossz etel.
- A jatekos 3 elettel indul.
- A falat inditas elott lehet szerkeszteni egermutatos vezerlessel.
- Falnak utkozes vagy onutkozes eseten jatek vege.

## Fuggosegek

- OpenGL
- GLFW
- GLM
- CMake

Ha valami hianyzik macOS-en:

```bash
brew install glfw glm cmake
```

## Build es futtatas

```bash
cmake -S . -B build
cmake --build build -j
./build/Snake
```

## Vezerles

Szerkesztesi fazis:
- Bal klikk: uj pont
- Jobb klikk + huzas: pont mozgatasa
- J: simitas novelese
- N: simitas visszavetele
- Enter: fal rogzitese, jatek inditasa

Jatek fazis:
- Nyil gombok: mozgas
- R: uj jatek (game over utan)
- ESC: kilepes

## Simitas roviden

A fal pontjain egy Catmull-Clark jellegu 2D simitas fut. Egy lepes utan tobb kozbenso pont keletkezik, emiatt a gorbe folyamatosabbnak tunik.

## Forraskod szerkezet

- src/main.cpp: GLFW inicializalas, callbackek, fo ciklus
- src/Game.h/.cpp: jatekallapot, fazisok, etelkezeles
- src/Snake.h/.cpp: kigyo mozgas es novekedes
- src/Wall.h/.cpp: fal szerkesztes, simitas, utkozes
- src/Renderer.h/.cpp: kirajzolas
- src/Shader.h/.cpp: shader betoltes es uniform kezeles
- src/shaders/: GLSL shaderek

## Megjegyzes

macOS alatt az OpenGL deprecated warningok varhatoak. Ettol a program meg normalisan futhat.
