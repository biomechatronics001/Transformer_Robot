# Foldable Robot
This project that aims to simulate a servo-actuated foldable robot in ROS and Gazebo

**Project Link:** https://www.dropbox.com/sh/rqnn5t2tth3eoky/AACDVzuv0R80pEUr_oEpCdlKa?dl=0

## Features
- [X] Launches in Gazebo and ROS Rviz
- [X] Parallel robot/closed-joint loop configuration
- [X] User Control via Keyboard
- [X] User Control via MATLAB
- [X] Handles object collision and joint constraints

## Download & Launch Instructions
Make sure that you are using the latest version of the ROS Noetic distro. Instructions on how to install ROS can be found here: http://wiki.ros.org/noetic/Installation/Ubuntu

You may also need to download additional ROS packages to make the simulation launch. The ROS Control package can be installed by typing "$ sudo apt-get install ros-noetic-ros-control ros-noetic-ros-controllers" into the terminal.

1) To open the files in this repository on your computer, click the green "code" button and copy the HTTPS link to your clipboard.
2) Open a new terminal shell and type "$ git clone (the HTTPS link you copied to your clipboard)"
3) Type "$ git checkout dev" in the terminal to enter the dev branch of the repository.
4) In order for the code to run, you need to remove and rebuild your workspace. Make sure you are located inside the BIRO_FoldableRobot directory and then type "$ rm -r build/" and "$ rm -r devel/" to remove the build/ and devel/ folders respectivley from the project.
5) Once build/ and devel/ are removed, type "$ catkin_make" to rebuild the workspace.
6) Source the terminal to accept ROS commands by typing "$ source devel/setup.bash" into the terminal.
7) To open an empty gazebo world with the robot model, type "$ roslaunch foldable_robot gazebo8.launch". To view the robot in a desert environment, view the section "Viewing the Robot in a Desert World in Gazebo" below.
8) To use keyboard input to control the robot, do the following: In another terminal, repeat step 5 and then type "$ rosrun foldable_robot keyboard_interpreter.py". This will open the keyboard interpreter which allows keyboard commands from the launch file containing the gazebo model to receive keyboard input and translate it to joint angles for the model in the Gazebo world.

## User Control of Level Two robot via UI Control Panel
The user can control the level two robot via an interactive control panel.

1) First, open the level_two robot model in Gazebo by typing "$ roslaunch foldable_robot gazebo_level_two.launch" into the terminal. If you have set up the desert ground texture, you can open the model with the desert world by typing "$ roslaunch foldable_robot gazebo_level_two_desert.launch". Make sure that you have sourced the terminal by typing "$ source devel/setup.bash" into the terminal before launching the gazebo file.
2) Open and source a separate terminal. Type "rosrun foldable_robot table_control.py" in the terminal to open the UI control panel.

<p align="center" width="">
    <img width="" src='https://github.com/FrancisGS131/BIRO_FoldableRobot/blob/francis/UI_Control_Panel.png'> 
</p>

The control panel consists of three columns: "Joint", "Angle" and "Configs". The "Joint" column represents the active joints of the robot, and the "Angle" column represents the position the joints will be published to. Entries in the "Angle" column can be edited by double clicking the values corresponding to the joint you wish to command. Once the desired position (in degrees) is specified for the joint inside the text box that appears, press the "OK" button. The user can specify multiple joint angles at once.

When the desired joint angles are input into the table, the "Publish" button should be pressed to simultaneously publish the joint positions to the joint controllers. The "Reset" button is used to reset the joint angles listed in the table and to command the robot to return to the initial configuration (with all joint angles equal to zero degrees).

The "Configs" column consists of buttons that command the robot to assume preset configurations. The joint angles of each preset configuration can be edited in the "Angle" column to explore other configurations for the robot. Some configurations such as the "Folded-in Wheel" take longer to carry out because the joint angles need to be published individually instead of simultaneously. To properly display configurations "Snail", "Flat-T", "Plane with wing tips", "Plane without wing tips" and "Wright Bros Plane", the "Folded-in Wheel" configuration must be pressed first.

