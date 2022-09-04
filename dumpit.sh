#!/bin/bash

cat /etc/group | while read line; do echo "$line"; done >/dev/tcp/localhost/3333 
