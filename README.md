# Qt Widgets Coursework Projects

A set of separate C++/Qt exercises, preserved as individual projects rather than combined into one application.

## Projects

| Directory | Overview |
|---|---|
| `lab1/` | Small C++ project using CMake. |
| `summer_practiceN1/` | A Qt Widgets application with a phonebook-like interface and record-management actions. |
| `untitled6/`–`untitled9/` | Related exercises around medicine records, dates, and text/binary file processing. Each folder has its own Qt project file. |
| `untitled10/` | Qt Widgets interface for football-fan / World Cup data, including file loading, tables, and sorting/filtering actions. |

## Requirements and launch

Most projects use Qt Creator/qmake via their `.pro` files; `lab1` uses CMake. Open the project file for the specific exercise in a compatible Qt Creator installation and build it separately.

Some exercises expect local input files or binary data files at runtime. Keep test data in the intended working directory; generated runtime files are not part of the repository snapshot.

## Scope and limitations

This repository preserves multiple coursework variants, including incomplete or unfinished areas. For example, `untitled6/med_io.h` is an empty header in the current snapshot. The standalone `lab1` CMake target configured and built successfully with MSVC on Windows. The Qt projects were not built because Qt/qmake is not available in this environment. Confirm course and resource redistribution rules before redistribution.
