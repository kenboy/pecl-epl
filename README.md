# EPL - Extended PHP Library

[![Build Status](https://travis-ci.org/kenboy/pecl-epl.svg?branch=master)](https://travis-ci.org/kenboy/pecl-epl) ![Supported PHP versions: 7.3](https://img.shields.io/badge/php-7.3-blue.svg) ![Supported PHP versions: 7.4](https://img.shields.io/badge/php-7.4-blue.svg)

### Install the extension

```shell
git clone git@github.com:kenboy/pecl-epl.git
cd pecl-epl
phpize
./configure
make
sudo make install
```

Then add `extension=epl.so` to your *php.ini*, or in a dedicated *epl.ini* file created within the include directory.  
