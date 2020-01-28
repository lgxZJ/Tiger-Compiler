################################################################################################
# WARNING:
#	Not runing this script under WSL on windows, ths ABI of x86 (gcc -m32) architecture
#	computers broke. Thus you can only compile this Tiger-Compiler successfully but cannot
#	run the generated exectuable, it will prompt you infos like the following:
#
#		`cannot execute binary file exec format error`
#
################################################################################################
apt-get install make
apt-get install automake
apt-get install lcov
./travis_scripts/install_cunit_ubuntu.sh
apt-get install g++ 
apt-get install libcppunit-dev
chmod u+x ./travis_scripts/install_libc6-dev-i386_ubuntu.sh
./travis_scripts/install_libc6-dev-i386_ubuntu.sh
