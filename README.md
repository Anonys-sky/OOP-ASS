# OOP-ASS

Terminal-based Mini File System Explorer for an Object Oriented Programming assignment.

## Files

The main program is inside the `MiniFileSystem` folder:

- `main.cpp`
- `file.h`
- `File.cpp`
- `Folder.h`
- `Folder.cpp`
- `FileSystem.h`
- `FileSystem.cpp`
- `filesystem.txt`

## Compile and Run

Open a terminal in the `MiniFileSystem` folder and run:

```bash
g++ main.cpp File.cpp Folder.cpp FileSystem.cpp -o MiniFileSystem
./MiniFileSystem
```

The program loads the starting folder tree from `filesystem.txt`.
