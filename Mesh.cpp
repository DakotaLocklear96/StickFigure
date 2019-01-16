#include "stdafx.h"
#include <fstream>      // For input streams from files
#include <string>       // For the string type
#include <sstream>      // For streams from strings
#include "Mesh.h"
#include <math.h>
using namespace std;
int maxAngle = 360;
const double PI = 3.141592653589793;

CMesh::CMesh(void)
{
}


CMesh::~CMesh(void)
{
}


void CMesh::InitGL()
{
	for (PTV v = m_triangles.begin(); v != m_triangles.end(); v++)
	{
		normalArray.push_back(m_normals[v->n]);
		vertexArray.push_back(m_vertices[v->v]);
		texArray.push_back(m_tvertices[v->t]);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(glm::vec3),
		&vertexArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(glm::vec3),
		&normalArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
	glBufferData(GL_ARRAY_BUFFER, texArray.size() * sizeof(glm::vec2),
		&texArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void CMesh::RenderGL()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
}

void CMesh::AddTriangleVertex(int v, int n, int t)
{
	TV tv;
	tv.v = v;
	tv.n = n;
	tv.t = t;
	m_triangles.push_back(tv);
}

//
// Name :         CMesh::AddFlatQuad()
// Description :  Add a quadrangle to the triangle mesh.
//
void CMesh::AddFlatQuad(int a, int b, int c, int d, int n)
{

	// First triangle
	AddTriangleVertex(a, n, -1);
	AddTriangleVertex(b, n, -1);
	AddTriangleVertex(c, n, -1);

	// Second triangle
	AddTriangleVertex(a, n, -1);
	AddTriangleVertex(c, n, -1);
	AddTriangleVertex(d, n, -1);
}


void CMesh::AddQuad(int a, int b, int c, int d)
{
	// First triangle
	AddTriangleVertex(a, a, -1);
	AddTriangleVertex(b, b, -1);
	AddTriangleVertex(c, c, -1);

	// Second triangle
	AddTriangleVertex(a, a, -1);
	AddTriangleVertex(c, c, -1);
	AddTriangleVertex(d, d, -1);
}


//
// Name :         CMesh::ComputeSmoothNormals()
// Description :  Compute a normal for each vertex that is the
//                average of the surface normals incident on the 
//                vertex.
//
void CMesh::ComputeSmoothNormals(void)
{
	m_normals.resize(m_vertices.size());
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_normals[i] = glm::vec3(0, 0, 0);

	for (PTV v = m_triangles.begin(); v != m_triangles.end(); )
	{
		// Collect up the vertices of a triangle...
		int a = v->v;
		v++;
		int b = v->v;
		v++;
		int c = v->v;
		v++;

		// Surface normal
		glm::vec3 normal = glm::cross(m_vertices[b] - m_vertices[a],
			m_vertices[c] - m_vertices[a]);
		normal *= 1 / normal.length();

		// Add to the incident vertices normals
		m_normals[a] += normal;
		m_normals[b] += normal;
		m_normals[c] += normal;

		// Normalize the normals
		for (unsigned int i = 0; i < m_vertices.size(); i++)
			m_normals[i] *= 1 / m_normals[i].length();
	}
}

void CMesh::LoadOBJ(const char* filename)
{
	ifstream str(filename);
	if (!str)
	{
		AfxMessageBox(L"File not found");
		return;
	}

	string line;
	while (getline(str, line))
	{
		istringstream lstr(line);

		string code;
		lstr >> code;
		if (code == "v")
		{
			double x, y, z;
			lstr >> x >> y >> z;
			AddVertex(glm::vec3(x, y, z));
		}
		else if (code == "vn")
		{
			double a, b, c;
			lstr >> a >> b >> c;
			AddNormal(glm::vec3(a, b, c));
		}
		else if (code == "vt")
		{
			double s, t;
			lstr >> s >> t;
			AddTexCoord(glm::vec2(s, t));
		}
		else if (code == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				char slash;
				int v, t, n;
				lstr >> v >> slash >> t >> slash >> n;
				AddTriangleVertex(v - 1, n - 1, t - 1);
			}
		}

	}
}

void CMesh::CreateCube()
{
	AddVertex(glm::vec3(-0.5, -0.5, 0.5));
	AddVertex(glm::vec3(-0.5, 0.5, 0.5));
	AddVertex(glm::vec3(0.5, 0.5, 0.5));
	AddVertex(glm::vec3(0.5, -0.5, 0.5));
	AddVertex(glm::vec3(-0.5, -0.5, -0.5));
	AddVertex(glm::vec3(-0.5, 0.5, -0.5));
	AddVertex(glm::vec3(0.5, 0.5, -0.5));
	AddVertex(glm::vec3(0.5, -0.5, -0.5));

	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));

	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(0, 5, 1);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(2, 5, 3);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(3, 0, 1);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(6, 0, 3);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(0, 3, 1);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(7, 3, 3);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(5, 2, 1);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(2, 2, 3);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(5, 5, 1);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(7, 5, 3);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(4, 1, 1);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(1, 1, 3);
}

