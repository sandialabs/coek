#!/bin/sh

URL=$(curl -k --silent "https://api.github.com/repos/Tencent/rapidjson/tags" | grep tarball_url | head -1 | cut -d '"' -f 4)
BASE=$(basename $URL)
echo "Latest RapidJSON release: " $URL


echo "Downloading RapidJSON tarball ..."
wget -q -nv -O ${BASE}_tarball $URL
if [ ! -f ${BASE}_tarball ]; then
   echo "Didn't download $URL properly."
   exit 1
fi


echo "Extracting RapidJSON tarball ..."
\rm -Rf rapidjson rapidjson_tmp
mkdir rapidjson_tmp
tar -xzf ${BASE}_tarball -C rapidjson_tmp
mv rapidjson_tmp/*rapidjson* rapidjson
\rm -Rf ${BASE}_tarball rapidjson_tmp

