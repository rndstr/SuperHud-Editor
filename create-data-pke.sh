#!/bin/bash

VERSION=`cat configure.ac | grep AC_INIT | sed 's!.*\[\([0-9]\+\.[0-9]\+\.[0-9]\+\)\].*!\1!'`
#VERSION=`echo "AC_INIT([asdf], [30.3.0], [asdf])" | sed 's!.*\([0-9]\.[0-9]\.[0-9]\).*!\1!'`

if [ -z "$VERSION" ]; then
  echo "cannot find version, exiting."
  exit 1
fi

echo "version is $VERSION"
echo "zip -r data-$VERSION.pke . -i@data-$VERSION.include"
sleep 1
echo -n "."
sleep 1
echo -n "."
sleep 1
echo -n "."
echo
cd data/
rm -f data-$VERSION.pke
zip -r data-$VERSION.pke . -i@data-$VERSION.include