void CMesh::CreateCylinder(double radius,double height)
{
	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));

	//Bottom Circle
	int j = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(0., 0., 0.));
			j--;
		}
		else
		{
			double angRad = (2 * j * PI) / maxAngle;
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, 0.));
			j++;
		}
	}

	for (int i = 0; i < maxAngle * 3; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Top Circle
	int k = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(0., 0., height));
			k--;
		}
		else
		{
			double angRad = (2 * k * PI) / maxAngle;
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, 1.));
			k++;
		}
	}

	for (int i = (maxAngle * 3); i < maxAngle * 6; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Side Cylinder Wall bottom up
	int sW = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		double angRad = (sW * i * PI) / maxAngle;
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, 0.));
			sW--;
		}
		else
		{
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, height));
			sW++;
		}
	}

	for (int i = (maxAngle * 6); i < maxAngle * 9; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Side Cylinder Wall Top Down
	int sW2 = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		double angRad = (sW2 * i* PI) / maxAngle;
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, height));
			sW2--;
		}
		else
		{
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, 0.));
			sW2++;
		}
	}

	for (int i = (maxAngle * 9); i < maxAngle * 12; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

}

void CMesh::CreateLetter()
{
	//Front
	AddVertex(glm::vec3(0.0, 0.0, 0.0));
	AddVertex(glm::vec3(0.0, 3.0, 0.0));
	AddVertex(glm::vec3(0.75, 3.0, 0.0));
	AddVertex(glm::vec3(0.75, 0.0, 0.0));
	AddVertex(glm::vec3(3.0, 0.0, 0.0));
	AddVertex(glm::vec3(3.0, 0.75, 0.0));
	AddVertex(glm::vec3(0.75, 0.75, 0.0));

	//Back
	AddVertex(glm::vec3(0.0, 0.0, 1.0));
	AddVertex(glm::vec3(0.0, 3.0, 1.0));
	AddVertex(glm::vec3(0.75, 3.0, 1.0));
	AddVertex(glm::vec3(0.75, 0.0, 1.0));
	AddVertex(glm::vec3(3.0, 0.0, 1.0));
	AddVertex(glm::vec3(3.0, 0.75, 1.0));
	AddVertex(glm::vec3(0.75, 0.75, 1.0));

	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));

	//Front
	AddTriangleVertex(0, 4, 0);
	AddTriangleVertex(1, 4, 1);
	AddTriangleVertex(2, 4, 2);
	AddTriangleVertex(0, 4, 0);
	AddTriangleVertex(3, 4, 2);
	AddTriangleVertex(2, 4, 3);

	AddTriangleVertex(3, 4, 0);
	AddTriangleVertex(6, 4, 1);
	AddTriangleVertex(5, 4, 2);
	AddTriangleVertex(3, 4, 0);
	AddTriangleVertex(4, 4, 2);
	AddTriangleVertex(5, 4, 3);

	//Back
	AddTriangleVertex(7, 5, 0);
	AddTriangleVertex(8, 5, 1);
	AddTriangleVertex(9, 5, 2);
	AddTriangleVertex(7, 5, 0);
	AddTriangleVertex(10, 5, 2);
	AddTriangleVertex(9, 5, 3);

	AddTriangleVertex(10, 5, 0);
	AddTriangleVertex(13, 5, 1);
	AddTriangleVertex(12, 5, 2);
	AddTriangleVertex(10, 5, 0);
	AddTriangleVertex(11, 5, 2);
	AddTriangleVertex(12, 5, 3);

	//Left
	AddTriangleVertex(0, 1, 0);
	AddTriangleVertex(7, 1, 1);
	AddTriangleVertex(8, 1, 2);
	AddTriangleVertex(0, 1, 0);
	AddTriangleVertex(1, 1, 2);
	AddTriangleVertex(8, 1, 3);

	//Top
	AddTriangleVertex(1, 2, 0);
	AddTriangleVertex(8, 2, 1);
	AddTriangleVertex(9, 2, 2);
	AddTriangleVertex(1, 2, 0);
	AddTriangleVertex(2, 2, 2);
	AddTriangleVertex(9, 2, 3);

	//Right 
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(9, 0, 1);
	AddTriangleVertex(13, 0, 2);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(6, 0, 2);
	AddTriangleVertex(13, 0, 3);

	//Right Bottom
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(13, 2, 1);
	AddTriangleVertex(12, 2, 2);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(5, 2, 2);
	AddTriangleVertex(12, 2, 3);

	//Far Right
	AddTriangleVertex(4, 0, 0);
	AddTriangleVertex(5, 0, 1);
	AddTriangleVertex(12, 0, 2);
	AddTriangleVertex(4, 0, 0);
	AddTriangleVertex(11, 0, 2);
	AddTriangleVertex(12, 0, 3);

	//Bottom
	AddTriangleVertex(0, 3, 0);
	AddTriangleVertex(7, 3, 1);
	AddTriangleVertex(11, 3, 2);
	AddTriangleVertex(0, 3, 0);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(11, 3, 3);
}

