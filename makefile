#######################
## Galaxian          ##
## Authors:          ##
## 	Justin Ricks ##
#######################

###################
## The Real Deal ##
###################
a.out: gameDriver.cpp game.o uiDraw.o uiInteract.o score.o enemyProjectile.o playerProjectile.o vector.o
	g++ -o a.out gameDriver.cpp enemyProjectile.o game.o score.o playerProjectile.o uiDraw.o uiInteract.o vector.o -lglut -lGLU -lGL

#####################
## Driver Programs ##
#####################
drawDriver: drawTest.cpp uiDraw.o uiInteract.o vector.o
	g++ -o drawDriver drawTest.cpp vector.o uiDraw.o uiInteract.o -lglut -lGLU -lGL

#########################
## Dependencies (.o's) ##
#########################
game.o: game.cpp game.h
	g++ -c game.cpp
playerProjectile.o: game.o playerProjectile.h playerProjectile.cpp
	g++ -c playerProjectile.cpp
enemyProjectile.o: game.o enemyProjectile.h enemyProjectile.cpp
	g++ -c enemyProjectile.cpp
vector.o: vector.cpp vector.h point.h
	g++ -c vector.cpp
socre.o: score.cpp score.h point.h
	g++ -c score.cpp
uiDraw.o: uiDraw.cpp uiDraw.h point.h
	g++ -c uiDraw.cpp
uiInteract.o: uiInteract.cpp uiInteract.h point.h
	g++ -c uiInteract.cpp
########################
## Additional Options ##
########################
clean:
	rm *~ *.o *#
tar:
	tar -cf project4.tar *.cpp *.h makefile
