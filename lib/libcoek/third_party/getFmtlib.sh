#!/bin/sh

URL=$(curl -k --silent "https://api.github.com/repos/fmtlib/fmt/releases/latest" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest fmtlib release: " $URL


echo "Downloading fmtlib tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting fmtlib tarball ..."
\rm -Rf fmtlib fmtlib_tmp
mkdir fmtlib_tmp
tar -xzf ${BASE}_tarball -C fmtlib_tmp
mv fmtlib_tmp/fmtlib* fmtlib 
\rm -Rf ${BASE}_tarball fmtlib_tmp

