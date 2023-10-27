=====
Chess
=====

This is a demonstration of a simple chess AI that uses basic min/max algorithm

Gameplay
--------
You can't really play in this version. Just watch the AI play itself. Human-player functionality is available in the original C++ version (described below) and may be added to the main C# GUI version in the future.

Controls
________
* Double-arrow button ➜ Move the game forward by 1 move.
* Left/right arrows   ➜ Move backward/forward through the move history.

Getting Started
---------------
This project is a Microsoft Visual Studio project. Open the solution file in Visual Studio to see the code and create your own executable.

If you're on Windows and don't want to install Visual Studio, you can launch the standalone executable as is, which is located at **.\\Chess\\bin\\Release\\net6.0\\win-x64\\Chess.exe**

Playing the Original
--------------------
The chess AI was originally developed in C++ and supported human playability. This version used normal text output to draw the chess board in between turns and gather player input. I wanted to jazz things up though and make a nice GUI though, so I ported the AI over to C# so I could use Microsoft's XNA video-game development environment. Microsoft has since abandoned the XNA environment, but Monogame has taken its place and is now used in the main project. Although I was able to create a decent board display GUI, I discontinued work on this project before I implemented controls for human playability, which is why the C# GUI version only allows AI players.

Anyways, if you want to play against the AI yourself or just prefer the original for some reason, you can find it in the **.\\Original** folder. Just run the make file to build the executable (found at **.\\Original\\bin\\chess.exe**). Player 1 is currently set to a human player, but you can make either player human or AI by modifying the ``CPU1`` and ``CPU2`` parameters at the top of **.\\Original\\src\\main.cpp**.