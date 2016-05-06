#!/bin/bash

RUN="java"
OUTDIR="./bin"
CLASSPATH=".:javax.mail.jar"
OUTDIR="."

EXTRA=""

MAINCLASS="mailUtility"

#zip images.zip ../Images/*.png

if [ ! -d "$OUTDIR" ]; then
    echo "Class files directory does not exist: run 'compile1.sh' first, exiting...";
    exit 1;
fi


cmd=`echo "$RUN" "$EXTRA" -classpath "$CLASSPATH" "$MAINCLASS" "$1"`
echo "Run command: \"$cmd\" ";

echo "--------------------------";

$cmd

echo "--------------------------"; echo "";