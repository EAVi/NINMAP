# NINMAP
A map format and map parser for my Ninja game.

# NINMAP Map Compiler
In the "src" directory, you'll find the source for the map parser.
You may choose to compile the sources, or you can download the latest compiled parser on [github](https://github.com/EAVi/NINMAP/releases)

# NINMAP User-Defined Language
In the "language" directory, you'll find the User-Defined Language
It is a custom language made for Notepad++ with autocomplete and highlighting. 

### NINMAP rules

There are 6 types of objects which constitute a map:

* attribute - required arguments including spawn point and music. One is required for a map.
* background - skies, trees, water, etc.
* block
* enemy
* door - an area which when touched causes the player to teleport
* updoor - like a door, but requires the player to hold "up", and may have a texture.

Details on the arguments can be found in the User-Defined Language in Notepad++ by pressing CTRL-SHFT-SPACE.

"src/constants.hpp" contains more information about the types for each argument.

| Operator Symbol | Description | Precedence | Associativity |
| --- | --- | --- | --- |
| __( )__ | Parentheses | | left-to-right |
| __*__ | Multiply | Multiplicative | left-to-right |
| __/__ | Divide | Multiplicative | left-to-right |
| __+__ | Add | Additive| left-to-right |
| __-__ | Subtract | Additive| left-to-right |
| __:__ | Byte Add (LHS * 256 + RHS) | Additive | left-to-right |

* You can comment out a line using ___#___  
* Whitespace is ignored between operators and delimiters.
* separate arguments with ___,___
* semicolon is not needed at the end of an object, but you are allowed to put one there without error, if it makes you comfortable.

When parsed, the objects are first evaluated, then stable sorted by type, then converted into map binaries. 