/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2011-2013 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#include "util_pipeline.h"
#include "util_render.h"
#include <fstream>
#include <iomanip>
#include <GL\freeglut.h>
#include <process.h>
#include <math.h>
#define PI 3.14159265
PXCImage::ImageData data;

float GraphData[240][320];
char *Data1 = new char[99];
bool boolTemp; 
void Kmean();
void Second_Kmean();
void findCenter();
int tempX=0,tempY=0,tempI=0;
int tempX2=0,tempY2=0;
struct handNode
{
   int z;
   float dis,degree;
   int fingerNum;
};
handNode hand[240][320];
int ConvertPositionAngel(int x1,int y1,int x2,int y2){
      double res= atan2((double)(y2-y1),(double)(x2-x1))/ PI * 180.0;
      return (res>=0 && res <=180)?res+=90:((res<0 && res>=-90)? res+=90: res+=450);
  }

void OnKeyboard(unsigned char ch, int x, int y)
{
	// If user hit escape, clean up properly and quit
	if(ch == 27)
	{
		exit(0);
	}

	// Otherwise let user resize the hand based on two scaling factors
	switch(ch)
	{
		case 'k': Kmean(); break;
		case 'l': Second_Kmean(); break;
		case 'j': findCenter(); break;
	}
	
}
void Display(void)
{
	int AlphaTemp=0;
	tempX2=tempX/tempI+20;
	tempY2=tempY/tempI;
	float tempX3=((tempX2-160)/-160.0);
	if(tempX3<0.05 && tempX3>0)
	{
		AlphaTemp=1;
	}
	else if(tempX3<-0.05 && tempX3>-0.1)
	{
		AlphaTemp=2;
	}
	else if(tempX3<-0.15 && tempX3>-0.2)
	{
		AlphaTemp=3;
	}
	else if(tempX3<-0.25 && tempX3>-0.3)
	{
		AlphaTemp=4;
	}
	//int tempX=0,tempY=0,tempI=0;
    glPushMatrix();
	glBegin(GL_POINTS);
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<320;j++)
		{
			glColor3f(GraphData[i][j],GraphData[i][j],GraphData[i][j]); 
			
			switch(hand[i][j].fingerNum)
			{
				case 0:
					glColor3f(0,0,0); 
					break;
				case 1:
					switch(AlphaTemp)
					{
						case 0:
							glColor3f(GraphData[i][j],GraphData[i][j],GraphData[i][j]); 
							break;
						case 1:
							glColor3f(GraphData[i][j]*1.0f, GraphData[i][j]*0.0f, GraphData[i][j]*0.0f); 
							break;
						case 2:
							glColor3f(GraphData[i][j]*1.0f, GraphData[i][j]*1.0f, GraphData[i][j]*0.0f); 
							break;
						case 3:
							glColor3f(GraphData[i][j]*1.0f, GraphData[i][j]*0.0f, GraphData[i][j]*1.0f); 
							break;
						case 4:
							glColor3f(GraphData[i][j]*0.0f, GraphData[i][j]*1.0f, GraphData[i][j]*0.0f); 
							break;
					}
					break;
				case 2:
					glColor3f(0,0,1);
					break;
				case 3:
					glColor3f(0,1,0);
					break;
				case 4:
					glColor3f(0,1,1);
					break;
				case 5:
					glColor3f(1,0,0);
					break;
				case 6:
					glColor3f(1,0,1);
					break;
			}
			glVertex2f((j-160)/-160.0,-(i-120)/120.0); 	
		}
	}
	
	
	glColor3f(1.0f,0,0); 
	
	glVertex2f((tempX2-160)/-160.0,-(tempY2-120)/120.0); 
	
	float max=0;
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<320;j++)
		{
			if(GraphData[i][j]!=0)
			{
				hand[i][j].dis= sqrt((pow((tempX2-j),2.0)+pow((tempY2-i),2.0)));
				hand[i][j].degree=ConvertPositionAngel(tempX,tempY,i,j);

				if((hand[i][j].dis<50.0 && hand[i][j].dis>0)||j>tempX2)
					hand[i][j].fingerNum=0;
				else
					hand[i][j].fingerNum=1;
			}
			else
				hand[i][j].dis=0;
		}
	}
	//findCenter();
	glEnd (); 
	
	
	
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	if(AlphaTemp==1)
		glColor4f(1.0f, 0.0f, 0.0f,1.0f);
	else
		glColor4f(1.0f, 0.0f, 0.0f,0.1f);

	glBegin(GL_POLYGON);
		glVertex2f(0,1);
		glVertex2f(0.05,1);
		glVertex2f(0.05,-1);
		glVertex2f(0,-1);
	glEnd();
	if(AlphaTemp==2)
		glColor4f(1.0f, 1.0f, 0.0f,1.0f);
	else
		glColor4f(1.0f, 1.0f, 0.0f,0.1f);
	glBegin(GL_POLYGON);
		glVertex2f(-0.05,1);
		glVertex2f(-0.1,1);
		glVertex2f(-0.1,-1);
		glVertex2f(-0.05,-1);
	glEnd();
	if(AlphaTemp==3)
		glColor4f(1.0f, 0.0f, 1.0f,1.0f);
	else
		glColor4f(1.0f, 0.0f, 1.0f,0.1f);
	glBegin(GL_POLYGON);
		glVertex2f(-0.15,1);
		glVertex2f(-0.2,1);
		glVertex2f(-0.2,-1);
		glVertex2f(-0.15,-1);
	glEnd();
	if(AlphaTemp==4)
		glColor4f(0.0f, 1.0f, 0.0f,1.0f);
	else
		glColor4f(0.0f, 1.0f, 0.0f,0.1f);
	glBegin(GL_POLYGON);
		glVertex2f(-0.25,1);
		glVertex2f(-0.3,1);
		glVertex2f(-0.3,-1);
		glVertex2f(-0.25,-1);
	glEnd();
	
    glPopMatrix();
    glutSwapBuffers();
}

