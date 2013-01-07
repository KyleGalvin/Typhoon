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
#results may vary on the output name
makepkg --asroot
pacman -U ./packer-20130104-1-any.pkg.tar.xz
     
#c compiler
pacman -S make gcc
     
#2d windowing library (event calls, image rendering, primitive shapes)
pacman -S sdl sdl_gfx sdl_image
     
#3d opengl libraries
pacman -S freeglut
     
#kinect libraries. R-PI has issues with power and usb bandwidth for the kinect. Don't expect high performance
#TODO: finish config
packer -S libfreenect-git

##Cluster compilation requirements
#set locale to suppress error warnings (good system upkeep anyhow)
echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen
locale-gen
#BEFORE COMPILE THIS SOFTWARE YOU MUST CREATE A CLUSTER GROUP, EXECUTE AS ROOT:
groupadd -r -g 666 haclient
#BEFORE COMPILE THIS SOFTWARE YOU MUST CREATE A CLUSTER USER, EXECUTE AS ROOT:
useradd -r -g haclient -u 666 -d /var/lib/heartbeat/cores/hacluster -s /sbin/nologin -c "cluster user" hacluster
#Perl Locale Error fix
echo "LC_ALL=C">/etc/default/locale
#automake-1.11 didn't compile when the date was set to 1970 (R-PI doesnt have a real-time clock)
timedatectl set-time "2013-01-30 18:00:00"

#automake-1.13 api doesn't compile ha-glue. Use legacy version
packer -S automake-1.11
#cluster package dependencies
pacman -S autoconf pkgconfig
#Cluster node install
packer -S ha-glue ha-pacemaker

