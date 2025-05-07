# A Massively Parallel Buchberger Algorithm

This package provides a massively parallel implementation of Buchbergers algorithm.
The application relies on the task-based workflow provided by [GPI-Space](http://www.gpi-space.de/) for task coordination, and uses the computer algebra  system [Singular](https://www.singular.uni-kl.de/) for computational tasks.

We also provide a massively parallel [Buchberger test](https://github.com/singular-gpispace/gspc-bb/blob/BB_TEST/README.md). Here we check that the given generating system is a Gröbner basis. Based on the verification, there is also a verified modular Buchberger algorithm, which builds on the generic modular framework for algebraic geometry, see [verified modular Buchberger](https://github.com/singular-gpispace/modular/tree/MODULAR_STD_WITH_VERIFICATION). Here both the Gröbner basis property as well as the key inclusion are tested, which yields a verification in the homogeneous case.

Note that all of this is work in progress.

This application builds on the Singular dynamical module implemented by Lukas Ristau from the repository
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

##  Examples for using gspc_buchberger
The following is a minimal example for using the parallel implementation of Buchberger's
algorithm to compute a Gröbner basis.

Start SINGULAR in install_dir...
```bash
SINGULARPATH="$GPISpace_Singular_buchberger/install_dir"  Singular

```

...and run
```bash
LIB "buchbergergspc.lib";
LIB "random.lib";

configToken gc = configure_gspc();

gc.options.tmpdir = "tempdir";
gc.options.nodefile = "nodefile";
gc.options.procspernode = 6;
gc.options.loghostfile = "loghostfile";
gc.options.logport = 3217;

ring r = 0,x(1..7),dp;
ideal I = randomid(maxideal(2),5);   

ideal G = gspc_buchberger(I, gc, 5);

```

```gspc_buchberger``` takes an additional argument, here set to 5, that determines the maximal number of s-polynomial reductions running in parallel. We found it is best set to the total number of cores minus one, i.e. (number of nodes x procspernode - 1).

For more examples including procedures to display timings and additional information about the algorithm refer to example.lib
You can run example.lib with
```bash
cd $GPISpace_Singular_buchberger/install_dir
SINGULARPATH="$GPISpace_Singular_buchberger/install_dir"  Singular example.lib

```

Be sure to always use the spack installation of Singular by loading it as described above with ```spack load singular```

## Timings (from the Master's Thesis)
Examples used:
* RANDOM: An ideal generated by 7 homogeneous quadrics with random coeffi-
cients in 9 variables, generated in Singular as ```system("--random",42); ring
r=0,x(1..9),dp; ideal I = randomid(maxideal(2),7);```
* KAT_n_HOM: homogenization of the katsura ideal with n generators, generated in
Singular as ```ring r=0,x(0..n),dp; ideal I = homog(katsura(n),x(n));```
* CYCLIC_n_HOM: homogenization of the cyclic ideal with n generators, generated in
Singular as ```ring r=0,x(0..n),dp; ideal I = homog(cyclic(n),x(n));```
* M_RATIONAL: The sum M = M1 + M2 of two modules over the polynomial ring
Q(s, t)[z1, . . . , z15] arising in the computation of IBP relations for Feynman integrals using the module intersection method. (The underlying Feynman diagram is
the “tenniscourt” diagram.)
* M_BLOCK_ORD: the same module but with respect to a block ordering ```(dp(15),dp(2))``` using s and t as two additional variables (in the second block)


The following tables show timings in seconds of the parallel implementation compared to SINGULAR's standard (serial) implementation ```std``` as well as speedup and parallel efficiency using different amounts of cores:

##### RANDOM
cores | gspc_buchberger | speedup | efficiency | std
  --- | ---  | ---  | ---    | ---
   1  | 8986 | 1.00 | 100\%  | 8317
   2  | 4708 | 1.91 | 95.4\% | --
   4  | 2451 | 3.66 | 91.7\% | --
   8  | 1265 | 7.10 | 88.8\% | --
   16 | 773  | 11.6 | 72.7\% | --
   32 | 529  | 17.0 | 53.1\% | --
   48 | 485  | 18.5 | 38.6\% | --

##### KAT_11_HOM
cores | gspc_buchberger | speedup | efficiency | std
  --- | ---  | ---  | ---    | ---
   1  | 1494  | 1.00 | 100\%  | 1495
   2  | 1195  | 1.25 | 62.5\% | --
   4  | 1015  | 1.47 | 36.8\% | --
   8  | 915   | 1.63 | 20.4\% | --
   16 | 854   | 1.75 | 10.9\% | --
   32 | 798   | 1.87 | 5.85\% | --
   48 | 762   | 1.96 | 4.08\% | --

##### M_RATIONAL
cores | gspc_buchberger | speedup | efficiency | std
  --- | ---  | ---  | ---    | ---
   1  | 1158 | 1.00 | 100\% | 20608
   2  | 692  | 1.67 | 83.7\% | --
   4  | 465  | 2.49 | 62.3\% | --
   8  | 317  | 3.65 | 45.7\% | --
   16 | 249  | 4.65 | 29.1\% | --
   32 | 202  | 5.73 | 17.9\% | --
   48 | 197  | 5.87 | 12.3\% | --

##### M_BLOCK_ORD
cores | gspc_buchberger | speedup | efficiency | std
  --- | ---  | ---  | ---    | ---
   1  | 2406 | 1.00 | 100\%  | 15267
   2  | 1481 | 1.62 | 81.2\% | --
   4  | 995  | 2.42 | 60.5\% | --
   8  | 824  | 2.92 | 36.5\% | --
   16 | 742  | 3.24 | 20.3\% | --
   32 | 694  | 3.47 | 10.9\% | --
   48 | 663  | 3.63 | 7.56\% | --

Comparing the parallel implementation (```gspc_buchberger```) with SINGULAR's ```std``` as well as the two implementations of modular methods, SINGULAR's ```modStd``` and the SINGULAR/GPI-Space implementation ```gspc_modular``` (the time they take for verifying the result with a Buchberger Test being measured separately):

##### Comparison of the different implementations of Buchberger's algorithm, using 48 cores (all times in seconds)
example | std | modStd | verification | gspc_buchberger | gspc_modular | verification
---          | ---    | ---  | ---  |---   | ---  | ---  
RANDOM       | 8317   | 968  | 10.3 | 485  | 7532 | 10.8
CYCLIC_8_HOM | 196313 | 447  | 9.3  | --   | 181  | 11.8
KAT_11_HOM   | 1495   | 350  | 8.9  | 762  | 190  | 12.3
KAT_12_HOM   | 95554  | 3330 | 134  | 4209 | 1785 | 147
M_RATIONAL   | 20608  | --   | 2.82 | 197  | --   | 0.28
M_BLOCK_ORD  | 15267  | --   | 2.41 | 663  | --   | 1.98

## Additional Timings
### Comparison with the F4 algorithm

The examples used here are ideals generated by the 2x2-minors of a 2x7 Hankel-matrix
```
v1 v2 v3 v4 v5 v6 v7
v2 v3 v4 v5 v6 v7 v8
```
where ```v1,...,v8``` are generic polynomials in the ring Q[x1,...,x8] with coefficients chosen at random between -20 and 20.
The first column in the following table are the degrees of ```v1,...,v8```
We compared the runtimes (in seconds) of ```gspc_buchberger``` on 48 cores, OSCAR's implementation of the F4 algorithm ```groebner_basis_f4```, as well as SINGULAR's ```std```:

##### Runtimes of the parallel Buchberger, OSCAR's implementation of F4 and SINGULAR's std
degrees of v1,...,v8 | gspc_buchberger | groebner_basis_f4 | ratio of runtimes | std
--- | --- | --- | --- | ---
1,1,1,1,1,1,2,3  | 1.43 | 0.373 | 3.82 | 2.64
1,1,1,1,1,1,2,4  | 1.93 | 0.890 | 2.17 | 7.06
1,1,1,1,1,1,2,5  | 3.05 | 2.06  | 1.48 | 17.2
1,1,1,1,1,1,2,6  | 5.66 | 5.04  | 1.12 | 32.8
1,1,1,1,1,1,2,7  | 9.94 | 12.4  | 0.80 | 57.5
1,1,1,1,1,1,3,3  | 3.79 | 1.54  | 2.46 | 20.5
1,1,1,1,1,1,3,4  | 7.61 | 3.36  | 2.27 | 55.1
1,1,1,1,1,1,3,5  | 19.4 | 6.64  | 2.93 | 135
1,1,1,1,1,1,3,6  | 36.7 | 16.01 | 2.29 | 275
1,1,1,1,1,1,3,7  | 66.6 | 31.4  | 2.12 | 524
1,1,1,1,1,2,2,2  | 3.79 | 0.580 | 6.54 | 17.6
1,1,1,1,1,2,2,3  | 17.2 | 2.04  | 8.44 | 85.1
1,1,1,1,1,2,2,4  | 40.7 | 5.91  | 6.88 | 272

The F4 algorithm is faster than the parallel implementation by a factor of up to ~10, in some examples it is even slower, while both implementations are significantly faster than SINGULAR's ```std```.
