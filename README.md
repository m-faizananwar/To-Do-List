# KeyNotes: Professional Terminal Workspace

KeyNotes is a professional task management application designed for the command line interface. It provides a structured environment for tracking routines and work-focused tasks with a minimalist and sophisticated aesthetic.

## Features

### Professional Color Palette
KeyNotes uses a curated color system that provides visual clarity while maintaining a professional appearance. Each task category and interface element is clearly distinguishable through consistent color coding.

### Interactive Navigation
The application features an interactive menu system that replaces traditional numeric input with arrow-key navigation. This provides a smoother and more modern user experience within the terminal.


### Comprehensive Task Management
- Automated timestamping for every task entry.
- Category-based filtering (Routine vs. Work Focus).
- Dashboard overview of pending and completed tasks.
- Persistence across sessions using local data storage.

### High Performance
Built entirely in C, the application ensures low latency and high performance across all common terminal emulators.

## Installation and Execution

### Prerequisites
A C compiler (e.g., gcc) and the `make` utility are required to build the application.

### Compilation
To compile the application using the included build system, run:

```bash
make
```

### Execution
Once compiled, launch the workspace with:

```bash
make run
```

Alternatively, you can run the binary directly:

```bash
./keynotes
```

## Operation
- Use the **Up/Down Arrow Keys** to highlight menu options.
- Press **Enter** to confirm your selection.
- Follow the on-screen prompts for data entry and task management.

## License
This project is licensed under the MIT License. See the LICENSE file for details.
