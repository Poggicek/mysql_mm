# MySQLMM

MySQLMM is a simple non-blocking MySQL connector for MetaMod.

# Features

- Non-blocking MySQL queries
- Windows & Linux support
- Simple API

# Interface

MySQLMM will expose an interface in `OnMetamodQuery` which can then be queried with `(IMySQLClient*)g_SMAPI->MetaFactory(MYSQLMM_INTERFACE, &ret, NULL);` by other plugins.
Interface definition can be found in `src/public`.

## Compilation

### Requirements

- [Metamod:Source](https://www.sourcemm.net/downloads.php/?branch=master) (build 1219 or higher)
- [AMBuild](https://wiki.alliedmods.net/Ambuild)

### Instructions

Follow the instructions below to compile CS2Fixes.

```bash
git clone https://github.com/Poggicek/mysql_mm && cd mysql_mm

export MMSOURCE112=/path/to/metamod/
export HL2SDKCS2=/path/to/hl2sdk-cs2

mkdir build && cd build
python3 ../configure.py -s cs2
ambuild
```

> [!IMPORTANT]
> Linux build requires `libmysqlclient-dev` package to be installed.