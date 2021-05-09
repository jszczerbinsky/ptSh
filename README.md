### About

ptSh is a packet of shell scripts, that run standard shell commands and display them in a prettier way.

### Features

Currently available scripts:

| Script name | Corresponding command | Available arguments |
| ------------ | ------------ | ------------ |
| `ptls` |  `ls` | `-l` |
| `ptpwd` | `pwd` | |
| `ptmkdir` | `mkdir` | all |
| `pttouch` | `touch` | all |

You can customize ptSh scripts in many ways. Config file is in `/home/$USER/.config/ptSh/config`.

### Installation

Just clone this repository and use `make` to install it

If you want to use ptSh scripts by default you can set aliases in your `.bashrc` file.
To automatically set all ptSh aliases, add this to your `.bashrc`:

```shell
source ptSh_set_aliases
```

### Some screenshots

![](/img/ptls.png)
![](/img/ptpwd.png)
