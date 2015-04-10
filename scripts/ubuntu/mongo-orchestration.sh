# Enable MongoDB Enterprise repo
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/stable multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise.list
apt-get update

# Python stuff for mongo-orchestration
apt-get install -y python python-dev python-pip

# Latest MongoDB Enterprise
apt-get install -y mongodb-enterprise

# mongo-orchestration doesn't play well with pymongo3
# https://github.com/10gen/mongo-orchestration/issues/188
pip install pymongo==2.8

pip install --upgrade 'git+https://github.com/10gen/mongo-orchestration.git#egg=mongo_orchestration'

# Launch mongo-orchestration
mongo-orchestration -b 192.168.112.10 start

