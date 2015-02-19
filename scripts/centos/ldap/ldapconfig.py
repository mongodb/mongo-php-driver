#!/usr/bin/python

import optparse
import ldap
import ldap.modlist as modlist

def main():
	parser = optparse.OptionParser(usage="""\
				%prog [options]
				Add users to LDAP """)

	# add in command line options. Add mongo host/port combo later
	parser.add_option("-f", "--filename", dest="fname",
			    help="name of file with user names",
			    default=None)

	(options, args) = parser.parse_args()

	if options.fname is None:
		print "\nERROR: Must specify name of file to import\n"
		sys.exit(-1)

	# Open a connection
	l = ldap.initialize("ldap://localhost")

	# Bind/authenticate with a user with apropriate rights to add objects
	l.simple_bind_s("cn=Manager,dc=10gen,dc=me","password")
	
	for uname in open(options.fname, 'r'):	
		try:
			# The dn of our new entry/object
			print "adding ", uname
			dn= 'uid=' + uname.lower() + ',ou=Users,dc=10gen,dc=me'
	
			ldif = configUser(uname.rstrip('\r\n'))

			# Do the actual synchronous add-operation to the ldapserver
			l.add_s(dn,ldif)
		except ldap.LDAPError, e:
			print e.message['info']

	# Its nice to the server to disconnect and free resources when done
	l.unbind_s()

# Do the tld configuration for the ldap tree
def configDC():
	# A dict to help build the "body" of the object
	attrs = {}
	attrs['objectclass'] = ['organization','dcObject']
	attrs['dn'] = 'dc=10gen,dc=me'
	attrs['dc'] = '10gen'
	attrs['o'] = '10gen'
	# Convert our dict to nice syntax for the add-function using modlist
	ldif = modlist.addModlist(attrs)

def configOU():
	# A dict to help build the "body" of the object
	attrs = {}
	attrs['dn'] = 'dc=10gen,dc=me'
	attrs['objectclass'] = ['organiationalUnit']
	attrs['ou'] = 'Users'
	ldif = modlist.addModlist(attrs)

def configUser( uname ):
	attrs = {}
#	attrs['dn'] = ['cn=' + uname + 'ou=Users,dc=10gen,dc=me']
	attrs['cn'] = [uname]
#	attrs['uid'] = [uname]
	attrs['sn'] = 'TestUser'
	attrs['objectclass'] = ['inetOrgPerson']
	attrs['userPassword'] = 'password'
	return modlist.addModlist(attrs)

if __name__ == "__main__":
	main()
