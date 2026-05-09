Linux

##### Dependencies

`gcc make git alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama` 
Raylib

`git clone --depth 1 https://github.com/raysan5/raylib.git raylib
cd raylib/src/
make PLATFORM=PLATFORM_DESKTOP # To make the static version.
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED # To make the dynamic shared version.`

Include `raylib/` in `./Core/Source`
