<<<<<<< Updated upstream
# JMS_Simulation_Robot (JSR)
### Minseo Jeong Making_Your_Simulation_Robot(MYSR)

#### System part 최종 프로젝트: [시뮬레이션 로봇](https://www.notion.so/sysconresearch/7fc4a65881794b37a0d292a61730601c?pvs=4)
    - 개발 환경: ROS1(noetic), Ubuntu 20.04
    - 기간: 2023.07.24(월)~10.31(화)
    - Road Map
      1. Turtlebot3 Simulation
      2. Robot model 구성(URDF & xacro)
      3. teleop & sim world
      4. SLAM(Mapping + Navigation)
      5. Mission Command
--------------------------

**1. Turtlebot3 Simulation** [참고](https://www.notion.so/sysconresearch/1-Turtlebot3-Simulation-3a285c942cf04044aa7b8403d6742802?pvs=4) | [개인 정리 페이지](https://www.notion.so/sysconresearch/Assign3_Robot-simulation-1-e0c62e254a2e4a6c885b16e239e11809)
   - 기간: 2023.07.25(Fri)~08.10(Wed)
   - 터틀봇3 시뮬레이션인 Gazebo, SLAM, Navigation 체험  
     각 시뮬레이션의 노드 및 패키지 구성 방법 파악  
     costmap의 정의, 의미, 용도, 구성요소 등 파악
    
**2. Robot model 구성(URDF & xacro)** [참고](https://www.notion.so/sysconresearch/2-Robot-model-URDF-xacro-1ba0466ff7e641fc87c1683a30ad597e?pvs=4)
   - 기간: 2023.08.11(Wed)~08.18(Fri)
   - 구동 휠 2개, 라이다 1개 모바일 로봇 모델 생성 후 Rviz, Gazebo 상에서 로드
   
**3. teleop & sim world** [참고](https://www.notion.so/sysconresearch/3-teleop-sim-world-478a78464d0143e386a7677b94a65f45?pvs=4)

**4. SLAM(Mapping + Navigation)** [참고](https://www.notion.so/sysconresearch/4-SLAM-Mapping-Navigation-605ea983137e410c82320961afe61196?pvs=4)

**5. Mission Command** [참고](https://www.notion.so/sysconresearch/5-Mission-Command-9aa18ef95f3944378efca67093f05201?pvs=4)

=======
### Minseo Jeong git repository

#### System part assignment with manager HaeJin Kim
   1. Assign 1 [link](https://www.notion.so/sysconresearch/Assign1_pub-sub-0be098e3f7874cc6bb7725537804cc9c)
      - Duration: 2023.07.14(Fri)~07.19(Wed)
      - Environment: ROS1(noetic), Ubuntu 20.04
      - Concept: Make ros publisher and subscriber package.   
                 Publisher period is 0.1s and it has to count publish period. Subscriber period of subscribing and sleep is 1s and it has to receive topic and count number from publisher node
      - Node name: publisher, subsriber
      - Topic name: message
   
  2. Assign 2 [link](https://www.notion.so/sysconresearch/Assign2_service-with-trigger-6bae57aa8078488c806031ef00d7fc8e)
     - Duration: 2023.07.19(Wed)~07.21(Fri)
     - Environment: ROS1(noetic), Ubuntu 20.04
     - Concept: Make ros calculator service node using trigger service type.   
                We can input two numbers and four operator(+,-,/,*). If input two numbers and one of the operator, service node calculates the value and shows result when trigger service was sent and its status is true.
     - Node name: server
     - Service name: calculate, trigger_check
       
  3. Robot simulation  
     3-1. Turtlebot3 simulation [link](https://www.notion.so/sysconresearch/Turtlebot3-3-Ubuntu-20-04-noetic-50ea8f64a5144074bd6aa0e3b3e8408d)
     - Duration: 2023.07.24(Mon)~08.31(Thu)
     - Environment: ROS1(noetic), Ubuntu 20.04
     - Concept: Make personal Turtlebot3 simulation   
                It must be applied teleop, nav goal, mapping
     - Goal: Learn and think how to make mobile robot in ros simulation(gazebo, rviz) system and what is needed to make own mobile robot simulation.   
  
      3-2. Mobile Robot simulation
      - Duration: 2023.09.01(Fri)~10.31(Tue)
      - Environment: ROS1(noetic), Ubuntu 20.04
         
         
>>>>>>> Stashed changes
