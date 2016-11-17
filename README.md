# Myoware servo control

[![Build Status](https://travis-ci.org/Autofabricantes/Myoware-servo-control.svg?branch=master)](https://travis-ci.org/Autofabricantes/Myoware-servo-control)

Code to control a servomotor with a Myoware sensor

# Firmware upload instructions
This is a [Platformio](http://platformio.org/) project. To compile and upload the firmware to one of the supported boards, run the following command:
```bash
~/Myoware-servo-control$ platformio run --target upload
```

The *main.ino* file located inside the *src* folder can also be uploaded with the Arduino IDE. Make sure that the libraries located inside the *lib* folder are [installed in the Arduino environment](https://www.arduino.cc/en/Guide/Libraries).

# License
<a rel="license1" href="https://www.gnu.org/licenses/gpl-3.0.html"><img alt="GNU License" style="border-width:0" src="https://www.gnu.org/graphics/gplv3-88x31.png" /></a>
This work is licensed under the [GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.html).
