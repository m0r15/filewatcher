# File Watcher

## Description

CLI Application for watch for integrity of files with Inotify.

CLI Arg is config file with file's list who under watch.

It prints file's list who readed from config file and prints state of take 
control.

When works, app is controling files for change

When change detected from file, it prints message and restoring from backup.

---
## 3rdparty libs

- [BS::thread_pool](https://github.com/bshoshany/thread-pool/tree/master)