yum install lbzip2
wget https://dev-www.libreoffice.org/src/cppunit/cppunit-1.13.0.tar.gz -O cppunit.tar.gz
cd cppunit
./autogen.sh
./configure
make
make check # optional
make install
