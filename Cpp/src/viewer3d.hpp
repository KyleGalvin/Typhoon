class viewer3d{

	private:

	struct SDLDrawMetaData{
		static const int screen_w=900;
		static const int screen_h=600;
		static const int screen_bpp=32;
		static const int fps=60;
		SDL_Surface *screen;
	}sdl;

	SDL_Surface* cubeTexture;

	void draw3DBox(vector<float> Locations, vector<float> Dimensions, vector<int> Color, float Alpha){
		vector<float>::iterator Di = Dimensions.begin();
		vector<float>::iterator Li = Locations.begin();
	
		glColor4f(Color[0]/255.0,Color[1]/255.0,Color[2]/255.0,Alpha);
		glBindTexture(GL_TEXTURE_2D,1);
		glBegin(GL_QUADS);
		//front
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		//top
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		//bottom
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		//back
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		//left
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		//right
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		glEnd();
	}

	void drawSOM(som* s){
		//scale all values within our unit cube
		vector<float> Locations;
		Locations.push_back(0);
		Locations.push_back(0);
		Locations.push_back(0);

		vector<float> Dimensions;
		Dimensions.push_back(0);
		Dimensions.push_back(0);
		Dimensions.push_back(0);

		for(int i = 0; i < s->get_h(); i++){
			for(int j = 0; j < s->get_w(); j++){
				for(int k = 0; k < s->get_d(); k++){
					Locations[0] = (i+1)/(float)s->get_h();
					Locations[1] = (j+1)/(float)s->get_w();
					Locations[2] = (k+1)/(float)s->get_d();
		
					//divide cubes by putting spacers between them
					Locations[0] *=1.1;
					Locations[1] *=1.1;
					Locations[2] *=1.1;

					Dimensions[0] = 1.0/s->get_h();
					Dimensions[1] = 1.0/s->get_w();
					Dimensions[2] = 1.0/s->get_d();

					//locate the cell to find color data
					vector<float> cell = s->getCell(k + (j * s->get_d()) + (i * s->get_d() * s->get_w()));

					vector<int> Color;
					Color.push_back((int)cell[0]);
					Color.push_back((int)cell[1]);
					Color.push_back((int)cell[2]);
					draw3DBox(Locations, Dimensions, Color, 0.25);
				}
			}
		}
	}

	public:

	bool init(){

		if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
			return false;
		}

		if ( SDL_SetVideoMode( sdl.screen_w,sdl.screen_h,sdl.screen_bpp,SDL_OPENGL ) == NULL ){
			return false;
		}

		SDL_WM_SetCaption( "SDL GL", NULL );
	
	
		//openGL texturing and depth map configuration
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND); //Enable alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f,1.0f);

		glBindTexture(GL_TEXTURE_2D,1);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

		GLint nOfColors;
		GLenum texture_format;
	
		//grab texture as SDL surface	
		SDL_Render::loadimage("./sprites/pxlwhite.png" ,&cubeTexture);	
		//transfer SDL surface into openGL texture
		nOfColors = cubeTexture->format->BytesPerPixel;
		if(nOfColors == 4){
			if(cubeTexture->format->Rmask = 0x000000ff){
				texture_format = GL_RGBA;
			}else{
				texture_format = GL_BGRA;
			}
		}else if(nOfColors == 3){
			if(cubeTexture->format->Rmask = 0x000000ff){
				texture_format = GL_RGB;
			}else{
				texture_format = GL_BGR;
			}
		}
	
		glTexImage2D( GL_TEXTURE_2D,0,nOfColors,cubeTexture->w,cubeTexture->h,0,texture_format,GL_UNSIGNED_BYTE,cubeTexture->pixels);
		
	}
	
	
	void render(model* m){
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,sdl.screen_w/(float)sdl.screen_h,0.1,100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m->cam->step();
//		cout<<"Camera Loc:"<<m->cam->Location[0]<<" "<<m->cam->Location[1]<<" "<<m->cam->Location[2]<<"\n";
		gluLookAt(m->cam->Location[0],m->cam->Location[1],m->cam->Location[2],
			m->cam->Location[0]+(*m->cam->Target)[0],m->cam->Location[1]+(*m->cam->Target)[1],m->cam->Location[2]+(*m->cam->Target)[2],
			(*m->cam->Up)[0],(*m->cam->Up)[1],(*m->cam->Up)[2]);
	
		som* s = &(m->mySOM);
		drawSOM(s);
		SDL_GL_SwapBuffers();
	}

};
