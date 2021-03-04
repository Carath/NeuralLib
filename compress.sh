#!/bin/sh

# This script purpose is to clean the whole project directory, and compress it
# to an archive placed in the same directory than the project is.
# Launch it with the command:
# sh compress.sh


# Safer to get this at the beginning, since we may move between directories.
PROJECT_PATH=$PWD


clean()
{
	cd $1
	echo '\n'//////////////////////////////////////////////////
	echo // Cleaning /$1/:
	echo //////////////////////////////////////////////////'\n'
	make clean
	cd ..
}


clean NeuralLib
clean MNIST_learning
clean MNIST_animation


# Cleaning unnecessary network saves:
notUsedSavesDirectory=MNIST_learning/saves
echo '\n'//////////////////////////////////////////////////
echo // Cleaning the directory: $notUsedSavesDirectory
echo //////////////////////////////////////////////////'\n'
rm -rf $notUsedSavesDirectory/*
echo Done.


# Compressing the whole project directory:
projectDirectory=$(basename -- "$PROJECT_PATH")
echo '\n'//////////////////////////////////////////////////
echo // Compressing the directory: $projectDirectory
echo //////////////////////////////////////////////////'\n'
tar -czf ../$projectDirectory.tgz ../$projectDirectory
echo Done.


exit
