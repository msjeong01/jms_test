### Minseo Jeong personal git repository

#### System part assignment with manager HaeJin Kim.
   1. Assign 1 [link](https://www.notion.so/sysconresearch/Assign1_pub-sub-0be098e3f7874cc6bb7725537804cc9c)
     - duration: 2023.07.14(Fri)~07.19(Wed)
     - environment: ROS1(noetic), Ubuntu 20.04
     - concept: Make ros publisher and subscriber package.
                Publisher period is 0.1s and it has to count publish period.
                Subscriber period of subscribing and sleep is 1s and it has to receive topic and count number from publisher node
     - node name: publisher, subsriber
     - topic name: message
      
  2. Assign 2 [linlk](https://www.notion.so/sysconresearch/Assign2_service-with-trigger-6bae57aa8078488c806031ef00d7fc8e)
     - duration: 2023.07.19(Wed)~07.21(Fri)
     - environment: ROS1(noetic), Ubuntu 20.04
     - concept: Make ros calculator service node using trigger service type
                We can input two numbers and four operator(+,-,/,*). If input two numbers and one of the operator, service node calculates the value and shows result when trigger service was sent and its status is ture.
     - node name: server
     - service name: calculate, trigger_check
       
  3. Robot simulation 
     3-1. Study using turtlebot3 [link](https://www.notion.so/sysconresearch/Turtlebot3-3-Ubuntu-20-04-noetic-50ea8f64a5144074bd6aa0e3b3e8408d)
          Learn and think how to make mobile robot in ros simulation(gazebo, rviz) system and what is needed.
