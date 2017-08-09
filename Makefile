PROJECT_NAME := esp32-weather

flash: all

all: main/indexhtml.h

main/indexhtml.h: data/index.html
	python data2h.py data/index.html main/indexhtml.h

include $(IDF_PATH)/make/project.mk