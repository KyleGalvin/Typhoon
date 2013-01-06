#!/bin/bash
   
#make sure pacman package manager is up to date
pacman -Syy
pacman -Syu
     
#X11 install with openbox window manager
pacman -S xorg-server
pacman -S xorg-xinit
pacman -S openbox
echo "exec openbox-session" >> ~/.xinitrc
pacman -S xf86-video-fbdev
     
#browser
pacman -S midori
     
#terminal
pacman -S urxvt-unicode
     
#source code versioning
pacman -S git
     
#packer install for extended AUR packages
wget https://aur.archlinux.org/packages/pa/packer/packer.tar.gz
wget https://aur.archlinux.org/packages/pa/packer/PKGBUILD
#packer dependency
pacman -S jshon
#results may vary
makepkg --asroot
pacman -U ./packer-20130104-1-any.pkg.tar.xz
     
#c compiler
pacman -S make gcc
     
#2d windowing library (event calls, image rendering, primitive shapes)
pacman -S sdl sdl_gfx sdl_image
     
#3d opengl libraries
pacman -S freeglut
     
#kinect libraries. R-PI has issues with power and usb bandwidth for the kinect. Don't expect high performance
packer -S libfreenect-git

packer -S ha-glue ha-pacemaker

