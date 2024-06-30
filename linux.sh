#!/bin/bash

# Update package lists
sudo apt-get update

# Install necessary tools
sudo apt-get install -y build-essential wget unzip git

# Download AuraM6 source code from GitHub
git clone https://github.com/arkwise/AuraM6.git ~/AuraM6

# Set up DJGPP
wget http://www.delorie.com/djgpp/zip/djdev203.zip
unzip djdev203.zip -d ~/djgpp/
export DJGPP=~/djgpp/djgpp.env

# Install Allegro 4.x (this is a placeholder, you might need to adjust for the exact version)
sudo apt-get install -y liballegro4-dev

echo "Setup complete!"
