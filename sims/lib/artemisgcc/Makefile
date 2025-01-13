# Makefile for GCC
#
# Author: Daniel Yoshinobu Takada Chino
# Revision: 
#
CC=g++
AR=ar
CFLAGS=-m64 -fPIC
prefix?=/usr/local
exec-prefix?=/usr/local
SRCPATH=../../src/artemis
INCLUDEPATH=../../include/
INCLUDE=-I$(INCLUDEPATH)
SRC=	$(SRCPATH)/image/ImageBase.cpp \
	$(SRCPATH)/image/Pixel.cpp \
	$(SRCPATH)/image/bmp/BmpLib.cpp \
	$(SRCPATH)/image/jpg/JpgLib.cpp \
	$(SRCPATH)/image/png/PngLib.cpp \
	$(SRCPATH)/extractor/DiscreteCosineTransformation.cpp \
	$(SRCPATH)/extractor/HMMDColorSystem.cpp \
	$(SRCPATH)/extractor/XYZColorSystem.cpp \
	$(SRCPATH)/extractor/EdgeHistogram.cpp \
	$(SRCPATH)/extractor/HSVColorSystem.cpp \
	$(SRCPATH)/extractor/YCrCbColorSystem.cpp \
	$(SRCPATH)/extractor/HaralickFeature.cpp \
	$(SRCPATH)/extractor/SRGBColorSystem.cpp
	
OBJS=$(subst .cpp,.o,$(SRC))

STD=-std=c++11

LIBNAME=../libartemis.a

# Implicit Rules
%.o: %.cpp $(HEADERS)
	@echo Compiling $<.
	@$(CC) $(CFLAGS) $(STD) -c $< -o $@ $(INCLUDE)

# Rules
$(LIBNAME): $(OBJS)
	$(AR) -r $(LIBNAME) $(OBJS)

default: $(LIBNAME)

help:
	@echo Arboretum gcc Makefile
	@echo '
	@echo Targets:
	@echo    default: Build libarboretum.a
	@echo    help:    Prints this help screen
	@echo    clean:   Remove all .o files
	@echo    install: Install library and headers
	@echo '
	@echo The install target has two options:
	@echo    prefix = include prefix
	@echo    exec-prefix = lib prefix
	@echo '
	@echo e.g.: make install prefix=... exec-prefix=...
	@echo '
	@echo Current values are:
	@echo    prefix = $(prefix) [$(prefix)/include]
	@echo    exec-prefix = $(exec-prefix) [$(exec-prefix)/lib]

clean:
	rm -f $(OBJS)
	rm -f $(LIBNAME)

install:
	@echo This target is not complete yet.
