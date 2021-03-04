#!/bin/sh

# Building OpenBLAS from source in Ubuntu.

# Choosing a version from: https://github.com/xianyi/OpenBLAS/releases/
# Note that this should be more stable than the last version on the develop branch.
# Futhermore, the master branch is quite outdated.
VERSION='0.3.13'

# Directory where to install OpenBLAS (in its own dir):
DIR='/opt'

# For Ubuntu. Use yum for CentOS / Red Hat.
PACKAGE_MANAGER=apt

sudo $PACKAGE_MANAGER install cmake gfortran

wget https://github.com/xianyi/OpenBLAS/releases/download/v$VERSION/OpenBLAS-$VERSION.zip
unzip -q OpenBLAS-$VERSION.zip
cd OpenBLAS-$VERSION

# This step may take some time to complete:
make
sudo make install PREFIX=$DIR/OpenBLAS

# Finally setting an environment variable:
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$DIR/OpenBLAS/lib/" >> ~/.bashrc
echo "Done."
