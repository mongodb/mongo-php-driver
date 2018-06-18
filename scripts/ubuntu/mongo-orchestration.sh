# 3.0
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10

# 3.6
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2930ADAE8CAF5059EE73BB4B58712A2291FA4AD5

# 4.0
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 68818C72E52529D4

echo 'deb http://repo.mongodb.com/apt/ubuntu trusty/mongodb-enterprise/3.0 multiverse' | tee /etc/apt/sources.list.d/mongodb-enterprise-3.0.list
echo 'deb http://repo.mongodb.com/apt/ubuntu trusty/mongodb-enterprise/3.6 multiverse' | tee /etc/apt/sources.list.d/mongodb-enterprise-3.6.list
echo 'deb http://repo.mongodb.com/apt/ubuntu trusty/mongodb-enterprise/testing multiverse' | tee /etc/apt/sources.list.d/mongodb-enterprise-4.0.list

apt-get update

apt-get install -y libsnmp30 libgsasl7 libcurl4-openssl-dev

apt-get download mongodb-enterprise-server=3.0.15
apt-get download mongodb-enterprise-server=3.6.1
apt-get download mongodb-enterprise-mongos=3.6.1
apt-get download mongodb-enterprise-server=4.0.0~rc5
dpkg -x mongodb-enterprise-server_3.0.15_amd64.deb 3.0
dpkg -x mongodb-enterprise-server_3.6.1_amd64.deb 3.6
dpkg -x mongodb-enterprise-mongos_3.6.1_amd64.deb 3.6
dpkg -x mongodb-enterprise-server_4.0.0~rc5_amd64.deb 4.0

# Python stuff for mongo-orchestration
apt-get install -y python python-dev
python get-pip.py

pip install --upgrade mongo-orchestration

# Launch mongo-orchestration
mongo-orchestration -f mongo-orchestration-config.json -b 192.168.112.10 --enable-majority-read-concern start
