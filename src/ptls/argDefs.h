#ifndef PTLS_ARG_DEFS
#define PTLS_ARG_DEFS

#define HELP_ARG                  ' '
#define HELP_ARG_W                "--help"
#define HELP_ARG_DESC             "Shows help"

#define ALL_ARG                   'a'
#define ALL_ARG_W                 "--all"
#define ALL_ARG_DESC              "Displays all files in directory"

#define ALMOST_ALL_ARG            'A'
#define ALMOST_ALL_ARG_W          "--almost-all"
#define ALMOST_ALL_ARG_DESC       "Displays all files in directory, except of . and .."

#define IGNORE_BACKUPS_ARG        'B'
#define IGNORE_BACKUPS_ARG_W      "--ignore-backups"
#define IGNORE_BACKUPS_ARG_DESC   "Ignores files, that ends with ~"

#define L_ARG                     'l'
#define L_ARG_W                   ""
#define L_ARG_DESC                "Displays more informations about a file"

#define INODE_ARG                 'i'
#define INODE_ARG_W               "--inode"
#define INODE_ARG_DESC            "Shows file inode"

#define NO_GROUP_ARG              'G'
#define NO_GROUP_ARG_W            "--no-group"
#define NO_GROUP_ARG_DESC         "Ignores information about group"

#define TIME_ARG                  't'
#define TIME_ARG_W                ""
#define TIME_ARG_DESC             "Shows last modifiaction time as HH:MM"

#define FULL_TIME_ARG             'T'
#define FULL_TIME_ARG_W           "--full-time"
#define FULL_TIME_ARG_DESC        "Shows last modification time as HH:MM:SS"

#define FULL_SIZE_ARG             'S'
#define FULL_SIZE_ARG_W           "--full-size"
#define FULL_SIZE_ARG_DESC        "Shows file size in bytes"

#define DECIMAL_SIZE_ARG          'D'
#define DECIMAL_SIZE_ARG_W        "--decimal"
#define DECIMAL_SIZE_ARG_DESC     "Shows size in decimal units instead of binary"

#define NUMERIC_UID_GID_ARG       'n'
#define NUMERIC_UID_GID_ARG_W     "--numeric-uid-gid"
#define NUMERIC_UID_GID_ARG_DESC  "Displays UID and GID numbers instead of names"

#define REVERSE_ARG               'r'
#define REVERSE_ARG_W             "--reverse"
#define REVERSE_ARG_DESC          "Reverses sorting order"

#define S_X_ARG                   'x'
#define S_X_ARG_W                 ""
#define S_X_ARG_DESC              "Sort by extension instead of name"

#define S_TIME_ARG                'm'
#define S_TIME_ARG_W              ""
#define S_TIME_ARG_DESC           "Sort by last modification time instead of name"

#define NO_DIRS_TOP_ARG           ' '
#define NO_DIRS_TOP_ARG_W         "--no-dirs-top"
#define NO_DIRS_TOP_ARG_DESC      "Don't group directories on top"

#endif
