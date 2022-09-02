#!/bin/sh

URL=$(curl -k --silent "https://api.github.com/repos/pybind/pybind11/tags" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest pybind11 release: " $URL


echo "Downloading pybind11 tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting pybind11 tarball ..."
\rm -Rf pybind11 pybind11_tmp
mkdir pybind11_tmp
tar -xzf ${BASE}_tarball -C pybind11_tmp
mv pybind11_tmp/pybind* pybind11
\rm -Rf ${BASE}_tarball pybind11_tmp

