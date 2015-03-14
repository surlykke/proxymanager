## ProxyManager

### Easy switching of proxysettings for your desktop

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

### Download and install

#### Prerequisites

To build and run proxymanager you'll need

  * git (if you checkout source from the repository)
  * cmake
  * make 
  * a C++ compiler
  * qt4 libraries
  * qt4 development headers
  * cntlm
 
#### Building

Either download a tarball from [releases](https://github.com/surlykke/proxymanager/releases) and extract it or clone it.

And then:

```
   cd <proxymanager dir>
   mkdir build
   cd build
   cmake ..
   make
   sudo make install
```

#### Running

Run:

```
   proxymanager
```

- and you'll see proxymanagers icon in your system tray.

Rightclick the icon and choose ```Manage...```, to open a dialog where you can define proxysetups. When you have defined one or more proxy-setups, you can again rightclick the systemtray icon and choose a proxysetup from the context menu.

## Note:

proxymanager keeps NTLM passwords in **clear text** in it's configuration file 

```
~/.config/Surlykke IT/ProxyManager.conf`
```


**Please** do:

```
  cd ~/.config/Surlykke\ IT
  chmod 600 ProxyManager.conf
```
