## Assign branch

### System part assignment with manager HaeJin Kim
  - **Topic**: 1st and 2nd assignment of system part
  - **Total duration**: 2023.7.14(Fri) ~ 7.21(Fri)
  - **Environment**: ROS1(noetic), Ubuntu 20.04
  - **Language**: C++

   1. **`Assign 1`** [link](https://www.notion.so/sysconresearch/Assign1_pub-sub-0be098e3f7874cc6bb7725537804cc9c?pvs=4)
      - **Duration**: 2023.07.14(Fri)~07.19(Wed)
      - **Concept**: *Make ros publisher and subscriber package.*<br/>
                 Publisher period is 0.1s and it has to count publish period. Subscriber period of subscribing and sleep is 1s and it has to receive topic and count number from publisher node
      - **Node name**: publisher, subsriber
      - **Topic name**: message

  2. **`Assign 2`** [link](https://www.notion.so/sysconresearch/Assign2_service-with-trigger-6bae57aa8078488c806031ef00d7fc8e?pvs=4)
     - **Duration**: 2023.07.19(Wed)~07.21(Fri)
     - **Concept**: *Make ros calculator service node using trigger service type.*<br/>
                We can input two numbers and four operator(+,-,/,*). If input two numbers and one of the operator, service node calculates the value and shows result when trigger service was sent and its status is true.
     - **Node name**: server
     - **Service name**: calculate, trigger_check

#### Update Date: 2023.10.31