# **BB_eval**

## Introduction
bb_eval is a simple tool to evaluate software exercises submitted to the well-known BlackBoard platform. This tool is specially intended for introdution to C/C++ programming. 

bb_eval makes use of the Blackboard database of student submissions to a given task, and also the BlackBoard student list. It launches a given evaluator for each submitted exercise. The evaluator (made on purpose for each exercise) executes student source and scores it according to the teacher needs.  

Once all the exercises submitted by the students are evaluated, a .xls file is created with the grades computed for each student. This file can be diretcly upload to BlackBoard in order to make available the grades to the students.

## Dependencies
This is a simple C++ project running on Linux. Although it was tested on ubuntu, it should compile and execute smoothly in most Linux-like systems. The two major dependencies are g++ and CMake tools.

## Compilation and instalation
Download this source code into your preferred directory:
```
$ git clone https://github.com/fercabe/bb_eval
```
Create the build directory and move into it:
```
$ mkdir ./bb_eval/build
$ cd ./bb_eval/build
```
Compile the project:
```
$ cmake ..
$ make
```
Install the bb_eval binary into system. This is optional, but very combinient:
```
$ sudo make install
```
Yoou can uninstall the binaries for your system by running the following command:
```
$ sudo make uninstall
```
## Running bb_eval
bb_eval searches for the **bb_eval.conf** file with all the evaluation information. In "conf_example" you can find a self-explained file with a simple evaluation. This file contents all the parameters of the evaluation: list path, submission files path, how to launch the evaluator for each excersise, etc.

When bb_eval finishes successfully, it creates a .xls file with given "eval_id" as name with the results of the evaluation. This file can be upload to BlackBoard into the corresponding grade column.
