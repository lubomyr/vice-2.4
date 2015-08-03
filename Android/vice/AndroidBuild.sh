#!/bin/sh


LOCAL_PATH=`dirname $0`
LOCAL_PATH=`cd $LOCAL_PATH && pwd`

ln -sf libsdl-1.2.so $LOCAL_PATH/../../../obj/local/armeabi-v7a/libSDL.so

if [ \! -f vice/configure ] ; then
	sh -c "cd vice && ./autogen.sh"
fi

if [ \! -f vice/Makefile ] ; then
	../setEnvironment-armeabi-v7a.sh sh -c "cd vice && ./configure --host=arm-linux-androideabi --without-x --enable-sdlui --with-sdlsound"
fi

make -C vice && mv -f vice/src/x64 libapplication-armeabi-v7a.so
