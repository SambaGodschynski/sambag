export CFLAGS="-m32"
export CXXFLAGS="-m32"
export LDFLAGS="-m32"
export BUILDPATH="$(pwd)"
export PKG_CONFIG_PATH=${BUILDPATH}/lib/pkgconfig

cd boost
./bootstrap.sh --with-toolset=gcc 
./b2 install --prefix=$BUILDPATH address-model=32 #--layout=tagged --build-type=complete

cd zlib
./configure --prefix=$BUILDPATH
make install


cd cairo
./configure --prefix=$BUILDPATH --disable-xlib --disable-ft --disable-gobject
make install
