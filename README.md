# Modding Chameleon Twist

## Setup

- Have the chameleon twist decomp installed and be in the root folder
- `mkdir src/mod/`
- `cd src/mod/`
- `git clone https://github.com/chameleonTwistRet/ct1-mod.git .`
- `cd ../..`
- `./configure --mod`
- `ninja`

## Making Changes

From the root of the decomp anytime you run `ninja` it will build.

If you wish to build vanilla again run `./configure` and if you wish to build mods again run `./configure --mod`

After every change to a mod run: `make clean`