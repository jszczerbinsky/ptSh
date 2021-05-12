### About

ptSh is a packet of shell scripts, that run standard shell commands and display them in a prettier way.

### Features

Currently available scripts:

| Script name | Corresponding command | Available arguments |
| ------------ | ------------ | ------------ |
| `ptls` |  `ls` | `-l`, `-a`, `-A` |
| `ptpwd` | `pwd` | all |
| `ptmkdir` | `mkdir` | all |
| `pttouch` | `touch` | all |

You can customize ptSh scripts in many ways. Config file is in `/home/$USER/.config/ptSh/config`.

### Installation

You need to install a font, that supports unicode special symbols, like [NerdFonts](https://github.com/ryanoasis/nerd-fonts).

#### Automated installation

Just download an installation script from [releases](https://github.com/jszczerbinsky/ptSh/releases) and run it.

#### Manual installation

Clone this repository and use `make` to install it.

### Configuration

If you want to use ptSh scripts by default you can set aliases in your `.bashrc` file.
To automatically set all ptSh aliases, add this to your `.bashrc`:

```shell
source ptSh_set_aliases
```

### Some screenshots

![](/img/ptls.png)
![](/img/ptpwd.png)
