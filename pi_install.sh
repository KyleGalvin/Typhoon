#!/bin/bash
   
#make sure pacman package manager is up to date
pacman -Syu
     
#X11 install with openbox window manager
#pacman -S xorg-server xorg-xinit openbox xf86-video-fbdev
#echo "exec openbox-session" >> ~/.xinitrc
  
#browser
#pacman -S midori
     
#terminal
#pacman -S urxvt-unicode
     
#software development and compilation tools. automake is left out due to a conflict we will resolve further down the script
pacman -S git make gcc autoconf pkgconfig python mercurial libtool 

#might need a restart to set the time properly. Many network operations dont work without an accurate clock
#ntpd     
#shutdown -r now

#if rebooting doesnt set the time...
#timedatectl set-time "2013-01-30 18:00:00"

#packer install for extended AUR packages
wget https://aur.archlinux.org/packages/pa/packer/packer.tar.gz
tar -xvf packer.tar.gz
cd packer
#packer dependency
pacman -S jshon
#results may vary on the output name
makepkg --asroot
pacman -U ./packer-20130104-1-any.pkg.tar.xz
cd ../
     
#2d windowing library (event calls, image rendering, primitive shapes)
#pacman -S sdl sdl_gfx sdl_image
     
#3d opengl libraries
#pacman -S freeglut
     
#kinect libraries. R-PI has issues with power and usb bandwidth for the kinect. Don't expect high performance
#TODO: finish config
#packer -S libfreenect-git

##COMPILE CLUSTER SOFTWARE
#automake-1.13 api doesn't compile ha-glue. Use legacy version.
packer -S automake-1.11
#automake binary called without version name. Create a softlink to provide direction
ln -ST /usr/bin/automake-1.11 /usr/bin/automake
ln -ST /usr/bin/aclocal-1.11 /usr/bin/aclocal
#set locale to suppress error warnings (good system upkeep anyhow)
echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen
locale-gen
#BEFORE COMPILE THIS SOFTWARE YOU MUST CREATE A CLUSTER GROUP, EXECUTE AS ROOT:
groupadd -r -g 666 haclient
#BEFORE COMPILE THIS SOFTWARE YOU MUST CREATE A CLUSTER USER, EXECUTE AS ROOT:
useradd -r -g haclient -u 666 -d /var/lib/heartbeat/cores/hacluster -s /sbin/nologin -c "cluster user" hacluster
#Perl Locale Error fix
#echo "LC_ALL=C">/etc/default/locale

#Cluster node install part one (big compile for the PI)
#packer -S ha-glue
hg clone http://hg.linux-ha.org/glue/
cd glue
autoreconf --force --install
#glue dependency
pacman -S libxml2 libaio
./configure
#I needed to remove somewhere near 5 or 6 -Werror flags just to make this compile. 
#I don't have a log of which Makefiles were altered, and it was a pain.
#Hopefully the flags were superfluous.
make && make install
cd ../

#now that we are done with the legacy version, we can install the current version
pacman -S automake



#install the rest of the clustering suite part two (big compile for the PI, one package per command)

##this one had to be downloaded from source, then run autoreconf, ./configure, remove -Werror from several Makefiles, then 'make' and 'make install'  it
#packer -S ha-resourceagent 
#get the source
git clone https://github.com/ClusterLabs/resource-agents.git
#configure, compile, and install
cd resource-agents

./autogen.sh
./configure

#for each Makefile contained in the current directory (and recursively in subfolders)
find ./ -name Makefile | while read line; do
        #strip the Werror flag from all matched files and place into temp file
        cat "$line" | sed -e 's/-Werror//g' >> "$line".bak
        #now that we're done modifications, move file to replace original
        mv "$line".bak "$line"
done

make
make install
cd ../
