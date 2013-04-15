//#include "./src/model.hpp"//model aggregates 3d SOM model, 3d camera view, and SDL event controller into a unified state object
//#include "./src/viewer3d.hpp"//view - openGL camera rendering logic
//#include "./src/controller3d.hpp"//controller - flushes SDL event queue in order to manipulates model

int main(){

	//retrieve window initialization settings and initialize the window
	viewer3d* myView = new viewer3d();
	myView->init();

	//create and initialize program model consisting of SOM structure, 3d camera viewer, and event-handler control
	model* myModel = new model();
	myModel->init();
	
	while(!myModel->terminateProgram){
		myView->render(myModel);//draw model to screen with view parameters
		controller3d::emptySDLEventBuffer(myModel,myView);//user acts on the model and view
	}

	return 0;
}

