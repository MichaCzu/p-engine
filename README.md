## No longer updated

Project is no longer maintained nor developed. Setup might be problematic, so please keep this in mind.

## Building:

For building i have prepared a simple bash script that will run cmake from bin directory (which should also contain all app resources), and then make to create executable. simply run this in the console.

Linux

```
./compile.sh
```

Windows

```
./wcompile-st.bat
```

Otherwise just use cmake and then make to compile and run this project.

## Used libraries?

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- SDL_gpu
- //LZZ3
- //boost

## Subsystems/Headers:

- constants.hpp - important variables for quick engine configuration.
- core.hpp - initialization, engine cleaning, game loop management and game states management.
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
- crypt.hpp - encrypting and decrypting strings and files.
- file.hpp - saving and loading text files.
- ini.hpp - for handling .ini styled data files.

planned for later:

- archive.hpp - loading files from archives and managing resource packs.
- network.hpp - for multiplayer purposes and website communication (Curl / SDL_net)
- achiev.hpp - for achivement ( possibly steamapi )
- thread.hpp - for simple multihtreading implementation.
