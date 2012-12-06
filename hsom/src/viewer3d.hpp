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

	//z-picking with mouse interaction
	static const int BUFSIZE = 8192;
	GLuint selectBuf[BUFSIZE];

	int processHits(int hits, GLuint buffer[]){
		printf ("Click hits = %d\n", hits);

		GLuint *hitPtr = buffer;
		GLuint names;

		GLuint *winningNameListPtr;
		GLuint winningNameCount;
		GLuint shortestDistance = 0xffffffff; 

		for(int i=0; i<hits; i++){
			names = *hitPtr;//the first element of hitPtr record is the number of names we have given the first hit object
			cout<<"namecount:"<<names<<"\n";
			hitPtr++;//the second element is the z-distance
			if(*hitPtr < shortestDistance){
				winningNameCount = names;
				shortestDistance = *hitPtr;
				winningNameListPtr = hitPtr+2;//skip over the min/max values for the hit surface to the name list;
			}
			hitPtr += names+2; //skip over the rest of the hit record
		}

		hitPtr = winningNameListPtr;
		//I only expect one name so far, although multiple are supported
		cout<<"index:"<<*winningNameListPtr<<"\n";
		return *winningNameListPtr;//return name, which is the array integer of our som value
	}
	
	void startPicking(int x, int y){

		GLint viewport[4];

		glSelectBuffer(BUFSIZE,selectBuf);
		glRenderMode(GL_SELECT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glGetIntegerv(GL_VIEWPORT,viewport);
		gluPickMatrix(x,viewport[3]-y,5,5,viewport);
		gluPerspective(45.0,sdl.screen_w/(float)sdl.screen_h,0.1,100);
		glMatrixMode(GL_MODELVIEW);
		glInitNames();

	}

	int stopPicking(){
	
		int hits;
		
		// restoring the original projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glFlush();
		
		// returning to normal rendering mode
		hits = glRenderMode(GL_RENDER);
		
		// if there are hits process them
		if (hits != 0)
			return processHits(hits,selectBuf);
		else
			return -1;

	}
	
	void draw3DBox(vector<float> Locations, vector<float> Dimensions, vector<int> Color, float Alpha){
		vector<float>::iterator Di = Dimensions.begin();
		vector<float>::iterator Li = Locations.begin();
	
		glColor4f(Color[0]/255.0,Color[1]/255.0,Color[2]/255.0,Alpha);
		glBindTexture(GL_TEXTURE_2D,1);
		glBegin(GL_QUADS);
		//front
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
		//bottom
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		//top
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		//back
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0]+Di[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		//right
		glTexCoord2f(0.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]+Di[2]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]+Di[2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(Li[0],Li[1]+Di[1],Li[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3f(Li[0],Li[1],Li[2]);
		//left
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

	void drawSOM(model* m){
		//scale all values within our unit cube
		vector<float> Locations;
		Locations.push_back(0);
		Locations.push_back(0);
		Locations.push_back(0);

		vector<float> Dimensions;
		Dimensions.push_back(0);
		Dimensions.push_back(0);
		Dimensions.push_back(0);

		int selectedCell = m->mySOM.selectedCellSingle;
		set<int> BMUGroup = m->mySOM.selectedBMUGroup;
		vector<int> cellGroup = m->mySOM.selectedCellGroup;
		for(int i = 0; i < m->mySOM.h; i++){
			for(int j = 0; j < m->mySOM.w; j++){
				for(int k = 0; k < m->mySOM.d; k++){

					int index = k + (j * m->mySOM.d) + (i * m->mySOM.d * m->mySOM.w);
						
					Locations[0] = (i+1)/(float)m->mySOM.h;
					Locations[1] = (j+1)/(float)m->mySOM.w;
					Locations[2] = (k+1)/(float)m->mySOM.d;
		
					//divide cubes by putting spacers between them
					Locations[0] *=1.5;
					Locations[1] *=1.5;
					Locations[2] *=1.5;

					Dimensions[0] = 1.0/m->mySOM.h;
					Dimensions[1] = 1.0/m->mySOM.w;
					Dimensions[2] = 1.0/m->mySOM.d;
						
					//locate the cell to find color data
					vector<float> cell = m->mySOM.cells[index];

					vector<int> Color;
					Color.push_back((int)cell[0]);
					Color.push_back((int)cell[1]);
					Color.push_back((int)cell[2]);
					
					//if a cell has been selected, render it seperately
					//selected cells are opaque, and opaque must be rendered first
					if(selectedCell >-1 && selectedCell == index){
							vector<int> selectColor;
							selectColor.push_back(0);
							selectColor.push_back(0);
							selectColor.push_back(0);
							draw3DBox(Locations, Dimensions,selectColor,1);

					//if a BMU group has been described, render it with higher opacity.
					//this takes precedence over the cell group, as the BMU group is contained within it
					}else if(BMUGroup.size()>0 && find(BMUGroup.begin(),BMUGroup.end(),index)!= BMUGroup.end()){
						glDepthMask(GL_FALSE);
						glPushName(index);
						draw3DBox(Locations, Dimensions,Color,0.9);
						glPopName();
						glDepthMask(GL_TRUE);


					//if a cell group has been described, render it with higher opacity
					}else if(find(cellGroup.begin(),cellGroup.end(),index)!=cellGroup.end()){
						glDepthMask(GL_FALSE);
						glPushName(index);
						draw3DBox(Locations, Dimensions,Color,0.5);
						glPopName();
						glDepthMask(GL_TRUE);

					//otherwise, render with normal som viewing parameters
					}else{
							//switch to transparent rendering mode.
							//depth testing must be unwritable for transparency to look ok
							glDepthMask(GL_FALSE);

							glPushName(index);
							draw3DBox(Locations, Dimensions, Color, 0.10);
							glPopName();

							//back to opaque rendering
							glDepthMask(GL_TRUE);
					}	
				}
			}
		}
	
	}

	public:
	
	bool mousePickingPass;

	bool init(){
		//only true when the controller recieves a leftmouseup event
		mousePickingPass = false;

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
		glEnable(GL_CULL_FACE);//backface culling for properly rendering transparent cubes
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

		//if the controller has detected a mouse click, it will use this bool to trigger a selection
		//Prevent race conditions: Read the value once into a temp var to make logic decisions from over the course of the function 
		//otherwise we could find no picking here, but at the end decide to do a cleanup of picking routine.
		bool tempMPP = mousePickingPass;

		//if we are waiting for a picking pass, we interrupt the rendering procedure.
		//at the end of this function we do a cleanup interrupt as well
		if (tempMPP){
			startPicking(m->mouseX,m->mouseY);			
		}else{

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(45.0,sdl.screen_w/(float)sdl.screen_h,0.1,100);
	
			glMatrixMode(GL_MODELVIEW);
		}

		//glClearDepth(1.0f);

		glLoadIdentity();

		m->cam->step();
		gluLookAt(m->cam->Location[0],m->cam->Location[1],m->cam->Location[2],
			m->cam->Location[0]+(*m->cam->Target)[0],m->cam->Location[1]+(*m->cam->Target)[1],m->cam->Location[2]+(*m->cam->Target)[2],
			(*m->cam->Up)[0],(*m->cam->Up)[1],(*m->cam->Up)[2]);

		drawSOM(m);
		SDL_GL_SwapBuffers();

		//clean up our selection triggering after determining whether an item was selected or not
		if(tempMPP){
			m->mySOM.selectedCellSingle = stopPicking();
			mousePickingPass = !mousePickingPass;
		}
	}

};
