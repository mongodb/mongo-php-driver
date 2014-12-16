# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "http://files.vagrantup.com/precise64.box"

  config.vm.network "private_network", ip: "192.168.112.10"

  config.vm.synced_folder ".", "/phongo"

  config.vm.provider "vmware_workstation" do |vmware, override|
    vmware.vmx["memsize"] = "8192"
    vmware.vmx["numvcpus"] = "2"

    override.vm.box_url = 'http://files.vagrantup.com/precise64_vmware.box'
    override.vm.provision "shell", inline: <<-SHELL
# Ensure that VMWare Tools recompiles kernel modules
# when we update the linux images
sed -i.bak 's/answer AUTO_KMODS_ENABLED_ANSWER no/answer AUTO_KMODS_ENABLED_ANSWER yes/g' /etc/vmware-tools/locations
sed -i.bak 's/answer AUTO_KMODS_ENABLED no/answer AUTO_KMODS_ENABLED yes/g' /etc/vmware-tools/locations
SHELL

  end


  config.vm.provision "shell", inline: <<-SHELL
# Enable MongoDB Enterprise repo
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/stable multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise.list
sudo apt-get update

# Tools you can't live without
sudo apt-get install -y build-essential git vim

# I can't stand emacs
echo 'set -o vi' | sudo tee /etc/profile.d/vishell.sh

# Python stuff for mongo-orchestration
sudo apt-get install -y python python-dev python-pip

# Latest MongoDB Enterprise
sudo apt-get install -y mongodb-enterprise

# FIXME Replace the DELETE ME with this line when PR#153 has been merged
# pip install mongo-orchestration
####### DELETE ME
# Checkout orchestration from VCS and the latest pip doesn't support -b
git clone https://github.com/10gen/mongo-orchestration.git
pushd mongo-orchestration
wget https://github.com/10gen/mongo-orchestration/pull/153.patch
git am 153.patch
sudo python setup.py install
popd
####### DELETE ME

# Launch mongo-orchestration
mongo-orchestration -b 192.168.112.10 start
SHELL

end

