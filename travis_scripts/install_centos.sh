sudo yum install gcc-c++

# used for unit-test coverage
sudo yum install lcov

# used for unit test of C language
sudo yum install CUnit-devel

# used for static linking
sudo yum install libstdc++-static
sudo yum install glibc-static

chmod u+x ./install_cppunit_centos.sh
./install_cppunit_centos.sh

chmod u+x ./install_glic-devel-i686_centos.sh
./install_glibc-devel-i686_centos.sh
