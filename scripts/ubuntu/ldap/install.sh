sudo apt-get -y install ldap-utils libsasl2-modules-ldap sasl2-bin

# setup saslauthd
sudo sed -i 's/MECHANISMS="pam"/MECHANISMS="ldap"/' /etc/default/saslauthd
sudo sed -i 's/START=no/START="yes"/' /etc/default/saslauthd
sudo cp /phongo/scripts/ubuntu/ldap/saslauthd.conf /etc/
sudo service saslauthd restart

testsaslauthd -u bugs -p password -s mongod -f /var/run/saslauthd/mux
#ldapsearch -x -LLL -b dc=10gen,dc=me -h 192.168.112.20
#ldapsearch -x -b '' -s base '(objectclass=*)' namingContexts -h 192.168.112.20

