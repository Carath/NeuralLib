#!/bin/sh

# This script purpose is to clean the whole project folder, and compress it
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


# Cleaning not useful network saves:
notUsedSavesFolder=MNIST_learning/saves/
echo '\n'//////////////////////////////////////////////////
echo // Cleaning the folder: $notUsedSavesFolder
echo //////////////////////////////////////////////////'\n'
rm -rf $notUsedSavesFolder*
echo Done.


# Compressing the whole project folder:
projectFolder=$(basename -- "$PROJECT_PATH")
echo '\n'//////////////////////////////////////////////////
echo // Compressing the folder: $projectFolder
echo //////////////////////////////////////////////////'\n'
tar -czf ../$projectFolder.tgz ../$projectFolder
echo Done.


exit
