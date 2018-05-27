wget https://sourceforge.net/projects/cppunit/files/cppunit/1.12.1/cppunit-1.12.1.tar.gz/download -O cppunit.tar.gz
mkdir cppunit
tar -zxvf cppunit.tar.gz -C ./cppunit --strip-components=1
cd cppunit
./configure
make
make check # optional
make install
