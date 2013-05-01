
PATH += "./"

TARGET_SRC = 
TARGET_SRC += main.c
TARGET_SRC += Battle.c
TARGET_SRC += MapManager.c
TARGET_SRC += MidLayerDisp.c
TARGET_SRC += MidLayerStr.c

CC = gcc
OUTPUT_FILE = e_main
OUTPUT = -o ../${OUTPUT_FILE}

clean:
	touch ${OUTPUT_FILE}
	rm ${OUTPUT_FILE}

preparation:


all: 
	cd src/ && ${CC} ${OUTPUT} ${TARGET_SRC}

rebuild: clean preparation all

