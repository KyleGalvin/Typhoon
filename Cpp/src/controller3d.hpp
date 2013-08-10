namespace controller3d{

	void SDL_WindowContext(model* m){
		if(m->event.type==SDL_QUIT){
			m->terminateProgram = true;
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

	void FirstPersonCameraContext(model* m){
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
		}else if(m->event.type == SDL_MOUSEMOTION ){
			//MouseMove(event.motion.xrel, event.motion.yrel, sdl.screen_h/2,sdl.screen_w/2);
		}
	}
	
	//flush our sdl event pipeline
	void emptySDLEventBuffer(model* m){
		while(SDL_PollEvent(&m->event)){
			SDL_WindowContext(m);
			FirstPersonCameraContext(m);
			TrainingContext(m);
		}
		
	}

}
