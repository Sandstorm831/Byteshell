# Byteshell

## Description
Here is a code for the Project which implements a basic shell in C, a Shell is a program that takes the command inputs written from the user’s keyboard and passes them to the machine to execute them through the kernel. It also verifies if the command input from the user is correct, it is a type of computer program called a command-line interpreter that lets Linux and Unix users control their operating systems with command-line interfaces. Shells allow users to communicate efficiently and directly with their operating systems. 

## List of functions
- `chdir`
- `waitpid`
- `strcmp`
- `fork`
- `getline`
- `malloc`
- `strtok`
- `free`

## List of implemented Built-ins
- `cd`
- `history`
- `alias`
- `unalias`
- `help`
- `exit`
- `env`
- `ls`
- `echo`

## Examples
```
> echo "hello_world"
hello_world
```
-------
```
> env
SHELL=/bin/bash
SESSION_MANAGER=local/rootx-HP-Laptop-15-da0xxx:@/tmp/.ICE-unix/2078,unix/rootx-HP-Laptop-15-da0xxx:/tmp/.ICE-unix/2078
QT_ACCESSIBILITY=1
.
.
.
GIO_LAUNCHED_DESKTOP_FILE=/var/lib/snapd/desktop/applications/codium_codium.desktop
GOPATH=/home/rootx/golang:/home/rootx/gowork
TERM_PROGRAM=vscode
OLDPWD=/home/rootx/byteshell
_=/home/rootx/byteshell/byteshell
```
----
```
> ls -l
total 1432
-rwxrwxr-x 1 rootx rootx   15968 Jun  9 00:05 byte
-rwxrwxr-x 1 rootx rootx   21760 Jun 25 01:35 byteshell
-rw-rw-r-- 1 rootx rootx    7040 Jun 25 00:13 byteshell.c
drwxrwxr-x 2 rootx rootx    4096 Jun 13 17:56 coder
-rwxrwxr-x 1 rootx rootx   16008 Jun 23 18:21 exper
-rw-rw-r-- 1 rootx rootx     243 Jun 23 18:21 exper.c
drwxrwxr-x 2 rootx rootx    4096 Jun 25 01:13 final_project
-rwxrwxr-x 1 rootx rootx   16088 Jun  8 23:58 fork
-rw-rw-r-- 1 rootx rootx     533 Jun 10 00:33 fork.c
-rwxrwxr-x 1 rootx rootx   16056 Jun  9 00:23 identify_zombie
-rw-rw-r-- 1 rootx rootx     898 Jun  9 00:54 identify_zombie.c
-rwxrwxr-x 1 rootx rootx   21752 Jun 25 00:49 main
-rwxrwxr-x 1 rootx rootx   16176 Jun 10 23:33 read_line
-rw-rw-r-- 1 rootx rootx     791 Jun 16 00:04 read_line.c
-rw-rw-r-- 1 rootx rootx 1284014 Jun  7 16:30 Week_1_Byteshell_1.pptx
-rwxrwxr-x 1 rootx rootx   16096 Jun  9 00:00 zombie
```
