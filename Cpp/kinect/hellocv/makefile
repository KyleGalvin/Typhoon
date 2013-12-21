LIB_GRAPHICS_OPENCV = -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video 

#-lopencv_contrib -lopencv_gpu -lopencv_ts
#-I/usr/include/ni -lOpenNI

all:
	g++ -o hellocv hellocv.c -lSDL -lSDL_image $(LIB_GRAPHICS_OPENCV)
