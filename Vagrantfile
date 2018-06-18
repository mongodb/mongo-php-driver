# -*- mode: ruby -*-
# vi: set ft=ruby et sw=2 :

Vagrant.configure(2) do |config|

  config.vm.synced_folder ".", "/phongo"

  config.vm.provider "vmware_workstation" do |vmware, override|
    vmware.vmx["memsize"] = "8192"
    vmware.vmx["numvcpus"] = "2"
  end

  config.vm.provider "virtualbox" do |virtualbox|
    virtualbox.memory = 2048
    virtualbox.cpus = 2
  end

  config.vm.define "mo", primary: true do |mo|
    mo.vm.network "private_network", ip: "192.168.112.10"

    mo.vm.box = "ubuntu/trusty64"

    mo.vm.provision "shell", path: "scripts/ubuntu/essentials.sh", privileged: true
    mo.vm.provision "file", source: "scripts/ubuntu/get-pip.py", destination: "get-pip.py"
    mo.vm.provision "file", source: "scripts/ubuntu/mongo-orchestration-config.json", destination: "mongo-orchestration-config.json"
    mo.vm.provision "shell", path: "scripts/ubuntu/mongo-orchestration.sh", privileged: true
    mo.vm.provision "shell", path: "scripts/ubuntu/ldap/install.sh", privileged: true
  end

  config.vm.define "ldap", autostart: false do |ldap|
    ldap.vm.network "private_network", ip: "192.168.112.20"

    ldap.vm.box = "http://puppet-vagrant-boxes.puppetlabs.com/centos-64-x64-vbox4210-nocm.box"
    ldap.vm.provider "vmware_workstation" do |vmware, override|
      override.vm.box_url = "https://dl.dropbox.com/u/5721940/vagrant-boxes/vagrant-centos-6.4-x86_64-vmware_fusion.box"
      override.vm.provision "shell", path: "scripts/vmware/kernel.sh", privileged: true
    end

    ldap.vm.provision "shell", path: "scripts/centos/essentials.sh", privileged: true
    ldap.vm.provision "shell", path: "scripts/centos/ldap/install.sh", privileged: true
  end
end

