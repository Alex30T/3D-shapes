#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include "glut.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

float angleX = 0;
float angleY = 0;
float angleZ = 0;

long double minX  = 0;
long double minY  = 0;
long double minZ  = 0;
				 
long double maxX  = 0;
long double maxY  = 0;
long double maxZ  = 0;



float z = -10;


bool flag = true;
bool flag1 = true;

int numVetrices;
int numEdges;
int numFaces;


std::string name = "core.txt";

void read_kb(unsigned char key, int, int) {


	switch (key)
	{
	case 'h':

		angleX += 2.0;

		break;

	case 'j':

		angleY += 2.0;

		break;

	case 'k':

		angleZ += 2.0;

		break;

	case '+':

		z++;
		glTranslatef(0, 0, z);

		break;

	case '-':

		z--;
		glTranslatef(0, 0, z);

		break;
	case 'm':

		if (flag == true) {
			flag = false;


		}
		else {
			flag = true;
		}

		break;

	case 27:

		exit(0);

		break;
	}

	glutPostRedisplay();
}



void keys(int key, int, int)
{
	switch (key)
	{

	case GLUT_KEY_F10:

		if (flag1 == true) {
			flag1 = false;
			glutPostRedisplay();
		}
		else {
			flag1 = true;
			glutPostRedisplay();
		}

		break;
	}
}

class Vertex
{

public: 

	long double x;
	long double y;
	long double z;

	

};

class Edge
{
public:

	long double point1;
	long double point2;


};

class Face
{

public:

	long double edge1;
	long double edge2;
	long double edge3;


};

class Grid
{
	std::vector <Vertex> vertices;
	std::vector <Edge> edges;     
	std::vector <Face> faces;     
	std::vector <bool> locations; 
												   
public:

