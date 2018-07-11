sudo yum install gcc-c++
sudo yum install make
sudo yum install automake

# used for unit-test coverage
sudo yum install lcov

sudo yum install wget
chmod u+x ./travis_scripts/install_lcov.sh
./travis_scripts/install_lcov.sh

# used for static linking
sudo yum install libstdc++-static
sudo yum install glibc-static

chmod u+x ./travis_scripts/nstall_cppunit_centos.sh
./travis_scripts/install_cppunit_centos.sh

chmod u+x ./travis_scripts/install_glibc-devel-i686_centos.sh
./travis_scripts/install_glibc-devel-i686_centos.sh

sudo yum install libtool
chmod u+x ./travis_scripts/install_cunit_ubuntu.sh
./travis_scripts/install_cunit_ubuntu.sh
