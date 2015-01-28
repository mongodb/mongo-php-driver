# Enable MongoDB Enterprise repo
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://repo.mongodb.com/apt/ubuntu precise/mongodb-enterprise/stable multiverse' | sudo tee /etc/apt/sources.list.d/mongodb-enterprise.list
sudo apt-get update

# Python stuff for mongo-orchestration
sudo apt-get install -y python python-dev python-pip

# Latest MongoDB Enterprise
sudo apt-get install -y mongodb-enterprise

sudo pip install --upgrade 'git+https://github.com/10gen/mongo-orchestration.git#egg=mongo_orchestration'

# Launch mongo-orchestration
mongo-orchestration -b 192.168.112.10 start

