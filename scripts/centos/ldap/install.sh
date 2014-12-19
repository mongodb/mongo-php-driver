yum -y update
yum -y install epel-release
yum -y install openldap-servers openldap-clients openldap-devel python-devel gcc cyrus-sasl-plain xfsprogs net-snmp ps-misc wget python-pip python-ldap

service slapd stop
service slapd start
#just in case
sleep 10

ldapadd -Y EXTERNAL -H ldapi:/// -f /phongo/scripts/centos/ldap/pw.ldif
# Add our specifics
ldapadd -x -D "cn=Manager,dc=10gen,dc=me" -w password -f /phongo/scripts/centos/ldap/Domain.ldif
ldapadd -x -D "cn=Manager,dc=10gen,dc=me" -w password -f /phongo/scripts/centos/ldap/Users.ldif

# Add the users
python /phongo/scripts/centos/ldap/ldapconfig.py -f /phongo/scripts/centos/ldap/users

# setup saslauthd
#sed -i 's/MECH=pam/MECH=ldap/' /etc/sysconfig/saslauthd
#cp /phongo/scripts/centos/ldap/saslauthd.conf /etc/
#service saslauthd start

testsaslauthd -u bugs -p password -s mongod -f /var/run/saslauthd/mux
#Show your work!
ldapsearch -x -LLL -b dc=10gen,dc=me
ldapsearch -x -b '' -s base '(objectclass=*)' namingContexts

#Set up mongod.conf
#echo "auth=true" >> /home/phongo/scripts/centos/ldap/mongod.conf
#echo "setParameter=saslauthdPath=/var/run/saslauthd/mux" >> /home/phongo/scripts/centos/ldap/mongod.conf
#echo "setParameter=authenticationMechanisms=PLAIN" >> /home/phongo/scripts/centos/ldap/mongod.conf
