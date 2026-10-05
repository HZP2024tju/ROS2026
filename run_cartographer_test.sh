# !/bin/bash

source source ./devel_isolated/setup.bash

# 启动 Cartographer 并加载数据包
roslaunch cartographer_ros demo_backpack_2d.launch bag_filename:=${HOME}/cartographer_paper_deutsches_museum.bag