#!/bin/sh
#
# Enable debugging by executing: 
#    bin/trace.sh forward0 1
#    bin/trace.sh forward1 1
#
URL=$(curl --silent "https://api.github.com/repos/coin-or/CppAD/tags" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest CppAD release: " $URL


echo "Downloading CppAD tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting CppAD tarball ..."
\rm -Rf CppAD CppAD_tmp
mkdir CppAD_tmp
tar -xzf ${BASE}_tarball -C CppAD_tmp
mv CppAD_tmp/coin* CppAD
\rm -Rf ${BASE}_tarball CppAD_tmp