void findCenter()
{
	tempX2=tempX/tempI+20;
	tempY2=tempY/tempI;
	
	float max=0;
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<320;j++)
		{
			if(GraphData[i][j]!=0)
			{
				hand[i][j].dis= sqrt((pow((tempX2-j),2.0)+pow((tempY2-i),2.0)));
				hand[i][j].degree=ConvertPositionAngel(tempX,tempY,i,j);

				if((hand[i][j].dis<50.0 && hand[i][j].dis>0)||j>tempX2)
					hand[i][j].fingerNum=0;
				else
					hand[i][j].fingerNum=1;
			}
			else
				hand[i][j].dis=0;
		}
	}
}
void Kmean()
{
	//int x1,x2,x3,x4,x5,y1,y2,y3,y4,y5;
	int x[5],y[5];
	double dis[5];
	int temp=-1;
	printf("Kmean Start\n");
	for(int i=0;i<5;i++)
	{
		x[i]=0;
		y[i]=0;
		dis[i]=0;
	}
	for(int i=0;i<240;i++)
	{
		for(int j=0;j<320;j++)
		{
			if(hand[i][j].fingerNum==1)
			{
				temp++;
				if(temp>=0 && temp<=4)
				{
					x[temp]=j;
					y[temp]=i;
					hand[i][j].fingerNum=temp+2;
				}
				else
				{
					double tempDis=999;
					int index;
					for(int k=0;k<5;k++)
					{
						dis[k]=sqrt((pow((x[k]-j),2.0)+pow((y[k]-i),2.0)));
						if(dis[k]<tempDis)
						{
							tempDis=dis[k];
							index=k;
						}
					}
					hand[i][j].fingerNum=index+2;
				}
			}
		}
	}

}
void Second_Kmean()
{
	int x[5],y[5];
	double dis[5];
	int tempx[5],tempy[5],t[5];
	printf("Second Part Start\n");
	for(int i=0;i<5;i++)
	{
		x[i]=0;
		y[i]=0;
		dis[i]=0;
	}
	for(int l=0;l<5;l++)
	{
		for(int i=0;i<240;i++)
		{
			for(int j=0;j<320;j++)
			{
				if(hand[i][j].fingerNum>1)
				{
					tempx[hand[i][j].fingerNum-2]+=j;
					tempy[hand[i][j].fingerNum-2]+=i;
					t[hand[i][j].fingerNum-2]++;
				}
			}
		}
		for(int i=0;i<5;i++)
		{
			x[i]=tempx[i]/t[i];
			y[i]=tempy[i]/t[i];
		}
		for(int i=0;i<240;i++)
		{
			for(int j=0;j<320;j++)
			{
				double tempDis=999;
				int index;
				for(int k=0;k<5;k++)
				{
					dis[k]=sqrt((pow((x[k]-j),2.0)+pow((y[k]-i),2.0)));
					if(dis[k]<tempDis)
					{
						tempDis=dis[k];
						index=k;
					}
				}
				hand[i][j].fingerNum=index+2;
			}
		}

	}
	
	
}
void camaeraCapture(void* p)
{
	int temp1=0,temp2=0;
	UtilRender   rraw(L"Raw Depth Stream");
	UtilPipeline praw;
	UtilCapture *pcapture;
    UtilRender rflt(L"Filtered Depth Stream");
    UtilPipeline pflt;
	pcapture = pflt.QueryCapture();
	if (NULL != pcapture)
		pcapture->SetFilter(PXCCapture::Device::PROPERTY_DEPTH_SMOOTHING,true);
	else {
		wprintf_s(L"Failed to query the capture module\n");
		//return 3;
	}
    pflt.EnableImage(PXCImage::COLOR_FORMAT_DEPTH);
	if (!pflt.Init()) {
		wprintf_s(L"Failed to initialize the pipeline with a depth stream input\n");
		//return 3;
	}
	
    for (bool br=true, bf=true; br||bf; Sleep(5)) {
		if (br) {
			if (praw.AcquireFrame(!bf)) {
				if (praw.IsDisconnected()) {
					if (!bf) wprintf_s(L"Device is unplugged. Exiting ...\n");
					br = false; 
				} else 
					if (!rraw.RenderFrame(praw.QueryImage(PXCImage::IMAGE_TYPE_DEPTH))) 
					{
						br=false;
					}

				
				praw.ReleaseFrame();
			}  
		}

        if (bf) {
			if (pflt.AcquireFrame(!br)) {
				if (pflt.IsDisconnected()) {
					if (!br) wprintf_s(L"Device is unplugged. Exiting ...\n");
					bf = false;
				} 
				else
					if (!rflt.RenderFrame(pflt.QueryImage(PXCImage::IMAGE_TYPE_DEPTH)))
					{
						bf=false;
					}
				PXCImage *depth=pflt.QueryImage(PXCImage::IMAGE_TYPE_DEPTH);
				depth->AcquireAccess(PXCImage::ACCESS_READ,&data);
				
				tempX=0;
				tempY=0;
				tempI=0;
				for(int i=0;i<240;i++)
				{
					for(int j=0;j<320;j++)
					{

						//no1
						if( *(data.planes[0]+2*(i*320+j)+1)*256.0+*(data.planes[0]+2*(i*320+j))<400)
						{
							hand[i][j].z=*(data.planes[0]+2*(i*320+j)+1)*256.0+*(data.planes[0]+2*(i*320+j));
							//hand[i][j].fingerNum=1;
							GraphData[i][j]=1-((*(data.planes[0]+2*(i*320+j)+1)*256.0+*(data.planes[0]+2*(i*320+j)))/1493.0);
							tempX+=j;
							tempY+=i;
							tempI++;
						}
						else
						{
							GraphData[i][j]=0;
							//hand[i][j].fingerNum=0;
						}
					
					}
				}
				
				
				depth->ReleaseAccess(&data);
				pflt.ReleaseFrame();

			} 
		}
	
	}
}
void idle()
{
	glutPostRedisplay();
}
int wmain(int argc, char * argv[]) {
	_beginthread(camaeraCapture, 0, (void*)Data1 );
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(320, 240);
	glutCreateWindow("Hand track demo");
	glPointSize(10.0f);
	glutKeyboardFunc(OnKeyboard);
	glutIdleFunc(idle);
	glutDisplayFunc(Display);		
	glutMainLoop();			
    return 0;
}
