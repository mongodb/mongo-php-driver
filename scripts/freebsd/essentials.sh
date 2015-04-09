# Update ports
sudo portsnap fetch extract

# Minimum required PHP install + pecl
sudo pkg install pcre php56 php56-openssl php56-json pear autoconf cyrus-sasl

# We need vim. git requires curl. mongoc requires libtool and automake 
sudo pkg install vim git curl libtool automake

# I can't stand emacs
echo 'set -o vi' | sudo tee /etc/profile.d/vishell.sh


