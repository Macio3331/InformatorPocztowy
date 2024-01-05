# InformatorPocztowy

Main goal of the project is to give a possiilty of checking if a mailbox is empty. 

Project contains two PHP script files placed in Raspberry Pi Zero which acts as a web server (with LAMP - Linux, Apache server, MySQL database + server and PHP - installed) and one .ino file which describes program installed on ESP32 DOIT DEVKITV1.

Raspberry Pi Zero hosts two scripts - one responsible for showing table with information about letters delivered and second responsible for getting connection with ESP32 microcontroller and posting data gained by ESP32 to database.

ESP32 is programmed to acquire connection to the Raspberry Pi Zero Server using WiFi and to send data to it. Also, as it is powered by 9V battery, system has the ability to measure the power left by testing its value once in half an hour. ESP32 works in a deep sleep mode so it may be woke up with either a letter delivered (signal from a motion sensor) or with a voltage measure. ESP32 also sends info when battery is going dead and sends required information to the server.

Program made for Microprocessor and Embedding Systems project classes.

Maciej Musiał, 2024
