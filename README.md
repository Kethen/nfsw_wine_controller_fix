## nfsw_wine_controller_fix

1. Workaround the game not accepting controllers that are xinput mapped but does not carry xbox360 vendor id and product id
2. Workaround the game getting confused by other input devices

With this workaround, as long as SDL manages to map the controller as a `SDL_GameController`, wine will be able to create a dinput8 Xbox360 controller that is good enough for the game.

For maximum controller compatibility, `sdl2-compat` is recommended over old sdl2 builds.

```
# build the workaround
bash build_podman.sh
LD_PRELOAD="$(pwd)/nfsw_wine_controller_fix_amd64.so;$(pwd)/nfsw_wine_controller_fix_386.so" wine soapbox_launcher_here/GameLauncher.exe
```
