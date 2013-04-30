
PATH += "./"

TARGET_SRC = 
TARGET_SRC += main.c
TARGET_SRC += Battle.c
TARGET_SRC += MapManager.c
TARGET_SRC += MidLayerDisp.c
TARGET_SRC += MidLayerStr.c

CC = gcc
OUTPUT_NAME = -o ../e_main

prepartion:

all: 
	cd src/ && ${CC} ${OUTPUT_NAME} ${TARGET_SRC}

go: all

