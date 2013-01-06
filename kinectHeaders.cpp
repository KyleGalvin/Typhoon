//OpenCV
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

//OpenNI
#include <ni/XnCppWrapper.h>

#define SAMPLE_XML_PATH "/usr/share/openni/SamplesConfig.xml"

Context g_context;
ScriptNode g_scriptNode;
DepthGenerator g_depth;
ImageGenerator g_image;
DepthMetaData g_depthMD;
ImageMetaData g_imageMD;

int main(){
	XnStatus rc;

	EnumerationErrors errors;
	rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	Context context;
	context.Init();

	return 0;
}
