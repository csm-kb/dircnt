# dircnt
The most basic UNIX C program for counting files and their total size in a directory.

Usage:
```
dircnt [dir] [--size]
```
Useful when running with the `watch` utility, for arbitrary monitoring purposes:
```
watch [-n interval_seconds] 'dircnt [dir] [--size]'
```
Monitoring massive directories with millions of files may suffer a loss of real-time performance, depending on your operating environment (can be hit or miss).

Run times:
| Count | `real` | `usr` |  `sys` | FileSys | Environment|
| --- | --- | --- | --- | --- | --- |
| 122,042,933 | `5m4.703s` | `0m10.375s` | `0m50.312s` | XFS | c5.12xlarge AMI box in AWS EC2, FS mounted on gp3 @ 16k IOPS r/w |
