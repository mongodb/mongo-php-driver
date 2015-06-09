if [ ! -e ".provisioned" ]; then
	# Tools you can't live without
	apt-get update
	apt-get install -y build-essential git vim libtool autoconf

	# I can't stand emacs
	echo 'set -o vi' | sudo tee /etc/profile.d/vishell.sh

	touch .provisioned
fi
