#!/bin/sh

[ -e Makefile ] || \
env LIBS="-lSDL_ttf -lguichan_sdl -lguichan" ../configure --without-x --enable-sdlui --with-sdlsound || exit 1

make -j4
