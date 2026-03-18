# robotic-mower
I am building and programming a robotic lawn mower controlled by an Arduino Uno R4. To start with, the mower moves using ultrasonic sensors, if it detects an object it wil change route.
The robot will be made of aluminium. It will be battery powered and i am reusing motors from a recycled robot.



here is an extended description:
We start with 240V AC from the mains, which goes directly to the charging station. Inside the charging station, it is converted to a lower voltage (24V DC or 12V DC, depending on further research, such as the required motor power, battery specifications, and the availability of components).

The charging station also serves as the base for transmitting the signal through the boundary wire.

The converted DC voltage is transferred from the charging station to the battery via a contact interface between the robot and the station.

The battery is directly connected to three H-bridges, which drive the two wheel motors and the motor for the cutting blade.

A 5V step-down converter is also connected to the battery to power the Arduino (UNO R4 WiFi). Additionally, a voltage divider is placed between the battery and the Arduino, allowing the Arduino to measure and calculate the battery percentage.

The Arduino will be programmed so that the robot mower moves in a random pattern within the area defined by the boundary wire. It also controls the motors via the H-bridges.

Apart from the H-bridges, there is one additional output: an LCD used as part of the user interface. This provides the user with information such as error codes and battery percentage.

The user interface also includes start and stop buttons to control the operation of the robot.

To ensure smooth navigation, the robot is equipped with four sensors: a boundary wire receiver module and three ultrasonic sensors. These detect when the robot approaches an obstacle and allow it to adjust its path if necessary.

In preparation for future expansion, an Arduino UNO R4 WiFi has been selected, enabling the possibility to control and monitor the robot via a smartphone app.

A GPS module is also planned, allowing the robot to follow more structured patterns (such as straight lines) and potentially operate without a boundary wire. In combination with the GPS module, a compass module will be included so the robot can determine its orientation and improve navigation accuracy.