void CMesh::CreateCone(double radius,double height)
{
	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));

	//Bottom Circle
	int j = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(0., 0., 0.));
			j--;
		}
		else
		{
			double angRad = (2 * j * PI) / maxAngle;
			AddVertex(glm::vec3(cos(angRad)*radius, sin(angRad)*radius, height));
			j++;
		}
	}

	for (int i = 0; i < maxAngle * 3; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	
}

void CMesh::CreateStool()
{
	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));

	//Bottom Circle
	int j = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(0., 0., 0.));
			j--;
		}
		else
		{
			double angRad = (2 * j * PI) / maxAngle;
			AddVertex(glm::vec3(cos(angRad), sin(angRad), 0.));
			j++;
		}
	}

	for (int i = 0; i < maxAngle * 3; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Top Circle
	int k = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(0., 0., 1.));
			k--;
		}
		else
		{
			double angRad = (2 * k * PI) / maxAngle;
			AddVertex(glm::vec3(cos(angRad), sin(angRad), 1.));
			k++;
		}
	}

	for (int i = (maxAngle * 3); i < maxAngle * 6; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Side Cylinder Wall bottom up
	int sW = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		double angRad = (2 * sW * PI) / maxAngle;
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(cos(angRad),sin(angRad), 0.));
			sW--;
		}
		else
		{
			AddVertex(glm::vec3(cos(angRad), sin(angRad), 1.));
			sW++;
		}
	}

	for (int i = (maxAngle * 6); i < maxAngle * 9; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}

	//Side Cylinder Wall Top Down
	int sW2 = 0;
	for (int i = 0; i < maxAngle * 3; i++)
	{
		double angRad = (2 * sW2 * PI) / maxAngle;
		if (i % 3 == 0)
		{
			AddVertex(glm::vec3(cos(angRad),sin(angRad), 1.));
			sW2--;
		}
		else
		{
			AddVertex(glm::vec3(cos(angRad), sin(angRad), 0.));
			sW2++;
		}
	}

	for (int i = (maxAngle * 9); i < maxAngle * 12; i++)
	{
		AddTriangleVertex(i, 4, 0);
	}
}
