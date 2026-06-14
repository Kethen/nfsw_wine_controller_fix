set -xe

CC=gcc
CPPC=g++

build () {
	C_SRC="libdetour/src/libdetour"
	CPP_SRC="main log"

	OBJS=""

	for f in $C_SRC
	do
		$CC $BUILD_FLAGS -c ${f}.c -o ${f}.o
		OBJS="$OBJS ${f}.o"
	done

	for f in $CPP_SRC
	do
		$CC $BUILD_FLAGS -c ${f}.cpp -o ${f}.o
		OBJS="$OBJS ${f}.o"
	done

	$CPPC $LINK_FLAGS $OBJS -o $1

}

BUILD_FLAGS="-Wformat -fPIC -O0 -g"
LINK_FLAGS="-shared"
build sdl_wine_combine_triggers.so
