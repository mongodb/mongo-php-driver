cd C:\php-sdk\phpdev\vc11\x86\pecl
"C:\Program Files\7-Zip\7z.exe" x -y Z:\phongo\mongodb-*.tgz
"C:\Program Files\7-Zip\7z.exe" x -y mongodb-*.tar
cd C:\php-sdk\phpdev\vc11\x86\php-src
C:\php-sdk\bin\phpsdk_setvars.bat
"c:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"
buildconf --force
configure --disable-all --enable-cli --enable-json --with-openssl --enable-mongodb

nmake
#nmake test TESTS="..\pecl\mongodb-*\tests"
