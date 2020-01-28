# install lcov
wget http://downloads.sourceforge.net/ltp/lcov-1.13-1.noarch.rpm
rpm -Uvh lcov-1.13-1.noarch.rpm
rm -f lcov-1.13-1.noarch.rpm

# install lcov deps
yum install -y perl-Digest-MD5

# install genhtml deps
yum install -y perl-GD
