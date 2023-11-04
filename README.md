# MySQLMM

MySQLMM is a simple non-blocking MySQL connector for MetaMod.

# Features

- Non-blocking MySQL queries
- Windows & Linux support
- Simple API

# Todo

- [x] Add query operation
- [ ] Create interface and expose through meta factory

# Interface

MySQLMM will expose an interface in `OnMetamodQuery` which can then be queried with `g_SMAPI->MetaFactory("IMySQLMM", &ret, NULL)` by other plugins.
Said interface is not yet implemented and will come after main functionality is done.

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