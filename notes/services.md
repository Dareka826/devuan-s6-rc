 S  - start
 K  - stop
(d) - daemon
(s) - sript
$name - virtual target <name>
+service - optional <service>

rcS:
  S mountkernfs.sh
  S eudev
  S mountdevsubfs.sh
  S bootlogd
  S hostname.sh    hwclock.sh
  S checkroot.sh
  S checkfs.sh
  S checkroot-bootclean.sh    kmod
  S mount-configfs    mountall.sh
  S mountall-bootclean.sh
  S apparmor    brightness    procps    stop-bootlogd-single    urandom
  S mountnfs.sh
  S mountnfs-bootclean.sh
  S bootmisc.sh
  S x11-common

rc0:
  K brightness    dhcpcd    eudev    hwclock.sh    urandom
  K sendsigs
  K umountnfs.sh
  K umountfs
  K umountroot
  K halt

rc6:
  K brightness    dhcpcd    eudev    hwclock.sh    urandom
  K sendsigs
  K umountnfs.sh
  K umountfs
  K umountroot
  K reboot

rc1:
  K dhcpcd
  S bootlogs    killprocs
  S single

rc[2-5]:
  S bootlogs    dbus    dhcpcd    rmnologin
  S rc.local    stop-bootlogd

deps:
  (s) apparmor -> after:$local_fs stop_after:umountfs
  (d) bootlogd -> after:mountdevsubfs.sh before:hostname before:keymap before:keyboard-setup before:procps before:pcmcia before:hwclock before:hwclockfirst before:hdparm before:hibernate-cleanup before:lvm2
  (s) bootlogs -> after:hostname after:$remote_fs weak_before:$x-display-manager weak_before:gdm weak_before:kdm weak_before:xdm weak_before:ldm weak_before:sdm weak_before:wdm weak_before:nodm
  (s) bootmisc.sh -> after:$remote_fs weak_after:udev after?:mountnfs-bootclean.sh
  brightness -> after:$local_fs stop_before:$local_fs
  checkfs.sh
  checkroot-bootclean.sh
  checkroot.sh
  dbus
  dhcpcd
  eudev
  halt
  hostname.sh
  hwclock.sh
  killprocs
  kmod
  mountall-bootclean.sh
  mountall.sh
  mount-configfs
  mountdevsubfs.sh
  mountkernfs.sh
  mountnfs-bootclean.sh
  mountnfs.sh
  procps
  rc.local
  reboot
  rmnologin
  sendsigs
  single
  stop-bootlogd
  stop-bootlogd-single
  umountfs
  umountnfs.sh
  umountroot
  urandom
  x11-common

insserv:
  $local_fs      +mountall +mountall-bootclean +mountoverflowtmp +umountfs
  $network       +networking +ifupdown
  $named         +named +dnsmasq +lwresd +bind9 +unbound +pdns-recursor $network
  $remote_fs     $local_fs +mountnfs +mountnfs-bootclean +umountnfs +sendsigs
  $syslog        +rsyslog +sysklogd +syslog-ng +dsyslog +inetutils-syslogd
  $time          +hwlock
  <interactive>  glibc udev console-screen keymap keyboard-setup console-setup cryptdisks cryptdisks-early checkfs-loop
