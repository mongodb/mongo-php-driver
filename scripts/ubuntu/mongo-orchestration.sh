# Enable MongoDB Enterprise repo
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/stable multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise.list
sudo apt-get update

# Python stuff for mongo-orchestration
sudo apt-get install -y python python-dev python-pip

# Latest MongoDB Enterprise
sudo apt-get install -y mongodb-enterprise

# FIXME Replace the DELETE ME with this line when PR#153 has been released
# pip install mongo-orchestration
####### DELETE ME
# Checkout orchestration from VCS and the latest pip doesn't support -b
git clone https://github.com/10gen/mongo-orchestration.git
pushd mongo-orchestration
wget https://github.com/10gen/mongo-orchestration/pull/161.patch
git am 161.patch
sudo python setup.py install
popd
####### DELETE ME

# Launch mongo-orchestration
mongo-orchestration -b 192.168.112.10 start

