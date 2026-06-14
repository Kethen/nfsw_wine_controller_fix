### sdl_wine_combine_triggers

Workaround for games that don't handle individual triggers

```
# build the workaround
bash build_podman.sh
LD_PRELOAD="$(pwd)/sdl_wine_combine_triggers_amd64.so;$(pwd)/sdl_wine_combine_triggers_386.so" ./your_game
```
