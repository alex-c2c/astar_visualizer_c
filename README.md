# A* Path Finding "Visualizer" using C
## What is this?
A C project that is probably full of unseen bugs that was made to accomplish certain criteria.
1) Read a file that contains lines of run length encoded(RLE) data.
2) Decode RLE data, to expand into a 2D grid for path finding.
	- The following characters represents the node types
		- 'O' - Empty node
		- 'S' - Start node
		- 'E' - End node
		- 'B' - Blocker node
		- 'P' - Path node (to be used in the output file)
3) Run A-Star path finding based on grid data.
4) Update grid with return path data.
5) Encode grid data back into RLE lines.
6) Write encoded RLE lines into output file.
## How to use
1) Compile code using makefile
`make`
2) Execute program, with absolute file path to the input and output file
`./bin/main <input_file_path> <output_file_path>`
3) The actual "Visualizer" part can be found in my other repo: [astar_visualizer_godot_csharp](https://github.com/alex-c2c/astar_visualizer_godot_csharp)
## Prerequisites
Example input file found in `data` directory
