sudo yum -y install gcc-c++
sudo yum -y install make
sudo yum -y install automake

# used for unit-test coverage
sudo yum -y install lcov

sudo yum -y install wget
chmod u+x ./travis_scripts/install_lcov.sh
./travis_scripts/install_lcov.sh

# used for static linking
sudo yum -y install libstdc++-static
sudo yum -y install glibc-static

chmod u+x ./travis_scripts/nstall_cppunit_centos.sh
./travis_scripts/install_cppunit_centos.sh

chmod u+x ./travis_scripts/install_glibc-devel-i686_centos.sh
./travis_scripts/install_glibc-devel-i686_centos.sh

sudo yum -y install libtool
chmod u+x ./travis_scripts/install_cunit_ubuntu.sh
./travis_scripts/install_cunit_ubuntu.sh
