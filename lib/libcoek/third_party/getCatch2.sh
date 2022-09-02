#!/bin/sh

URL=$(curl -k --silent "https://api.github.com/repos/catchorg/Catch2/releases/latest" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest Catch2 release: " $URL


echo "Downloading Catch2 tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting Catch2 tarball ..."
\rm -Rf Catch2 Catch2_tmp
mkdir Catch2_tmp
tar -xzf ${BASE}_tarball -C Catch2_tmp
mv Catch2_tmp/catch* Catch2
\rm -Rf ${BASE}_tarball Catch2_tmp

