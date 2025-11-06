## statx

This program uses the ```statx``` system call to return file status (extended) in the [body file format](https://wiki.sleuthkit.org/index.php?title=Body_file). The code was based on the example provided in the Linux kernel [test-statx.c](https://github.com/torvalds/linux/blob/master/samples/vfs/test-statx.c).

### Body file format

```text
MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
```

## 🔧 Building

```shell
make ARCH
```

## 🚀 How to use it

```shell
./statx FILE
```

### Example

```shell
./statx /etc
0|/etc|269|drwxr-xr-x|0|0|3242|1696957845|1696940645|1696940645|1678364047
```

## 📜 License

The statx project uses the [Apache License Version 2.0](LICENSE) software license.