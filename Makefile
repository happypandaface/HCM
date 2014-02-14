#this is for MinGW

#	gcc -Wall -Iinclude -L./lib -lglfw3 -lglu32 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32  openglTest.c

all:
	gcc -DUNICODE -DWINVER=0x0501 -D_GLFW_USE_CONFIG_H -I./include -o openglTest.c.obj -c openglTest.c
	ar cr objects.a openglTest.c.obj
	gcc -mwindows -Wl,--whole-archive objects.a -Wl,--no-whole-archive  -o openglTest.exe -Wl,--out-implib,libboing.dll.a -Wl,--major-image-version,0,--minor-image-version,0  ./lib/libglfw3.a -lglu32 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32