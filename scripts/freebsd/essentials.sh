# Update ports
sudo portsnap fetch extract update

# Minimum required PHP install + pecl
sudo pkg install -y pcre php56 php56-openssl php56-json php56-zlib pear autoconf pkgconf cyrus-sasl

# We need vim. git requires curl. mongoc requires libtool and automake 
sudo pkg install -y vim git curl libtool automake

# I can't stand emacs
echo 'set -o vi' | sudo tee -a /etc/profile


