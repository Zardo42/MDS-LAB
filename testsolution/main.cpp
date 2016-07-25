#include <iostream>
#include <string>
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.inl"
#include <conio.h>
#include <vector>

using namespace std;



float planeVsPoint(glm::vec3 N, glm::vec3 P, glm::vec3 C)
{
	float d = glm::dot(N, P);
	float val = glm::dot(N, C) - d;
	if (val == 0)
	{
		cout << "(" << C.x << ", " << C.y << ", " << C.z << ") is on the plane." << endl;
	}
	if (val > 0)
	{
		cout << "(" << C.x << ", " << C.y << ", " << C.z << ") is in front of the plane." << endl;
	}
	if (val < 0)
	{
		cout << "(" << C.x << ", " << C.y << ", " << C.z << ") is behind the plane." << endl;
	}
	return val;
}

glm::vec3 calculateNormalOfThePlaneWith3Points(glm::vec3 P0, glm::vec3 P1, glm::vec3  P2 )
{
	return glm::cross((P0 - P1), (P0 - P2));
}


//t = dot(planeNormal, pointOnPlane – p0) / dot(planeNormal, p1 – p0)
/*If: • t > 0 and t < 1: Intersection has occurred between the end points.
• t == 0: Intersection at first end point.
• t == 1: Intersection at second end point.
• t > 1: Intersection beyond second end point.
• t < 0: Intersection before first end point.


*/
float linePlaneIntersect(glm::vec3 N, glm::vec3 P, glm::vec3 P0, glm::vec3 P1, glm::vec3 & PossibleNew)
{
	float t = glm::dot(N, (P - P0)) / glm::dot(N, (P1 - P0));
	cout << "Segment (" << P0.x << ", " << P0.y << ", " << P0.z << ")" << " to (" << P1.x << ", " << P1.y << ", " << P1.z << ")" << " Value of t is " << t << ";";

	if (t > 0 && t < 1)
	{
		cout << " Intersection has occurred between the end points." << endl;
		glm::vec3 p2 = P0 + ((P1 - P0) * t);
		PossibleNew = p2;
	}

	if (t == 0) {cout << " Intersection at first end point." << endl;		PossibleNew = P0;}
	if (t == 1)	{cout << " Intersection at second end point." << endl;		PossibleNew = P1;}
	if (t > 1)  {cout << " Intersection beyond second end point." << endl;}
	if (t < 0)	{cout << " Intersection before first end point." << endl;}

	return t;
}


void PrintVertices(vector <glm::vec3> & V)
{
	glm::vec3 temp;
	for (auto i = V.begin(); i != V.end(); i++)
	{
		cout << "  (" << i->x << ", " << i->y << ", " << i->z << ")" << endl;
	}
}

void triangleCutter(vector <glm::vec3> & OV, vector <glm::vec3> & AV, vector <glm::vec3> & BV, glm::vec3 N, glm::vec3 P)
{
	glm::vec3 temp;
	for (auto i = OV.begin(); i != OV.end(); i++)
	{
		cout << "Original Triangle (" << i->x << ", " << i->y << ", " << i->z << ");  ";
		temp.x = i->x; 	temp.y = i->y; 	temp.z = i->z;

		float t = planeVsPoint(N, P, temp);
		if (t > 0)
		{
			AV.push_back(temp);	// Original Vertex is Above
		}
		if (t < 0)
		{
			BV.push_back(temp); // Original Vertex is Below
		}
		if (t == 0)
		{
			BV.push_back(temp); // Original Vertex is on the plane so add to both
			AV.push_back(temp);
		}
	}

	cout << endl << "Checking segments" << endl;


	if (OV.size() == 3)
	{
		// Algug = we got passed a triangle

		glm::vec3 Newpoint;
		float Line1t = linePlaneIntersect(N, P, OV[0], OV[1], Newpoint); // Triangle line segements in clockwise order?
		if (Line1t > 0 && Line1t < 1)
		{
			cout << "   Segment 1 Intersection at (" << Newpoint.x << ", " << Newpoint.y << ", " << Newpoint.z << ") adding to both lists" << endl;
			AV.push_back(Newpoint);
			BV.push_back(Newpoint);
		}


		float Line2t = linePlaneIntersect(N, P, OV[1], OV[2], Newpoint);
		if (Line2t > 0 && Line2t < 1)
		{
			cout << "   Segment 2 Intersection at (" << Newpoint.x << ", " << Newpoint.y << ", " << Newpoint.z << ") adding to both lists" << endl;
			AV.push_back(Newpoint);
			BV.push_back(Newpoint);
		}
		float Line3t = linePlaneIntersect(N, P, OV[2], OV[0], Newpoint);
		if (Line3t > 0 && Line3t < 1)
		{
			cout << "   Segment 3 Intersection at (" << Newpoint.x << ", " << Newpoint.y << ", " << Newpoint.z << ") adding to both lists" << endl;
			AV.push_back(Newpoint);
			BV.push_back(Newpoint);
		}


	}
	else
	{
		cout << "oops = we got passed something that does not have 3 vertices " << endl;
	}
}


