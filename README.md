# JMS_simulation_robot (MYSR)
### Minseo Jeong Making_Your_Simulation_Robot(MYSR)

#### System part 최종 프로젝트: 시뮬레이션 로봇[link](https://www.notion.so/sysconresearch/7fc4a65881794b37a0d292a61730601c?pvs=4)
    - 세팅 환경: ROS1(noetic), Ubuntu 20.04
    - 기간: 2023.07.24(월)~10.31(화)
    - Road Map
      1. [Turtlebot3 Simulation](https://www.notion.so/sysconresearch/1-Turtlebot3-Simulation-3a285c942cf04044aa7b8403d6742802?pvs=4)
      2. [Robot model 구성(URDF & xacro)](https://www.notion.so/sysconresearch/2-Robot-model-URDF-xacro-1ba0466ff7e641fc87c1683a30ad597e?pvs=4)
      3. [teleop & sim world](https://www.notion.so/sysconresearch/3-teleop-sim-world-478a78464d0143e386a7677b94a65f45?pvs=4)
      4. [SLAM(Mapping + Navigation)](https://www.notion.so/sysconresearch/4-SLAM-Mapping-Navigation-605ea983137e410c82320961afe61196?pvs=4)
      5. [Mission Command](https://www.notion.so/sysconresearch/5-Mission-Command-9aa18ef95f3944378efca67093f05201?pvs=4)
      

    1. Turtlebot3 Simulation [개인 페이지 link](https://www.notion.so/sysconresearch/Assign3_Robot-simulation-1-e0c62e254a2e4a6c885b16e239e11809)
      - 기간: 2023.07.25(Fri)~08.10(Wed)
      - 개요: 터틀봇3 시뮬레이션인 Gazebo, SLAM, Navigation을 체험해보고, 각 시뮬레이션의 노드 및 패키지 구성 방법을 파악한다.
             costmap의 정의, 의미, 용도, 구성요소 등을 파악한다.
    
    2. Robot model 구성(URDF & xacro)
     - 기간: 2023.08.11(Wed)~08.18(Fri)
     - concept: Make ros calculator service node using trigger service type
                We can input two numbers and four operator(+,-,/,*). If input two numbers and one of the operator, service node calculates the value and shows result when trigger service was sent and its status is ture.
     - node name: server
     - service name: calculate, trigger_check
   
    3. teleop & sim world   
    4. SLAM(Mapping + Navigation)
    5. Mission Command
