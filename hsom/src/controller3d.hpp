namespace controller3d{

	void SDL_WindowContext(model* m){
		if(m->event.type==SDL_QUIT){
			m->terminateProgram = true;
		}
	}

	void QueryContext(model* m){
		if(m->event.type == SDL_KEYDOWN){
			switch(m->event.key.keysym.sym){
				case SDLK_ESCAPE: m->mySOM.clearCellGroup(); break;
				case SDLK_n: m->mySOM.getCloudAll(m->mySOM.getCell(m->mySOM.getSelectedCell()),0.1) ; break;
				case SDLK_m: m->mySOM.getCloudBMU(m->mySOM.getCell(m->mySOM.getSelectedCell()),0.1) ; break;
				default: break;
			}
		}
	}

	void TrainingContext(model* m){
		if(m->event.type == SDL_KEYDOWN){
			switch(m->event.key.keysym.sym){
				case SDLK_SPACE: m->mySOM.trainStep() ; break;
				case SDLK_n: m->mySOM.trainPass() ; break;
				case SDLK_m: m->mySOM.trainAll(); break;
				default: break;
			}
		}
	}

	void FirstPersonCameraContext(model* m,viewer3d* v){
		if(m->event.type == SDL_KEYDOWN){
			switch(m->event.key.keysym.sym){
				case SDLK_w:m->cam->ForwardForce->magnitude += 0.1 ; break;
				case SDLK_s:m->cam->ForwardForce->magnitude -= 0.1 ; break;
				case SDLK_d:m->cam->RightForce->magnitude -= 0.1 ; break;
				case SDLK_a:m->cam->RightForce->magnitude += 0.1 ; break;
				case SDLK_e:m->cam->UpForce->magnitude -= 0.1 ; break;
				case SDLK_q:m->cam->UpForce->magnitude +=0.1 ; break;
				case SDLK_i:m->cam->rotateX(5) ; break;
				case SDLK_k:m->cam->rotateX(-5) ; break;
				case SDLK_j:m->cam->rotateY(5) ; break;
				case SDLK_l:m->cam->rotateY(-5) ; break;
				case SDLK_o:m->cam->rotateZ(5) ; break;
				case SDLK_u:m->cam->rotateZ(-5) ; break;
			}
		}else if(m->event.type == SDL_KEYUP){
			switch(m->event.key.keysym.sym){
				case SDLK_w:m->cam->ForwardForce->magnitude -= 0.1 ; break;
				case SDLK_s:m->cam->ForwardForce->magnitude += 0.1 ; break;
				case SDLK_d:m->cam->RightForce->magnitude += 0.1 ; break;
				case SDLK_a:m->cam->RightForce->magnitude -= 0.1 ; break;
				case SDLK_e:m->cam->UpForce->magnitude += 0.1 ; break;
				case SDLK_q:m->cam->UpForce->magnitude -=0.1 ; break;
			}
		}else if(m->event.type == SDL_MOUSEBUTTONUP ){
			//mouse picking triggers hsom creation

			//find clicked cell (if any). 
			//mousePickingPass will trigger the viewer to interrupt a render cycle for a picking pass
			m->mouseX = m->event.button.x;
			m->mouseY = m->event.button.y;
			v->mousePickingPass = true; 
			//find all cells similar to selected cell
			//find subset of training vectors that map to cell set
			//create new SOM consisting only of training vector subset
		}
	}
	
	//flush our sdl event pipeline
	void emptySDLEventBuffer(model* m,viewer3d* v){
		while(SDL_PollEvent(&m->event)){
			SDL_WindowContext(m);
			FirstPersonCameraContext(m,v);
			TrainingContext(m);
			//only allow query operations on a completely trained som with a user-selected cell
			if(m->mySOM.getIterations() == 0 && m->mySOM.getSelectedCell() > -1){
				QueryContext(m);
			}
		}
		
	}

}
