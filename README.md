# MAI_GeoMS

sudo apt-get update \
    && apt-get install -y \
        build-essential cmake python-dev python-pip  \
    && pip install conan
   
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

conan install  ../src  --build missing -s compiler=gcc  -s compiler.libcxx=libstdc++11

cmake ../src && \
	cmake --build .
