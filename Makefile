#this is for MinGW

all:
	gcc -Wall -Iinclude -L./lib -lglfw3 -lglu32 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32  openglTest.c