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
sudo yum install libtool wget

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
make
make install

# remove useless files
cd ..
rm -rf ./CUnit
rm -f CUnit.tar.bz2

# add additional lib path to a ld lib-load ENV variables
if [ -z "${LD_LIBRARY_PATH}" ]
then 

cat >> ~/.bashrc <<EOF

export LD_LIBRARY_PATH=/usr/local/lib 

EOF

else
    
cat >> ~/.bashrc <<EOF 

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib 

EOF
 
fi

echo ######################################################################
echo ############# execute following commands make it effect ##############
echo #############            source ~/.bashrc               ##############
echo ######################################################################
