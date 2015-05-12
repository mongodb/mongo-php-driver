# Enable MongoDB Enterprise repo
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/3.0 multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise-3.0.list
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/2.6 multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise-2.6.list
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/2.4 multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise-2.4.list
apt-get update

apt-get install -y libsnmp15 libgsasl7

sudo apt-get download mongodb-enterprise-server=3.0.3
sudo apt-get download mongodb-enterprise-mongos=3.0.3
sudo apt-get download mongodb-enterprise-server=2.6.9
sudo apt-get download mongodb-10gen-enterprise=2.4.13
dpkg -x mongodb-10gen-enterprise_2.4.13_amd64.deb 2.4.13
dpkg -x mongodb-enterprise-server_2.6.9_amd64.deb 2.6.9
dpkg -x mongodb-enterprise-server_3.0.3_amd64.deb 3.0.3
dpkg -x mongodb-enterprise-mongos_3.0.3_amd64.deb 3.0.3



# Python stuff for mongo-orchestration
apt-get install -y python python-dev python-pip

pip install --upgrade 'git+https://github.com/10gen/mongo-orchestration.git#egg=mongo_orchestration'

# Launch mongo-orchestration
mongo-orchestration -f mongo-orchestration-config.json -b 192.168.112.10 start

