# Changelog

## [0.4.1]

### Added
* Added ability to load a different map

-------------------------------------------------------------------------------------------------------------

## [0.4.0]

### Added
* Added maps

### Changed
* BIG code refractoring
* Changed from 2D squares to 3D cubes (Makes it easy to make maps)
* Location of walls, roofs and floors are made by the maps

### Removed
* Removed some unnecessary texture binding code
* Removed some unused and commented out code
* Removed obsolete wall data

-------------------------------------------------------------------------------------------------------------

## [0.3.0]

### Added
* Added up/down movement to camera class
* Added roof

### Changed
* Changed from using and EBO (Element Buffer Object) to just VAO (Vertex Array Object) and VBO (Vertex Buffer Object) with glDrawArrays to draw the shapes
* Changed from hard-coding the floor position and size to dynamically drawing them
* Changed from GL_LINEAR so the texture appears smooth to GL_NEAREST so the texture looks blocky and pixelated like it was back then
* Changed floorPositions vector to floorRoofPositions. It was changed to the same thing because we don't want the roof and floor to be off from one another
* Slight variable change
* Changed how the floor and roof positions are stored

### Fixed
* Fixed the roof being inside the ground

-------------------------------------------------------------------------------------------------------------

## [0.2.0]

### Added
* Hard coded some walls and floors
* Added original textures

-------------------------------------------------------------------------------------------------------------

## [0.1.0]

### Added
* Added contrib/
* Added contrib/glad/
* Added contrib/glad/glad.c
* Added contrib/glad/LICENSE
* Added contrib/glfw/
* Added contrib/glfw/LICENSE.md
* Added contrib/glm/
* Added contrib/glm/copying.txt
* Added contrib/stb/
* Added contrib/stb/stb_image.h
* Added data/
* Added data/icons/
* Added data/maps/
* Added data/maps/map.txt
* Added data/models/
* Added data/music/
* Added data/shaders/
* Added data/shaders/cube.vert
* Added data/shaders/cube.frag
* Added data/sounds/
* Added data/textures/
* Added src/
* Added src/ANSI/
* Added src/ANSI/ANSI.hpp
* Added src/ANSI/ANSI.md
* Added src/config/
* Added src/config/config.hpp
* Added src/camera/
* Added src/camera/camera.hpp
* Added src/shader/
* Added src/shader/shader.cpp
* Added src/shader/shader.hpp
* Added src/main.cpp
* Added AUTHORS.md
* Added CHANGELOG.md
* Added Windows95Maze.sln
* Added LICENSE
* Added README.md

### Changed
*

### Deprecated
*

### Removed
*

### Fixed
*

### Security
*

-------------------------------------------------------------------------------------------------------------