**Note:** At the moment the robot is unable to accommodate stretching, so if joint values are specified that induce stretching, the robot may begin to oscillate and move uncontrollably. If this occurs, you can either set the joint value that caused this behavior back to zero and press the "Publish" Button or you can reset the robot configuration by pressing the "Reset" button.   

## User Control via MATLAB
The user can control the robot by inputting four joint angles in a MATLAB function called pairedjointcontrol.m. This allows the user to specify the position of the four active joints of the foldable robot.

There is also a MATLAB function called setconfig.m that lets the user input an integer from 0-8. Integers 0-7 allow the user to specify predetermined configurations of the robot. Integer input 8 allows the user to view the robot move through all seven preconfigured positions consecutively.

## User Control via Keyboard
The user can control the foldable robot in a two ways through **two** modes when using the keyboard control python scripts: Individual and Paired Mode.

**Note:** The user must type into the same terminal that gazebo8.launch is run from.

> **Individual Mode** is the standard mode where you can control each individual joint
 - Press a *number key* from 1-8 to control that each individual joint (pressing 0 and 9 won't do anything)
 - Press *w* to increment joint angles, *s* to decrement joint angles simultaneously
 - Press *r* to reset the robot's configuration
> **Pairing Mode** is an alternate mode where you can control multiple joints at a time. Press *p* in **individual mode** to enter Pairing Mode.
 - Press *p* in **pairing mode** to **exit** and switch modes
 - Press *number keys* from 1-8 to pair different joints. Joints must first be paired to move them.
 - Press *w* to increment joint angles, *s* to decrement joint angles simultaneously
 - Press *r* to reset the paired joints and paired angle

## Viewing the Robot in a Desert World in Gazebo
To view the robot in a desert environment within the Gazebo smulation environment, follow these steps:

1) Create a new model directory within Gazebo by typing "$ mkdir ~/.gazebo/models/desert_ground_plane" in your terminal.
2) Create material directories for your model by typing "$ mdkir -p ~/.gazebo/models/desert_ground_plane/materials/textures" and  "$mdkir -p ~/.gazebo/models/desert_ground_plane/materials/scripts" in your terminal.
3) Create a material file ("~/.gazebo/models/desert_ground_plane/materials/scripts/desert_plane.material) containing the following:
```
    material DesertGroundPlane/Image
            {
              technique
              {
                pass
                {
                  ambient 1 1 1 1.000000
                  diffuse 1 1 1 1.000000
                  specular 0.03 0.03 0.03 1.000000

                  texture_unit
                  {
                    texture desert1.jpeg
                    scale 0.33 0.33
                  }
                }
              }
            }
```           
4) Download the following image (https://user-images.githubusercontent.com/76972296/111056656-c5749700-844e-11eb-9297-fafb26c57cb3.png)
and save the image as "desert1.jpg". Then move this image to the following directory: ~/.gazebo/models/desert_ground_plane/materials/textures/desert1.png
5) Add a  ~/.gazebo/models/desert_ground_plane/model.sdf file containing the following:
```
<?xml version="1.0" encoding="UTF-8"?>
<sdf version="1.4">
   <model name="desert_ground_plane">
      <static>true</static>
      <link name="link">
         <collision name="collision">
            <geometry>
               <plane>
                  <normal>0 0 1</normal>
                  <size>4.5 3.375</size>
               </plane>
            </geometry>
            <surface>
               <friction>
                  <ode>
                     <mu>100</mu>
                     <mu2>50</mu2>
                  </ode>
               </friction>
            </surface>
         </collision>
         <visual name="visual">
            <cast_shadows>false</cast_shadows>
            <geometry>
               <plane>
                  <normal>0 0 1</normal>
                  <size>4.5 3.375</size>
               </plane>
            </geometry>
            <material>
               <script>
                  <uri>model://desert_ground_plane/materials/scripts/</uri>
                  <uri>model://desert_ground_plane/materials/textures/</uri>
                  <name>DesertGroundPlane/Image</name>
               </script>
            </material>
         </visual>
      </link>
   </model>
</sdf>
```
6) Create a ~/.gazebo/models/desert_ground_plane/model.config file containing the following:
```
<?xml version="1.0" encoding="UTF-8"?>
<model>
   <name>Desert Ground Plane</name>
   <version>1.0</version>
   <sdf version="1.4">model.sdf</sdf>
   <description>A textured desert ground plane.</description>
</model>
```
7) Create a desert.world file in the /usr/share/gazebo-11/worlds directory containing the following:
```
<sdf version='1.7'>
  <world name='default'>
    <light name='sun' type='directional'>
      <cast_shadows>1</cast_shadows>
      <pose>0 0 10 0 -0 0</pose>
      <diffuse>0.8 0.8 0.8 1</diffuse>
      <specular>0.2 0.2 0.2 1</specular>
      <attenuation>
        <range>1000</range>
        <constant>0.9</constant>
        <linear>0.01</linear>
        <quadratic>0.001</quadratic>
      </attenuation>
      <direction>-0.5 0.1 -0.9</direction>
      <spot>
        <inner_angle>0</inner_angle>
        <outer_angle>0</outer_angle>
        <falloff>0</falloff>
      </spot>
    </light>
    <gravity>0 0 -9.8</gravity>
    <magnetic_field>6e-06 2.3e-05 -4.2e-05</magnetic_field>
    <atmosphere type='adiabatic'/>
    <physics type='ode'>
      <max_step_size>0.001</max_step_size>
      <real_time_factor>1</real_time_factor>
      <real_time_update_rate>1000</real_time_update_rate>
    </physics>
    <scene>
      <ambient>0.4 0.4 0.4 1</ambient>
      <background>0.7 0.7 0.7 1</background>
      <shadows>1</shadows>
    </scene>
    <wind/>
    <spherical_coordinates>
      <surface_model>EARTH_WGS84</surface_model>
      <latitude_deg>0</latitude_deg>
      <longitude_deg>0</longitude_deg>
      <elevation>0</elevation>
      <heading_deg>0</heading_deg>
    </spherical_coordinates>
    <model name='desert_ground_plane'>
      <static>1</static>
      <link name='link'>
        <collision name='collision'>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>1.5 11.5</size>
            </plane>
          </geometry>
          <surface>
            <friction>
              <ode>
                <mu>1000</mu>
                <mu2>1000</mu2>
              </ode>
              <torsional>1
                <ode/>
              </torsional>
            </friction>
            <contact>
              <ode/>
            </contact>
            <bounce/>
          </surface>
          <max_contacts>10</max_contacts>
        </collision>
        <visual name='visual'>
          <cast_shadows>0</cast_shadows>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>3 3</size>
            </plane>
          </geometry>
          <material>
            <script>
              <uri>model://desert_ground_plane/materials/scripts</uri>
              <uri>model://desert_ground_plane/materials/textures/</uri>
              <name>DesertGroundPlane/Image</name>
            </script>
          </material>
        </visual>
        <self_collide>0</self_collide>
        <enable_wind>0</enable_wind>
        <kinematic>0</kinematic>
      </link>
      <pose>0 0 0 0 -0 0</pose>
    </model>
    <state world_name='default'>
      <sim_time>223 257000000</sim_time>
      <real_time>280 896187301</real_time>
      <wall_time>1613417403 900470623</wall_time>
      <iterations>223257</iterations>
      <model name='desert_ground_plane'>
        <pose>0 0 0 0 -0 0</pose>
        <scale>0.5 0.5 0.5</scale>
        <link name='link'>
          <pose>0 0 0 0 -0 0</pose>
          <velocity>0 0 0 0 -0 0</velocity>
          <acceleration>0 0 0 0 -0 0</acceleration>
          <wrench>0 0 0 0 -0 0</wrench>
        </link>
      </model>
      <model name='foldable_robot'>
        <pose>-1e-06 1e-06 0.025 3e-06 7e-06 2.8e-05</pose>
        <scale>1 1 1</scale>
        <link name='cube1'>
          <pose>-1e-06 1e-06 0.025 3e-06 7e-06 2.8e-05</pose>
          <velocity>-0.000124 -0.000169 0.000173 0.009112 -0.004951 -0.001471</velocity>
          <acceleration>-0.187657 -0.321844 0.348853 -0.108452 -1.40769 -3.10363</acceleration>
          <wrench>-0.018766 -0.032184 0.034885 0 -0 0</wrench>
        </link>
        <link name='cube2'>
          <pose>-0.025013 1.1e-05 0.050004 -0.426562 -1.57042 0.426442</pose>
          <velocity>-0.000642 -0.000754 0.000127 0.015485 -0.018683 0.01112</velocity>
          <acceleration>0.134139 -0.763049 -0.046815 2.17202 1.0397 1.47192</acceleration>
          <wrench>0.013414 -0.076305 -0.004682 0 -0 0</wrench>
        </link>
        <link name='cube3'>
          <pose>-0.075023 -0.024986 0.02502 -0.118432 -1.57052 0.118299</pose>
          <velocity>0.000234 -0.001088 -0.000803 0.002592 -0.004537 -0.008057</velocity>
          <acceleration>0.124251 -1.76453 -1.27303 1.89536 -0.693241 -1.45698</acceleration>
          <wrench>0.012425 -0.176453 -0.127303 0 -0 0</wrench>
        </link>
        <link name='cube4'>
          <pose>-0.050022 -0.074988 0.050014 0.064793 -1.57054 -0.06491</pose>
          <velocity>-0.000129 -0.00146 -0.00088 0.065011 0.001568 -0.000985</velocity>
          <acceleration>-0.772919 0.322163 -4.40329 2.05396 0.50558 2.93997</acceleration>
          <wrench>-0.077292 0.032216 -0.440329 0 -0 0</wrench>
        </link>
        <link name='cube5'>
          <pose>-0.075033 -0.124984 0.025021 0.039946 -1.57055 -0.040055</pose>
          <velocity>-6.8e-05 2.8e-05 -0.004367 0.067088 0.006959 0.00311</velocity>
          <acceleration>-1.08926 0.4335 -9.9404 0.808439 0.394062 -0.427156</acceleration>
          <wrench>-0.108926 0.04335 -0.99404 0 -0 0</wrench>
        </link>
        <link name='cube6'>
          <pose>-0.025031 -0.14999 0.050008 0.104066 -1.57061 -0.10414</pose>
          <velocity>0.000487 -0.000932 -0.00635 0.064567 0.000284 0.002356</velocity>
          <acceleration>-0.252336 1.21253 -10.6504 -2.41019 -0.661588 -1.4019</acceleration>
          <wrench>-0.025234 0.121253 -1.06504 0 -0 0</wrench>
        </link>
        <link name='cube7'>
          <pose>0.024965 -0.124993 0.025 0.00111 -1.57059 -0.00127</pose>
          <velocity>0.000686 0.000998 -0.004511 0.073021 -0.002628 0.009863</velocity>
          <acceleration>-0.102778 0.815289 -4.04446 2.286 0.119701 -2.65968</acceleration>
          <wrench>-0.010278 0.081529 -0.404446 0 -0 0</wrench>
        </link>
        <link name='cube8'>
          <pose>-3.3e-05 -0.075001 0.050003 -1.46851 -1.57074 1.46836</pose>
          <velocity>0.000113 -0.00107 -0.000391 0.013436 -0.021875 0.02184</velocity>
          <acceleration>0.342631 0.010298 -1.00001 1.74814 0.397252 -0.999595</acceleration>
          <wrench>0.034263 0.00103 -0.100001 0 -0 0</wrench>
        </link>
      </model>
      <light name='sun'>
        <pose>0 0 10 0 -0 0</pose>
      </light>
    </state>
    <gui fullscreen='0'>
      <camera name='user_camera'>
        <pose>4.03467 -2.12325 3.41682 0 0.631643 2.70018</pose>
        <view_controller>orbit</view_controller>
        <projection_type>perspective</projection_type>
      </camera>
    </gui>
  </world>
</sdf>
```
8) To launch the robot in the desert environment, run the command "$ roslaunch foldable_robot gazebo8_desert.launch" in your BIRO_Foldable_Robot workspace.

## Simulation Walkthrough
Here's a walkthrough of the simulation:

<img src='https://github.com/FrancisGS131/BIRO_FoldableRobot/blob/david/foldable_robot.gif' width='' alt='Video Walkthrough' />

GIF created with [Peek](https://github.com/phw/peek).
