#include <iostream>
#include <vector>
#include <math.h>
#include <GLUT/glut.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;


const float width = 800, height = 600;
const float paperR = 0.992f, paperG = 0.96f, paperB = 0.9f;

vector<vec2> strokePoints, endPoints;
vector<float> pointsInk, pointsSize;
float inkQuantity, myTime;
int pointIndex = -1;

float myNormalize(float f, float l)
{
	return f / l * 2.0f - 1.0f;
}

float mylength(vec2 a, vec2 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void Display(void)
{
	glClearColor(paperR, paperG, paperB, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < strokePoints.size(); i++)
	{
	
		float inkProportion = 1.0f - pointsInk[i];

		if (inkProportion > 1.0f)
		{
			inkProportion = 1.0f;
			continue;
		}
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		vec3 pointColor = vec3(paperR, paperG, paperB) * inkProportion;

		glPointSize(pointsSize[i]);

		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		//glColor4f(pointColor.x, pointColor.y, pointColor.z, pointsInk[i]);
		//glColor4f(pointColor.x, pointColor.y, pointColor.z, 1.0);
		glColor4f(0.0f, 0.0f, 0.0f, pointsInk[i]);
		glVertex2f(strokePoints[i].x, strokePoints[i].y);
		glEnd();

		glDisable(GL_BLEND);
	}
	
	glutSwapBuffers();
}

void myTimer(int time)
{
	myTime += 0.1f;
	glutTimerFunc(100, myTimer, 1);
}

void mouseClick(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		vec2 temp = vec2(myNormalize(x, width), -myNormalize(y, height));
		strokePoints.push_back(temp);

		pointIndex++;

		inkQuantity = 0.6f;
		myTime = 0.0f;
		pointsInk.push_back(inkQuantity);
		pointsSize.push_back(inkQuantity * 15.0f + 15.0f);
		glutPostRedisplay();
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		vec2 temp = vec2(myNormalize(x, width), -myNormalize(y, height));
		endPoints.push_back(temp);
	}
}

void mouseMove(int x, int y)
{
	vec2 newStrokePoint = vec2(myNormalize(x, width), -myNormalize(y, height));
	vec2 oldStrokePoint = strokePoints[pointIndex];
	float oldPointInk = pointsInk[pointIndex];
	float oldPointSize = pointsSize[pointIndex];
	float distance = mylength(newStrokePoint, oldStrokePoint);
	
	if (distance > 0.01f)
	{

		strokePoints.push_back(newStrokePoint);

		inkQuantity = inkQuantity - distance * 0.75f;

		float newPointInk, newPointSize;

		if (myTime < 2.0f)
		{
			newPointInk = inkQuantity + +0.4f*myTime / 2.0f;
			pointsInk.push_back(newPointInk);
		}
		else
		{
			newPointInk = inkQuantity + +0.4f;
			pointsInk.push_back(newPointInk);
		}

		newPointSize = newPointInk * 15.0f + 15.0f + pointsSize[pointIndex] * 0.2f;
		pointsSize.push_back(newPointSize);

		if (distance > 0.015f)
		{
			float interPointNum = distance / 0.01f;

			for (float i = 1; i < interPointNum; i++)
			{
				vec2 interPoint = (i / interPointNum)*newStrokePoint + ((interPointNum - i) / interPointNum)*oldStrokePoint;
				float interInkQuantity = (i / interPointNum)*newPointInk + ((interPointNum - i) / interPointNum)*oldPointInk;
				float interPointSize = (i / interPointNum)*newPointSize + ((interPointNum - i) / interPointNum)*oldPointSize;
				strokePoints.push_back(interPoint);
				pointsInk.push_back(interInkQuantity);
				pointsSize.push_back(interPointSize);
			}
			//pointIndex += interPointNum - 1;
		}

		//pointIndex++;
		pointIndex = strokePoints.size() - 1;
		myTime = 0.0f;
	}

	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(width, height);
	glutCreateWindow("Black Ink Painting");
	glutDisplayFunc(Display);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutTimerFunc(100, myTimer, 1);
	cout << "Painting rule:" << endl;
	cout << "The more time your brush stays, the bigger the dot is." << endl;
	cout << "The closer your brush is to the end of stroke, the lighter the ink is." << endl;
	glutMainLoop();
	return 0;
}