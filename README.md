# README

ShatteredLands

## Building:

For building i have prepared a simple bash script that will run cmake from bin directory (which should also contain all app resources), and then make to create executable. simply run this in the console.

```
./compile.sh
```

Otherwise just use cmake and then make to compile and run this project.

## Requirements?

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- SDL_gpu
- //boost

## Subsystems/Headers:
I know those names are silly but they just makes it easier for me to work with.

- constants.hpp - important variables for quick engine configuration.
- core.hpp - initialization, engine cleaning, game loop managment and game states managment.
- debug.hpp - creating logs.
- engine.hpp - header for quick including.
- types.hpp - engine variables definitions.
- stateman.hpp - game state template.
- draw.hpp - viewport managment, calculating drawing scale and offset, primitive drawing.
- image.hpp - loading, managing and drawing sprites on screen.
- sound.hpp - loading, managing and playing sounds and music.
- text.hpp - loading, managing and drawing text.
- locale.hpp - loading and managing game locale files.
- input.hpp - managing game controls, getting text input.
- crypt.hpp - encrypting and decrypting strings, files and whole packs.
- file.hpp - saving and loading text files.
- ini.hpp - for handling .ini styled data files.

planned for later:

- archive.hpp - loading files from and managing resource packs.
- network.hpp - for multiplayer purposes and website communication (Curl / SDL_net)
- achiev.hpp - for achivement ( possibly steamapi )
- thread.hpp - for very easy multihtreading implementation.
