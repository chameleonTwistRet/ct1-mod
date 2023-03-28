# PracTwist v2

PracTwist is a mod for Chameleon Twist to enhance Speedrun practice - developed by [**Rainchus**](https://github.com/Rainchus/) and [**Ellie**]((https://github.com/Elisiah/)).

The [previous version](https://github.com/Rainchus/ChameleonTwistJP-Mod) of PracTwist was patched using custom boot assembly written by Rainchus. 

Version 2 is being built such that it will attach to our [Decompilation project](https://github.com/chameleonTwistRet/chameleonTwistv1.0-JP). This should allow for more flexibility and better readability of the code.

## Table of Contents:
- [Features](#features)
- [Installation for Users](#installation-for-users)
- [Installation for Developers](#installation-for-developers)

# Features

[TODO]

# Installation for Users
1. Download the latest release from the [releases page]()

2. Take a vanilla Chameleon Twist 1 JP rom in .z64 format and the latest release you just downloaded, patch them together using: https://hack64.net/tools/patcher.php

    **(Opt)**. If your Chameleon Twist JP rom isn't .z64 format, this site can swap it to that format https://hack64.net/tools/swapper.php 

# Installation for Developers

1. Setup the [Decompilation project](https://github.com/chameleonTwistRet/chameleonTwistv1.0-JP)
    TODO: Add instructions for this

2. From the root of the decomp run `./configure --mod`

3. `mkdir src/mod`

4. `git clone https://github.com/chameleonTwistRet/ct1-JP-SpeedrunMod.git src/mod/.`

5. run `ninja -t clean && ./configure --mod && ninja`

Now whenever you make changes to the mod, you can run `./configure --mod && ninja` to rebuild the mod.
