#!/bin/sh

URL=$(curl --silent "https://api.github.com/repos/trilinos/Trilinos/tags" | grep "trilinos-release" | head -1 | grep tarball_url | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest Trilinos release: " $URL


echo "Downloading Trilinos tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting Trilinos tarball ..."
\rm -Rf Trilinos Trilinos_tmp
mkdir Trilinos_tmp
tar -xzf ${BASE}_tarball -C Trilinos_tmp
mv Trilinos_tmp/trilinos* Trilinos
\rm -Rf ${BASE}_tarball Trilinos_tmp

