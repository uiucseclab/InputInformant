#!/bin/bash

COMP="javac"
CLASSPATH="javax.mail.jar"
OUTDIR="."
SOURCEPATH="."

EXTRA=""

FILES="mailUtility.java"

cmd=`echo "$COMP" "$EXTRA" -classpath "$CLASSPATH" -sourcepath "$SOURCEPATH" -d "$OUTDIR" "$FILES"`
echo "Compilation command: \"$cmd\" ";

echo "-------------------------------"

$cmd

if [ $? -eq 0 ]; then
	echo "Compilation succeeded!";
else
	echo "Compilation failed!";
fi
