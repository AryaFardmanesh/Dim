# Dim JavaScript runtime
Dim is a runtime engine for JavaScript. It is built on top of QuickJS.
Basically, Dim is a very small engine that implements some of the functions available in the C language for JavaScript.
The important thing to note is that Dim is not guaranteed to work in the real world and is developed for fun purposes only.

# Installation
First you need to clone the Dim project from GitHub.
```sh
git clone https://github.com/AryaFardmanesh/Dim.git
cd Dim
```
Then you need to run the following commands in the Dim directory to do the initial work to compile the program.
```sh
make compile_qjs
make init
make
```
The `compile_qjs` command starts compiling the QuickJS library, the `init` command also creates the directories necessary to compile the project and moves the compiled version of QuickJS to the `obj` directory in the project root, and then creates an archive of all the object files and places it in the project root.
The last command, when executed, compiles the Dim engine once and then runs the `sample/main.js` file with the compiled Dim engine.

# Dim command
For final use, just run the dim binary file in the /out directory. To run a JavaScript file, you should do the following command:
```sh
cd out
dim.exe ./my_file_name.js
```

# Licence
This project is licensed under the [GPL-v3.0](https://github.com/AryaFardmanesh/Dim/blob/main/LICENSE) license.
