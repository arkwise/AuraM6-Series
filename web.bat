@echo off
echo Starting the installation process...

echo Unzipping zlib...
unzip zlib-1.2.11.zip
cd zlib
echo Installing zlib...
rem Assuming there's a makefile or equivalent for zlib
make
make install
cd ..

echo Unzipping openssl (1.1.1m)...
unzip openssl-1.1.1m.zip
cd openssl
echo Installing openssl (1.1.1m)...
rem Assuming there's a makefile or equivalent for openssl
make
make install
cd ..

echo Unzipping openssl (1.1.1t)...
unzip openssl-1.1.1t.zip
cd opensst
echo Installing openssl (1.1.1t)...
rem Assuming there's a makefile or equivalent for openssl
make
make install
cd ..

echo Unzipping libssh2...
unzip libssh2-gf1b6fca.zip
cd libssh2
echo Installing libssh2...
rem Assuming there's a makefile or equivalent for libssh2
make
make install
cd ..

echo Unzipping mbedtls...
unzip mbedtls-2.7.10.zip
cd mbedtls
echo Installing mbedtls...
rem Assuming there's a makefile or equivalent for mbedtls
make
make install
cd ..

echo Unzipping watt32...
unzip watt32-2.2.10.zip
cd watt32-2.2.10
echo Installing watt32...
rem Assuming there's a makefile or equivalent for watt32
make
make install
cd ..

echo Unzipping curl...
unzip curl-8.2.1.zip
cd curl
echo Installing curl...
rem Assuming there's a makefile or equivalent for curl
make
make install
cd ..

echo Installation process completed!
