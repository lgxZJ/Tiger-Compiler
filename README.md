A Tiger Compiler
----------------
[![Build Status](https://travis-ci.org/lgxZJ/Tiger-Compiler.svg?branch=master)](https://travis-ci.org/lgxZJ/Tiger-Compiler) [![Coverage Status](https://coveralls.io/repos/github/lgxZJ/Tiger-Compiler/badge.svg?branch=master)](https://coveralls.io/github/lgxZJ/Tiger-Compiler?branch=master)


## The Tiger Language
Written by **Andrew W.Appel**, the author of [***Tiger Book***][tiger-book], the Tiger language is a small language with nested functions, records values with implicit pointers, arraya, integer and string variables, and a few simple structured control constructs.

For more details, see the **Appendix** section of that book.

## Prerequisites
Assumed you are in the root folder.

### CentOS
1.  `./travis_scripts/install_centos.sh`

### CentOS Docker
1.  `./travis_scripts/install_centos_docker.sh`

## Build
1.  `cd Compiler`
2.  `make`

Then the generated compiler will be hosted in `builds` folder named **tiger**.

## Compiler Usage
1.  `cd Compiler`
2.  `./builds/tiger-compiler ./testcases/testXXX.tig`

The generated executable file will be under the `testcases` folder with name `testXXX`.

## Debug
### Debug with STL
I'll make this a prerequisite which is going to be included in the above shell scripts. At this time you can setup all necessary stuffs using instructions at this site http://sourceware.org/gdb/wiki/STLSupport.

### Debug in Docker
Lanuch your docker with following options added `--privileged  --cap-add=SYS_PTRACE`.


## Unit Test
### Module Status
![Module-Status](https://i.loli.net/2020/01/29/SKU6dyPMsgNFqa5.png)

### Module Coverage 
First make sure that you have installed a browser such as **firefox**, the commands following will try to open the results via it.

1.  `cd Compiler/Coverage-Report`.
2.  `make`.

## Todo
See [Compiler/TODO](Compiler/TODO).


[tiger-book]:		http://www.amazon.com/Modern-Compiler-Implementation-Basic-Techniques/dp/0521583896
[build-status-img]:	https://travis-ci.org/lgxZJ/Tiger-Compiler.svg?branch=master
