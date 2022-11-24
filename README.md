# zezlup <img align="left" width="100" height="100" src=https://github.com/manekenpix/zezlup/blob/main/data/images/logo.png>

![Format Check](https://github.com/manekenpix/zezlup/actions/workflows/clang-format-check.yml/badge.svg)

Zezlup is a little slide-puzzle game for Linux created for the sole purpose of learning a bit of game development.

To make it more fun (and also learn more), I made my own tiny renderer for this project.

## Build

### Dependencies

- [zlib](https://zlib.net/)
- [glfw](https://www.glfw.org/)
- [glew](http://glew.sourceforge.net/)
- [glm](https://github.com/g-truc/glm)
- [freetype](https://freetype.org/)

To build zezlup:

```sh
git clone https://github.com/manekenpix/zezlup.git
cd zezlup
make release
```

## Game

To run the game:

```sh
./zezlup
```

### Menu

Select the puzzle you want to solve (use your mouse or arrows and enter keys)

![Image](data/images/zezlup_menu.png)

### Gameplay

**Controls:**

- `Arrow keys`: move selector.
- `x`: slide the selected cell.
- `z` (toggle): display the solution of the puzzle.
- `Backspace`: back to menu.
- `Esc`: exit zezlup.

![Image](data/images/zezlup_gameplay.png)

Display the solution of the puzzle when you feel lost

![Image](data/images/zezlup_solution.png)
