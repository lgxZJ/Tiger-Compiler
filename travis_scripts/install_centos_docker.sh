yum -y install gcc-c++
yum -y install make
yum -y install automake

# used for unit-test coverage
yum -y install lcov

yum -y install wget
chmod u+x ./travis_scripts/install_lcov.sh
./travis_scripts/install_lcov_docker.sh

# used for static linking
yum -y install libstdc++-static
yum -y install glibc-static

chmod u+x ./travis_scripts/nstall_cppunit_centos_docker.sh
./travis_scripts/install_cppunit_centos_docker.sh

chmod u+x ./travis_scripts/install_glibc-devel-i686_centos_docker.sh
./travis_scripts/install_glibc-devel-i686_centos_docker.sh

yum -y install libtool
chmod u+x ./travis_scripts/install_cunit_centos_docker.sh
./travis_scripts/install_cunit_centos_docker.sh
