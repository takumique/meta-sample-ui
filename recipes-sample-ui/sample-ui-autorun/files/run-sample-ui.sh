#!/bin/sh

/usr/bin/sample-ui
if [ $? -eq 130 ]; then
  shutdown -h now
fi

exit 0
