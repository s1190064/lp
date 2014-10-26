#!/bin/sh

for file in test[0-9] test[0-9][0-9]
do
  if [ -f $file ] ; then
    ./test.sh $file
  fi
done
