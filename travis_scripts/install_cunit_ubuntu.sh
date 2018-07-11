cunitLocations=`whereis libcunit.a`;
if [[ $cunitLocations = *"libcunit.a"* ]]; then
    echo 'CUnit is already installed! exiting...';
    exit 0;
fi

if [ -e "CUnit.tar.bz2" ]
then
    rm -f CUnit.tar.bz2
fi

if [ -d "CUnit" ]
then
    rm -rf CUnit
    mkdir CUnit
fi

# install needed native libraries
apt-get install libtool
apt-get install wget

# download CUnit,compile and install it
wget https://sourceforge.net/projects/cunit/files/latest/download -O CUnit.tar.bz2
mkdir CUnit
tar -jxvf CUnit.tar.bz2 -C ./CUnit --strip-components=1
cd CUnit

libtoolize
aclocal
autoheader
autoconf
automake --add-missing
chmod u+x ./configure
./configure 
sudo make
sudo make install

# remove useless files
cd ..
sudo rm -rf ./CUnit
sudo rm -f CUnit.tar.bz2
