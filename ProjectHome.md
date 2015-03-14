# ProxyManager #

## Easy switching of proxysettings for your desktop ##

It's a simple idea, really:
  * Set up a proxy on your own computer.
  * Point all your networking apps to this local proxy (firefox, eclipse, chrome, apt, bash, gnomeapps, kdeapps,...) (this you only have to do once).
  * When you connect to a network behind a proxy, point your local proxy to that proxy.

Voila!

This complies with the principle that all problems in software can be solved with an ekstra layer of indirection .

ProxyManager uses cntlm to do the heavy lifting, so you need to have cntlm installed in order to use ProxyManager.

[cntlm](http://cntlm.sourceforge.net/) is a brilliant piece of software. It is lighweight, has all the http- and https-proxy functionality you could wish for, and can do NTLM authorization for you. Handy if you are in a corporate environment with lots' of windows servers.

ProxyManager allows you to define a number of different cntlm-setups and easily switch between these.

Additionally ProxyManager will remember what setup you used last time you where on a network, and automagically enable that setup when you connect to that network again.

ProxyManager is written in C++ and Qt (known to work with 4.7 and 4.8)

## Download and install ##

### Prerequisites ###

To build and run proxymanager you'll need

  * mercurial (if you checkout source from the repository)
  * cmake
  * make
  * a C++ compiler
  * qt4 libraries
  * qt4 development headers
  * cntlm

On Debian or a Debian derived distribution you can do:

```
   sudo apt-get install cmake make g++ libqt4-dev cntlm
```

- which should pull in everything needed.

If you want to checkout proxymanager from the mercurial repo, you'll need mercurial as well:

```
   sudo apt-get install mercurial
```

On Arch you can do:

```
   sudo pacman -S cmake make qt
```

cntlm is not in the official Arch repositories so you'll have to get it from Aur:

https://aur.archlinux.org/packages/cntlm/

Get the tarball from there, extract it somewhere, cd to the extracted directory, and do:

```
   makepkg
   sudo pacman -U <name-of-generated-cntlm-package>
```

## Building ##

Either download a tarball from the [downloads page](http://code.google.com/p/proxymanager/downloads/list) and extract it:

```
   tar xzf proxymanager-[version].tgz
```

- or clone the mercurial repo:

```
   hg clone http://code.google.com/p/proxymanager
```

cd to proxymanager directory and do:

```
   cmake -DCMAKE_BUILD_TYPE=Release
   make
   sudo make install
```

## Running ##

Run:

```
   proxymanager
```

- and you'll see proxymanagers icon in your system tray: ![http://wiki.proxymanager.googlecode.com/hg/icon.png](http://wiki.proxymanager.googlecode.com/hg/icon.png)

Rightclick the icon and choose `Manage...`, to open a dialog where you can define proxysetups. When you have defined one or more proxy-setups, you can again rightclick the systemtray icon and choose a proxysetup from the context menu.

**note:**

proxymanager keeps NTLM passwords in _clear text_ in it's configuration file

`~/.config/Surlykke IT/ProxyManager.conf`.

A better solution is on the todo-list, but for now you should make sure that only youself have read access to that file.

Please do:

```
  cd ~/.config/Surlykke\ IT
  chmod 600 ProxyManager.conf
```