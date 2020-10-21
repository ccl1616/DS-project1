# DS-project1
write a Tetris alike program but w/ text input&output.

belows are file description

# cpp

game.cpp is the original version for which would crash by -O2
  - reason: some silly loop mistake which cause accessing to array[-1]
  
loopgame.cpp is the revision of game.cpp, change the recursive into loop
  - but still crash after -O2, so somehow some problem still hasn't solve
  - find the bug by print out the invalid coordinate, thus change into the version of "drop onto the row=highest refpt -1" (change refpt, add h_log)
  
add4row.cpp is because of the special case
  - which need to consider the shape is valid after all row elimination
  
# others
ref-point.txt
  for saving different version of reference point
  
fstream related
  change freopen into fstream
  
.data
  all of the testcases
  
 fortests
  - game.cpp is add4fstream
  - data are others data
