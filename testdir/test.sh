#!/bin/sh


./$1 > $1.result 2>&1
if diff $1.result $1.correct > /dev/null; then
  echo $1: 成功
else
  echo $1: 失敗
fi
