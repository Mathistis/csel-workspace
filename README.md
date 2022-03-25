# Workspace for CSEL

> :warning: **Open this project in a Docker Container**

## Cheat Sheet for developers

Configure with the default settings:

```
cd /buildroot
make csel_defconfig
```

Make changes if you want:

```
make menuconfig
```

Build:

```
make
```

If you change the configuration and want to keep it in the workspace, do the following:

```
make savedefconfig
cp /buildroot/configs/csel_defconfig /workspace/config/configs/
```

When the build is done, copy the images from buildroot to the workspace

```
sync-images.sh
```

You can now "burn" the Compact Flash using [BalenaEtcher](https://www.balena.io/etcher/)
