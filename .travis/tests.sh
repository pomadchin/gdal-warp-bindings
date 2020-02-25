#!/bin/bash

ln -s /tmp/c41078a1.tif src/experiments/data/c41078a1.tif # sic

docker run -it --rm \
      -v $(pwd):/workdir \
      -e CC=gcc -e CXX=g++ \
      -e CFLAGS="-Wall -Wno-sign-compare -Werror -O0 -ggdb3 -DSO_FINI -D_GNU_SOURCE" \
      -e BOOST_ROOT="/usr/local/include/boost_1_69_0" \
      -e JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64" \
      jamesmcclain/gdal-build-environment:4 make -j4 -C src tests || exit -1

docker run -it --rm \
      -v $(pwd):/workdir \
      -e CC=gcc -e CXX=g++ \
      -e JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64" \
      jamesmcclain/gdal-build-environment:4 make -j4 -C src/experiments/thread pattern oversubscribe pattern_jp2k || exit -1

# rm -f $(find | grep '\.\(o\|obj\|dylib\|dll\|so\|class\)$')
# rm -f src/com_azavea_gdal_GDALWarp.h
# rm -f src/experiments/thread/oversubscribe src/experiments/thread/pattern

# s3://geotrellis-test/daunnc/B08.jp2
# /vsis3/geotrellis-test/daunnc/B08.jp2
# gdalinfo /vsis3/geotrellis-test/daunnc/B08.jp2
# docker run -it --rm \
#       -v $(pwd):/workdir \
#       -e CC=gcc -e CXX=g++ \
#       -e JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64" \
#       daunnc/gdalwarpenv:0.11 bash
