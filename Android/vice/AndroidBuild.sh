#!/bin/sh


LOCAL_PATH=`dirname $0`
LOCAL_PATH=`cd $LOCAL_PATH && pwd`

ln -sf libsdl-1.2.so $LOCAL_PATH/../../../obj/local/$1/libSDL.so

if [ \! -f vice/configure ] ; then
	sh -c "cd vice && ./autogen.sh"
fi

if [ \! -f vice/$1/Makefile ] ; then
	mkdir -p vice/$1
	env PATH=`cd .. ; pwd`:$PATH ../setEnvironment-armeabi-v7a.sh sh -c "cd vice/$1 && ../configure --host=arm-linux-androideabi --without-x --enable-sdlui --with-sdlsound"
fi

make -j4 -C vice/$1 && mv -f vice/$1/src/x64 libapplication-$1.so
