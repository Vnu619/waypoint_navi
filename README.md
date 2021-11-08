# waypoint_navi
To load and save waypoints:
-> edit the directory location to save the csv file with waypoint coordinates
point_=ReadFile("/home/sutd_wasp/nav_points.csv");
run the line in a new terminal
->rostopic echo /amcl_pose 
In the rviz window drag the 2d pose estimate button on the map to choose the waypoints.
save the waypoint coordinates displayed on the terminal in  the csv file in the order of position.x, position.y, orientation.w, orientation.z
To run waypoint navigation steps to be followed:
->catkin_make
->rosrun waypoint_navi waypoint_navi_execute
