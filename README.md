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