	bool LoadData()
	{
		std::string line;

		std::ifstream file(name);
		if (!file.is_open())
		{
			std::cout << "ERROR: Failed to open file" << std::endl;
			exit(NULL);
			return 0;
		}
		else
		{

			

			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }),line.end());
			numVetrices = std::stoi(line);
			
			for (int i = 0; i < numVetrices; ++i)
			{
				std::getline(file, line);
				
				std::istringstream s(line);
				long double x = 0, y = 0, z = 0;
				s >> x;
				s.ignore();
				s >> y;
				s.ignore();
				s >> z;
				vertices.push_back({ x, y, z });
				
			}
			

			
			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numEdges = std::stoi(line);
			
			for (int i = 0; i < numEdges; ++i)
			{
				std::getline(file, line);

				std::istringstream s(line);
				long double point1 = 0, point2 = 0;
				s >> point1;
				s.ignore();
				s >> point2;

				edges.push_back({ point1, point2 });

				
			}

			
			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numFaces = std::stoi(line);
			
			for (int i = 0; i < numFaces; ++i)
			{
				std::getline(file, line);

				std::istringstream s(line);
				long double edge1 = 0, edge2 = 0, edge3 = 0;
				s >> edge1;
				s.ignore();
				s >> edge2;
				s.ignore();
				s >> edge3;

				faces.push_back({ edge1, edge2, edge3 });

				
			}

			
		}



		file.close();
	}

	void maxmin() {
		 
		  minX = vertices[0].x;
		  minY = vertices[0].y;
		  minZ = vertices[0].z;
		 
		  maxX = vertices[0].x;
		  maxY = vertices[0].y;
		  maxZ = vertices[0].z;

		for (const auto& vertex : vertices) {

			minX = std::min(minX, vertex.x);
			minY = std::min(minY, vertex.y);
			minZ = std::min(minZ, vertex.z);

			maxX = std::max(maxX, vertex.x);
			maxY = std::max(maxY, vertex.y);
			maxZ = std::max(maxZ, vertex.z);
		}

		std::cout << "Min X: " << minX << ", Max X: " << maxX << std::endl;
		std::cout << "Min Y: " << minY << ", Max Y: " << maxY << std::endl;
		std::cout << "Min Z: " << minZ << ", Max Z: " << maxZ << std::endl;

	}


	void drawGrid() {

		glColor3f(1.0f, 1.0f, 1.0f); 

		glLineWidth(4);

		for (int i = 0; i < numEdges; ++i) {

		

			glBegin(GL_LINES);
			
			glVertex3f(vertices[edges[i].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].point1].z - (maxZ - abs(minZ)) / 2.0);

			glVertex3f(vertices[edges[i].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].point2].z - (maxZ - abs(minZ)) / 2.0);


			glEnd();

		}

	}


	void drawTriangles() {

		glColor3f(0.0f, 0.0f, 1.0f);

	
		for (int i = 0; i < numFaces; ++i) {


			glBegin(GL_TRIANGLES);

			if (faces[i].edge1 < 0) {

				long double a = abs(faces[i].edge1) - 1;

				glVertex3f(vertices[edges[a].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[a].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[a].point2].z - (maxZ - abs(minZ)) / 2.0);

			//	glVertex3f(vertices[edges[a].point1].x, vertices[edges[a].point1].y, vertices[edges[a].point1].z);

			}
			else {

				glVertex3f(vertices[edges[faces[i].edge1].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge1].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge1].point1].z - (maxZ - abs(minZ)) / 2.0);

				//glVertex3f(vertices[edges[faces[i].edge1].point2].x, vertices[edges[faces[i].edge1].point2].y, vertices[edges[faces[i].edge1].point2].z);
			}


			if (faces[i].edge2 < 0) {

				long double b = abs(faces[i].edge2) - 1;

				glVertex3f(vertices[edges[b].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[b].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[b].point2].z - (maxZ - abs(minZ)) / 2.0);

				//glVertex3f(vertices[edges[b].point1].x, vertices[edges[b].point1].y, vertices[edges[b].point1].z);


			}
			else {
				glVertex3f(vertices[edges[faces[i].edge2].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge2].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge2].point1].z - (maxZ - abs(minZ)) / 2.0);

				//glVertex3f(vertices[edges[faces[i].edge2].point2].x, vertices[edges[faces[i].edge2].point2].y, vertices[edges[faces[i].edge2].point2].z);
			}

			if (faces[i].edge3 < 0) {

				long double c = abs(faces[i].edge3) - 1;

				glVertex3f(vertices[edges[c].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[c].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[c].point2].z - (maxZ - abs(minZ)) / 2.0);

			//	glVertex3f(vertices[edges[c].point1].x, vertices[edges[c].point1].y, vertices[edges[c].point1].z);


			}
			else {
				glVertex3f(vertices[edges[faces[i].edge3].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge3].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge3].point1].z - (maxZ - abs(minZ)) / 2.0);

				//glVertex3f(vertices[edges[faces[i].edge3].point2].x, vertices[edges[faces[i].edge3].point2].y, vertices[edges[faces[i].edge3].point2].z);
			}


			glEnd();


		}
	}


	void minmaxLine2() {
		glBegin(GL_LINES);


		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(4);

		glVertex3f(maxX, maxY, -maxZ / 2);
		glVertex3f(maxX - 0.3, maxY, -maxZ / 2);

		glVertex3f(maxX, maxY, -maxZ / 2);
		glVertex3f(maxX, maxY - 0.3, -maxZ / 2);

		glVertex3f(maxX, maxY, -maxZ / 2);
		glVertex3f(maxX, maxY, -maxZ / 2 + 0.3);


		glVertex3f(-maxX, maxY, -maxZ / 2);
		glVertex3f(-maxX + 0.3, maxY, -maxZ / 2);

		glVertex3f(-maxX, maxY, -maxZ / 2);
		glVertex3f(-maxX, maxY - 0.3, -maxZ / 2);

		glVertex3f(-maxX, maxY, -maxZ / 2);
		glVertex3f(-maxX, maxY, -maxZ / 2 + 0.3);



		glVertex3f(maxX, -maxY, -maxZ / 2);
		glVertex3f(maxX - 0.3, -maxY, -maxZ / 2);

		glVertex3f(maxX, -maxY, -maxZ / 2);
		glVertex3f(maxX, -maxY + 0.3, -maxZ / 2);

		glVertex3f(maxX, -maxY, -maxZ / 2);
		glVertex3f(maxX, -maxY, -maxZ / 2 + 0.3);



		glVertex3f(-maxX, -maxY, -maxZ / 2);
		glVertex3f(-maxX + 0.3, -maxY, -maxZ / 2);

		glVertex3f(-maxX, -maxY, -maxZ / 2);
		glVertex3f(-maxX, -maxY + 0.3, -maxZ / 2);

		glVertex3f(-maxX, -maxY, -maxZ / 2);
		glVertex3f(-maxX, -maxY, -maxZ / 2 + 0.3);









		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(4);

		glVertex3f(maxX, maxY, maxZ / 2);
		glVertex3f(maxX - 0.3, maxY, maxZ / 2);

		glVertex3f(maxX, maxY, maxZ / 2);
		glVertex3f(maxX, maxY - 0.3, maxZ / 2);

		glVertex3f(maxX, maxY, maxZ / 2);
		glVertex3f(maxX, maxY, maxZ / 2 - 0.3);


		glVertex3f(-maxX, maxY, maxZ / 2);
		glVertex3f(-maxX + 0.3, maxY, maxZ / 2);

		glVertex3f(-maxX, maxY, maxZ / 2);
		glVertex3f(-maxX, maxY - 0.3, maxZ / 2);

		glVertex3f(-maxX, maxY, maxZ / 2);
		glVertex3f(-maxX, maxY, maxZ / 2 - 0.3);



		glVertex3f(maxX, -maxY, maxZ / 2);
		glVertex3f(maxX - 0.3, -maxY, maxZ / 2);

		glVertex3f(maxX, -maxY, maxZ / 2);
		glVertex3f(maxX, -maxY + 0.3, maxZ / 2);

		glVertex3f(maxX, -maxY, maxZ / 2);
		glVertex3f(maxX, -maxY, maxZ / 2 - 0.3);



		glVertex3f(-maxX, -maxY, maxZ / 2);
		glVertex3f(-maxX + 0.3, -maxY, maxZ / 2);

		glVertex3f(-maxX, -maxY, maxZ / 2);
		glVertex3f(-maxX, -maxY + 0.3, maxZ / 2);

		glVertex3f(-maxX, -maxY, maxZ / 2);
		glVertex3f(-maxX, -maxY, maxZ / 2 - 0.3);

		glEnd();
	}

	void minmaxLine() {


		glBegin(GL_LINES);


		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(4);




		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01 - 0.3, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01 - 0.3);



		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01 + 0.3, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01 - 0.3);





		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01 - 0.3, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, -(maxZ - abs(minZ)) / 2.0 - 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01 + 0.3);



		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01 + 0.3, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, -(maxZ - abs(minZ)) / 2.0 - 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 - 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 - 0.01 + 0.3);




		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(4);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01 + 0.3, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01 + 0.3, (abs(minZ) - maxZ) / 2.0 - 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01 + 0.3);


		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01 - 0.3, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01 + 0.3, (abs(minZ) - maxZ) / 2.0 - 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, (abs(minZ) - maxZ) / 2.0 - 0.01 + 0.3);



		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01 + 0.3, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01 + 0.3, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 - 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01 - 0.3);



		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01 - 0.3, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01 + 0.3, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 - 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01 - 0.3);
		glEnd();


	}


};







void drawAxes()
{
	glLoadIdentity();
	gluLookAt(0, 0, 18.0f, 11.0f, 7.0f, 0, 0, 1, 0);


	glRotatef(angleX , 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();


}

Grid mesh;

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, z);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);
	if (name == "core.txt") {
		mesh.minmaxLine2();
	}
	else {
		mesh.minmaxLine();
	}
	
	if (flag == true) {
		mesh.drawGrid();
	}
	if (flag == false) {
		mesh.drawTriangles();
	}
	mesh.drawGrid();

	if (flag1 == true) {
		drawAxes();
	}


	glutSwapBuffers();
}

void ReshapeWindow(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char* argv[]) {

	mesh.LoadData();
	mesh.maxmin();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Пример вывода 3D графики");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0, 0, 0, 1);

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(read_kb);
	glutSpecialFunc(keys);

	glutMainLoop();

	return 0;
}


