# How to install a devuan chroot on a system that only has debian's debootstrap

## 0. Prerequisites

- debootstrap
- ca-certificates

## 1. Install debian with debootstrap

It doesn't really matter which suite you choose, so I just chose `stable`.

```sh
mkdir ./debian-conv
debootstrap --variant=minbase --merged-usr --force-check-gpg  stable ./debian-conv https://deb.debian.org/debian
```

To enter the chroot, you first need to mount `/dev`, `/sys` and `/proc`:

```sh
for x in dev sys proc; do mount --rbind "/${x}" "./debian-conv/${x}"; && mount --make-rslave "./debian-conv/${x}"; done
```

A sample chroot command:

```sh
chroot ./debian-conv /usr/bin/env -i TERM="xterm-256color" PATH="/usr/bin:/bin:/usr/sbin:/sbin" bash
```

## 2. Convert debian to devuan

Based on:
- [Bookworm to Daedalus Migration Guide](https://www.devuan.org/os/documentation/install-guides/daedalus/bookworm-to-daedalus)
- [Install Mono](https://www.mono-project.com/download/stable/#download-lin-debian)

### 1. Choose a mirror from [https://www.devuan.org/get-devuan](https://www.devuan.org/get-devuan)

### 2. Download the `devuan-archive-keyring.gpg` file from it

### 3. (optional) Redownload the keys from a keyserver to verify

### 3.1. Key extraction

Install gpg and run

```sh
gpg --show-keys ./devuan-archive-keyring.gpg
```

and copy the key fingerprints to a text file.

At the time of writing the keys are:

```
72E3CB773315DFA2E464743D94532124541922FB
E032601B7CA10BC3EA53FA81BB23C00C61FC752C
EFA95D7591EA95A5A417945F010291FF0AECE9B9
2CA538D192B8B4719065DDA70022D0AB5275F140
9F8D6C74DE661075FD171BE3B3982868D104092C
6A2769BF7BE79F1725696E0B55C470D57732684B
```

### 3.2. Key fetching

Create a new keyring from the key fingerprints:

```sh
# If no file
gpg --homedir /tmp --no-default-keyring --keyring /tmp/devuan.gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys <keys>

# If keys are in a file
xargs -d'\n' gpg --homedir /tmp --no-default-keyring --keyring /tmp/devuan.gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys < keys.txt
```

### 3.3. Verify that the signatures match

```sh
gpg --show-keys ./devuan-archive-keyring.gpg
gpg --show-keys /tmp/devuan.gpg
```

### 4. Edit apt sources

Remove old debian sources:

```sh
rm -rf /etc/apt/sources.list*
```

Add the devuan ones (<C-D> denotes Ctrl+D):

```sh
# Set the keyring path according to where you downloaded it
KEYRING_PATH="/tmp/devuan.gpg"

printf "%s\n" "deb [ signed-by=${KEYRING_PATH} ] http://deb.devuan.org/merged daedalus main" > /etc/apt/sources.list
```

### 5. Update & Upgrade

```sh
apt-get update
apt-get install devuan-keyring
apt-get update
apt-get upgrade
apt-get dist-upgrade
```

### 6. Prepare the environment for devuan debootstrap

### 6.1. Exit the chroot and mount the devuan target in the debian rootfs

```sh
mkdir ./devuan-daedalus
mount --bind ./devuan-daedalus ./debian-conv/mnt
```

### 6.2. Re-enter the `./debian-conv` chroot

### 7. Create the devuan rootfs

```sh
apt-get install debootstrap
debootstrap --exclude=nano --variant=minbase --merged-usr --force-check-gpg daedalus /mnt http://deb.devuan.org/merged
```

### 8. Cleanup

Exit the chroot and then clean up.

### 8.1. Unmount the mounts.

```sh
umount -R ./debian-conv/mnt

umount -R ./debian-conv/sys
umount -R ./debian-conv/proc

# May fail (probably will), see the next point
umount -R ./debian-conv/dev
```

### 8.2. Can't `umount ./debian-conv/dev`

If you can't unmount dev, that means that `gpg-agent` and `dirmngr` are still
running. If you don't have any `gpg-agent`s or `dirmngr`s started with a
homedir of /tmp outside of the chroot, then you can just run this:

```sh
apt-get install procps
pkill -f -- '--homedir /tmp'
```

### 9. Done!

Now you have a fully functional Devuan rootfs in `./devuan-daedalus`

Enjoy :3
