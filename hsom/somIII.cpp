#include <thread>
#include "./src/model.hpp"//model aggregates 3d SOM model, 3d camera view, and SDL event controller into a unified state object
#include "./src/viewer3d.hpp"//view - openGL camera rendering logic
#include "./src/controller3d.hpp"//controller - flushes SDL event queue in order to manipulates model


void viewThreadRun(model* myModel, viewer3d* myView){
	while(!myModel->terminateProgram){
		//take read-access
		ReadLock rLock(r_w_lock);
		myView->render(myModel);//draw model to screen with view parameters
		rLock.unlock();
	}
}

void controllerThreadRun(model* myModel, viewer3d* myView){
	while(!myModel->terminateProgram){
		//lock read/write access
		WriteLock wLock(r_w_lock);
		controller3d::emptySDLEventBuffer(myModel,myView);//user acts on the model and view
		wLock.unlock();
	}
}

int main(){

	//retrieve window initialization settings and initialize the window
	viewer3d* myView = new viewer3d();
	myView->init();

	//create and initialize program model consisting of SOM structure, 3d camera viewer, and event-handler control
	model* myModel = new model();
	myModel->init();

	//our view runs independently of our controller, allowing us to see the training in real-time
	//however, the controller is still in the same thread as the training logic
	//meaning we cannot position/orient the camera while in the middle of a training session
	std::thread t(controllerThreadRun,myModel,myView);
	viewThreadRun(myModel,myView);

	t.join();
	return 0;
}

