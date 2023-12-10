#ifndef MY_GLOBALS_H
    #define MY_GLOBALS_H

    //define your default values here, if there are different values in config.json, they are overwritten.
    // char mqtt_server[40] = "10.8.196.116";
    char mqtt_server[40] = "192.168.178.200";
    char mqtt_port[6] = "1883";
    char mqtt_user[10] = "mqtt";
    char mqtt_pwd[10] = "mqtt12";
    char raum[10] = "Physik1";
#else
#endif
