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
| File count | Run time | Filesystem | Environment|
| --- | --- | --- | --- |
| 122,301,025 | `<in progress>` | XFS | c5.12xlarge AMI box in AWS EC2, FS mounted on gp3 @ 16k IOPS r/w |