int main()
{
	// 3 points on plane through origin of x and y
	glm::vec3 Pv1 = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 Pv2 = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 Pv3 = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 Np = calculateNormalOfThePlaneWith3Points(Pv1, Pv2, Pv3); // calculated normal
	glm::vec3 Pp = Pv2; // is now one example point on our plane
	vector <glm::vec3> originalVertices;
	vector <glm::vec3> aboveVertices;
	vector <glm::vec3> belowVertices;

	{
		cout << endl << "Test vertices 1" << endl << endl;
		glm::vec3 Tv1 = glm::vec3(0.0, 0.0, 4.0); // This is our triangle vertex 1
		glm::vec3 Tv2 = glm::vec3(1.0, 0.0, 5.0); // 2
		glm::vec3 Tv3 = glm::vec3(0.5, 0.0, 6.0); // 3

		originalVertices.clear();
		aboveVertices.clear();
		belowVertices.clear();

		originalVertices.push_back(Tv1);
		originalVertices.push_back(Tv2);
		originalVertices.push_back(Tv3);
		triangleCutter(originalVertices, aboveVertices, belowVertices, Np, Pp);

		cout << endl << "Above vertices" << endl;
		PrintVertices(aboveVertices);
		cout << endl << "Behind vertices" << endl;
		PrintVertices(belowVertices);

	}

	{
		cout << endl << "Test vertices 2" << endl << endl;
		glm::vec3 Tv1 = glm::vec3(0.0, 0.0, -1.0); // This is our triangle vertex 1
		glm::vec3 Tv2 = glm::vec3(1.0, 0.0, -0.5); // 2
		glm::vec3 Tv3 = glm::vec3(0.5, 0.0, 1.0); // 3

		originalVertices.clear();
		aboveVertices.clear();
		belowVertices.clear();

		originalVertices.push_back(Tv1);
		originalVertices.push_back(Tv2);
		originalVertices.push_back(Tv3);
		triangleCutter(originalVertices, aboveVertices, belowVertices, Np, Pp);

		cout << endl << "Above vertices" << endl;
		PrintVertices(aboveVertices);
		cout << endl << "Behind vertices" << endl;
		PrintVertices(belowVertices);
	}

	{
		cout << endl << "Test vertices 3" << endl << endl;
		glm::vec3 Tv1 = glm::vec3(0.0, 0.0, -1.0); // This is our triangle vertex 1
		glm::vec3 Tv2 = glm::vec3(1.0, 0.0, 0.0); // 2
		glm::vec3 Tv3 = glm::vec3(0.5, 0.0, 1.0); // 3

		originalVertices.clear();
		aboveVertices.clear();
		belowVertices.clear();

		originalVertices.push_back(Tv1);
		originalVertices.push_back(Tv2);
		originalVertices.push_back(Tv3);
		triangleCutter(originalVertices, aboveVertices, belowVertices, Np, Pp);

		cout << endl << "Above vertices" << endl;
		PrintVertices(aboveVertices);
		cout << endl << "Behind vertices" << endl;
		PrintVertices(belowVertices);
	}


	cout << "Press any key: ";
	_getch();

	return (0);
}