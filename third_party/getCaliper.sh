#!/bin/sh

URL=$(curl -k --silent "https://api.github.com/repos/LLNL/Caliper/releases/latest" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest Caliper release: " $URL


echo "Downloading Caliper tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting Caliper tarball ..."
\rm -Rf Caliper Caliper_tmp
mkdir Caliper_tmp
tar -xzf ${BASE}_tarball -C Caliper_tmp
mv Caliper_tmp/*Caliper* Caliper
\rm -Rf ${BASE}_tarball Caliper_tmp

