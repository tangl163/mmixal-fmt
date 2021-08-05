## mmixal-fmt is a tool for formatting MMIXAL source code.

### Installation

```bash
make
make install
```

By default, the `mmixal-fmt` will be installed in `/usr/local/bin/`

### Usage

`mmixal-fmt` will read **MMIXAL** ([**MMIX**](https://mmix.cs.hm.edu/index.html) Assemble Language)
from `stdin` and output to the `stdout`. In fact, this tool is intended to integrate with this [vim plugin](https://github.com/tangl163/vim-mmixal-fmt).


Assuming below is what `mmix.mms` contains

```mmix
 % Example program to format
L IS 500        This is comment
t  IS $255   This is another comment
n GREG      % Comment
r GREG  // Comment
jj GREG 0        Comment
   PBN mm,2B
 LDA t,NewLn; TRAP 0,Fputs,StdOut
 CMP t,mm,2*(L/10-1A) ; PBNZ t,3B;       TRAP 0,Halt,0
```

After executing `cat mmix.mms | mmixal-fmt`, the output will be

```mmix
% Example program to format
L   IS    500               This is comment
t   IS    $255              This is another comment
n   GREG                    % Comment
r   GREG                    // Comment
jj  GREG  0                 Comment
    PBN   mm,2B
    LDA   t,NewLn
    TRAP  0,Fputs,StdOut
    CMP   t,mm,2*(L/10-1A)
    PBNZ  t,3B
    TRAP  0,Halt,0
```

And this

```mmix
t is $255
    LOC Data_Segment
    Greg @ Base Address
string BYTE "Hello, World!",#a,0

    LOc #100
Main LDA t,string
    TRAP 0,Fputs,StdOut
    Trap 0,Halt,0
```

Will be formatted as

```mmix
t       IS    $255
        LOC   Data_Segment
        GREG  @                     Base Address
string  BYTE  "Hello, World!",#a,0

        LOC   #100
Main    LDA   t,string
        TRAP  0,Fputs,StdOut
        TRAP  0,Halt,0
```

