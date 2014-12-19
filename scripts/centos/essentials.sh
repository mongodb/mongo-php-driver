# Tools you can't live without
sudo yum install -y git vim


# I can't stand emacs
echo 'set -o vi' | sudo tee /etc/profile.d/vishell.sh

# Who knows how to configure RHEL at all anyway?
sudo service iptables stop
sudo chkconfig iptables off
