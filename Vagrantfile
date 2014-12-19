# -*- mode: ruby -*-
# vi: set ft=ruby et sw=2 :

Vagrant.configure(2) do |config|

  config.vm.synced_folder ".", "/phongo"

  config.vm.provider "vmware_workstation" do |vmware, override|
    vmware.vmx["memsize"] = "8192"
    vmware.vmx["numvcpus"] = "2"
    override.vm.provision "shell", path: "scripts/vmware/kernel.sh"
  end

  config.vm.define "default", primary: true do |mo|
    mo.vm.network "private_network", ip: "192.168.112.10"

    mo.vm.box = "http://files.vagrantup.com/precise64.box"
    mo.vm.provider "vmware_workstation" do |vmware, override|
      override.vm.box_url = 'http://files.vagrantup.com/precise64_vmware.box'
    end

    mo.vm.provision "shell", path: "scripts/ubuntu/essentials.sh"
    mo.vm.provision "shell", path: "scripts/ubuntu/mongo-orchestration.sh"
  end

  config.vm.define "ldap", autostart: false do |ldap|
    ldap.vm.network "private_network", ip: "192.168.112.20"

    ldap.vm.box = "http://puppet-vagrant-boxes.puppetlabs.com/centos-64-x64-vbox4210-nocm.box"
    ldap.vm.provider "vmware_workstation" do |vmware, override|
      override.vm.box_url = "https://dl.dropbox.com/u/5721940/vagrant-boxes/vagrant-centos-6.4-x86_64-vmware_fusion.box"
    end

    ldap.vm.provision "shell", path: "scripts/centos/essentials.sh"
    #ldap.vm.provision "shell", path: "scripts/centos/mongo-orchestration.sh"
    ldap.vm.provision "shell", path: "scripts/centos/ldap/install.sh"
  end

end

