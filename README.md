# A Massively Parallel Buchberger Test

This package provides a massively parallel implementation of the Buchbergers test.
The application relies on the task-based workflow provided by [GPI-Space](http://www.gpi-space.de/) for task coordination, and uses the computer algebra  system [Singular](https://www.singular.uni-kl.de/) for computational tasks.

We also provide a massively parallel [Buchberger algorithm](https://github.com/singular-gpispace/gspc-bb/README.md). Here we compute from a given generating system a Gröbner basis.

This application  uses the Singular dynamic module implemented by Lukas Ristau from  the repository
[framework](https://github.com/singular-gpispace/framework).

# Installation using Spack
Spack is a package manager specifically aimed at handling software installations in supercomputing environments, but
usable on anything from a personal computer to an HPC cluster. It supports Linux and macOS (note that the Singular/GPI-Space
framework and hence our package requires Linux).

We will assume that the user has some directory path with read and
write access. In the following, we assume this path is set as the environment variable
`software_ROOT`, as well as `install_ROOT`:

```bash
export software_ROOT=~/singular-gpispace
export install_ROOT=~/singular-gpispace

```
Note, this needs to be set again if you open a new terminal session (preferably set it automatically by adding the line to your .profile file).

## Install Spack
If Spack is not already present in the above directory, clone Spack from Github:
```bash
git clone https://github.com/spack/spack.git $software_ROOT/spack

```
We check out verison v0.21 of Spack (the current version):
```bash
cd $software_ROOT/spack
git checkout releases/v0.21
cd $software_ROOT

```
Spack requires a couple of standard system packages to be present. For example, on an Ubuntu machines they can be installed by the following commands (which typically require sudo privilege)

```bash
sudo apt update

```
```bash
sudo apt install build-essential ca-certificates coreutils curl environment-modules gfortran git gpg lsb-release python3 python3-distutils python3-venv unzip zip

```

To be able to use spack from the command line, run the setup script:
```bash
. $software_ROOT/spack/share/spack/setup-env.sh

```
Note, this script needs to be executed again if you open a new terminal session (preferably set it automatically by adding the line to your .profile file).

Finally, Spack needs to boostrap clingo.  This can be done by concretizing any
spec, for example
```bash
spack spec zlib

```

Note: If you experience connection timeouts due to a slow internet connection you can set in the following file the variable `connect_timeout` to a larger value.
```bash
vim $software_ROOT/spack/etc/spack/defaults/config.yaml

```

### How to uninstall Spack
Note that Spack can be uninstalled by just deleting its directory and its configuration files. Be CAREFUL to do that, since it will delete your Spack setup. Typically you do NOT want to do that now, so the code is commented out. It can be useful if your Spack installation is broken:

```bash
#cd
#rm -rf $software_ROOT/spack/
#rm -rf .spack

```
## Install Singular and GPI-Space via spack

Once you have installed Spack, you need to install GPI-Space and Singular.

Clone the Singular/GPI-Space package repository into this directory:
```bash
git clone https://github.com/singular-gpispace/spack-packages.git $software_ROOT/spack-packages

```

Add the Singular/GPI-Space package repository to the Spack installation:
```bash
spack repo add $software_ROOT/spack-packages

```

Finally, install GPI-Space:
```bash
spack install gpi-space

```
install Singular:
```bash
spack install singular@4.3.0

```

Note, this may take quite a bit of time, when doing the initial installation, as it needs to build GPI-Space and Singular
including dependencies.

## Set up ssh

GPI-Space requires a working SSH environment with a password-less
SSH-key when using the SSH RIF strategy. To ensure this,
leave the password field empty when generating your ssh keypair.

By default, `${HOME}/.ssh/id_rsa` is used for authentication. If no such key exists,
```bash
ssh-keygen -t rsa -b 4096 -N '' -f "${HOME}/.ssh/id_rsa"

```
can be used to create one.

If you compute on your personal machine, there must run an ssh server. On an Ubuntu machine, the respective package can be installed by:

```bash
sudo apt install openssh-server

```

Your key has to be registered with the machine you want to compute on. On a cluster with shared home directory, this only has to be done on one machine. For example, if you compute on your personal machine, you can register the key with:
```bash
ssh-copy-id -f -i "${HOME}/.ssh/id_rsa" "${HOSTNAME}"

```

## compile gspc_buchberger


Define an install directory.
```bash

export GPISpace_Singular_buchberger="<install-directory>"

```

Find the installation path of gpi-space.
```bash
export GPISpace_install_dir="$software_ROOT/spack/opt/spack/.../gcc-12.2.0/gpi-space-23.06-..."

```

Make the build and install directories
```bash
mkdir $GPISpace_Singular_buchberger/build_dir
mkdir $GPISpace_Singular_buchberger/install_dir
mkdir $GPISpace_Singular_buchberger/install_dir/tempdir

```

clone the gspc-bb repository.
```bash
cd $GPISpace_Singular_buchberger
git clone https://github.com/singular-gpispace/gspc-bb.git

```

Start spack and load singular and gpi-space
```bash
. $software_ROOT/spack/share/spack/setup-env.sh

spack load gpi-space
spack load singular

```

Compile the project and copy necessary libraries into install_dir.
```bash
cmake                                                                         \
  -D Boost_USE_DEBUG_RUNTIME=OFF  					      \
  -D GSPC_WITH_MONITOR_APP=ON					              \
  -D GPISpace_ROOT=$GPISpace_install_dir                                      \
  -D CMAKE_INSTALL_PREFIX=$GPISpace_Singular_buchberger/install_dir           \
  -B $GPISpace_Singular_buchberger/build_dir                                  \
  -S $GPISpace_Singular_buchberger/gspc-bb

cmake                                                                         \
  --build $GPISpace_Singular_buchberger/build_dir                             \
  --target install                                                            \
  -j $(nproc)


cp $GPISpace_Singular_buchberger/install_dir/share/examples/buchbergergp.lib $GPISpace_Singular_buchberger/install_dir
cp $GPISpace_Singular_buchberger/gspc-bb/examples/example.lib $GPISpace_Singular_buchberger/install_dir

```

Create nodefile and loghostfile in the install directory.
```bash
cd $GPISpace_Singular_buchberger/install_dir

hostname > nodefile
hostname > loghostfile

```

Start the example script.
```bash
SINGULARPATH="$GPISpace_Singular_buchberger/install_dir"  Singular example.lib

```
