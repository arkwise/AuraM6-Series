#!/bin/bash

# Update Homebrew
brew update

# Install necessary tools
brew install wget unzip

# Set up DJGPP (this is a placeholder, setting up DJGPP on macOS might be more complex)
wget http://www.delorie.com/djgpp/zip/djdev203.zip
unzip djdev203.zip -d ~/djgpp/
export DJGPP=~/djgpp/djgpp.env

# Install Allegro (this is a placeholder, the exact version and method might vary)
brew install allegro

echo "Setup complete!"
