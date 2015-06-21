yum -y update
yum -y install openldap-servers openldap-clients openldap-devel python-devel gcc cyrus-sasl-plain xfsprogs net-snmp ps-misc wget python-ldap

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


