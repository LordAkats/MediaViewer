ULTRAENGINEPATH = C:/Program Files (x86)/Steam/steamapps/common/Ultra App Kit
SRC = Source/*.cpp
FLAGS = -I/usr/include/freetype2 -I/usr/include/fontconfig -D_ULTRA_APPKIT "-I$(ULTRAENGINEPATH)/Include"
LFLAGS = -no-pie -lX11 -lpthread -lXft -lXext -lXrender -lXcursor -ldl
OUT = MediaViewer$(PRODUCT_SUFFIX)

x64: $(SRC)
	g++ $(SRC) "$(ULTRAENGINEPATH)/Library/Linux/x64/$(CONFIGNAME)/AppKit.o" -o "$(OUT)" $(LFLAGS) $(FLAGS) $(CONFIGFLAGS)

clean:
	rm -f "$(OUT)"