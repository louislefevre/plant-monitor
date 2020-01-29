# Plant Monitor
Plant monitoring and maintenance system for Arduino.  

Development Blog: https://llefe001pc.tumblr.com/  
Video Demonstration: https://vimeo.com/383220678

## General Information
- Capacitive Sensors
    - 580 default value (non-submerged)
    - 310 lowest value (fully submerged)
    - 350 best value (half submerged)
    - When using all sensors at once, values change to: 335, 365, 580 (respectively)
    - Values raise by approximately 60 in value when water pumps activate; 15 per active water pump. Possibly as power is drawn away, sensors become less accurate?
- Light Sensor
    - 390 default value (regular position)
    - 0 lowest value (completely dark)
    - 530 medium value (10m distance)
    - 670 max value (close proximity)
    - Values may be less accurate due to power limitations.
