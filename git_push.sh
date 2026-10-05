#!/bin/bash

rm -rf build/ devel/ .git/ build_isolated/  devel_isolated/
git init
git remote add nerd git@github.com:HZP2024tju/ROS2026.git
git add .
git commit -m "damn"
git push --set-upstream nerd master -f