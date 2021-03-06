// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Step4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_ANIMATION_ARMSROTATE, &CChildView::OnAnimationArmsrotate)
	ON_COMMAND(ID_ANIMATION_ARMSDOWN, &CChildView::OnAnimationArmsdown)
	ON_COMMAND(ID_ANIMATION_HEADROTATE, &CChildView::OnAnimationHeadrotate)
	ON_COMMAND(ID_ANIMATION_ALLINONEAIMATION, &CChildView::OnAnimationAllinoneaimation)
	ON_COMMAND(ID_ANIMATION_HIPJOINT, &CChildView::OnAnimationHipjoint)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::InitGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//Legs/Arms
	m_cylinderTex.LoadFile(L"textures/bumpmap.jpg");
	m_legCylinder.CreateCylinder(0.5,4);
	m_legCylinder.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_legCylinder.InitGL();

	//Body
	m_bodyCylinder.CreateCylinder(1.5, 6);
	m_bodyCylinder.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_bodyCylinder.InitGL();

	//Head
	m_sphereTex.LoadFile(L"textures/bumpmap.jpg");
	m_sphere.SetRadius(2);
	m_sphere.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_sphere.InitGL();

	//Hat
	m_coneTex.LoadFile(L"textures/bumpmap.jpg");
	m_cone.CreateCone(4,2);
	m_cone.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_cone.InitGL();

}

void CChildView::RenderGL()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Head
	m_program = m_sphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(1., 5.5, 0.))
		* rotate(mat4(1.f), m_headRotation, vec3(0., 1., 0.));
	glm::mat4 VM = m_mVM * M;
	glm::mat4 PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sphereTex.TexName());

	m_sphere.RenderGL();

	//Left Leg
	m_program = m_legCylinder.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(0., 0., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.))
		* rotate(mat4(1.f), -30.f, vec3(0., 1., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cylinderTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_legCylinder.RenderGL();

	//Right Leg
	m_program = m_legCylinder.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	M = translate(mat4(1.f), vec3(2., 0., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.))
		* rotate(mat4(1.f), 30.f, vec3(0., 1., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cylinderTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_legCylinder.RenderGL();

	//Left Arm
	m_program = m_legCylinder.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	M = translate(mat4(1.f), vec3(0., 3., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.))
		* rotate(mat4(1.f), -1 * (m_armRotation), vec3(0., 1., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cylinderTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_legCylinder.RenderGL();

	//Right Arm
	m_program = m_legCylinder.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	M = translate(mat4(1.f), vec3(2., 3., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.))
		* rotate(mat4(1.f), m_armRotation, vec3(0., 1., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_cylinderTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_legCylinder.RenderGL();

	//Body
	m_program = m_bodyCylinder.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	M = translate(mat4(1.f), vec3(1., 5., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.))
		* rotate(mat4(1.f), m_hipRotation, vec3(0., 1., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cylinderTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_bodyCylinder.RenderGL();

	//Draw Cone
	m_program = m_cone.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	M = translate(mat4(1.f), vec3(1., 9., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
	//* rotate(mat4(1.f), 30.f, vec3(0., 0., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_coneTex.TexName());
	glDepthFunc(GL_LEQUAL);

	m_cone.RenderGL();

	if (m_armRotation < 120 && m_armsUp == true)
	{
		m_armRotation += 0.1;
		Invalidate();
	}

	if (m_armRotation > 60 && m_armsDown == true)
	{
		m_armRotation -= 0.1;
		Invalidate();
	}

	if (m_headRotate == true)
	{
		if (m_headRotation >= 120)
		{
			m_add = false;
		}

		if (m_headRotation <= 60)
		{
			m_add = true;
		}

		if (m_add == true)
		{
			m_headRotation++;
			Invalidate();
		}
		else
		{
			m_headRotation--;
			Invalidate();
		}
		
	}

	if (m_hipRotate == true)
	{
		if (m_hipRotation >= 3)
		{
			m_addHip = false;
		}

		if (m_hipRotation <= -3)
		{
			m_addHip = true;
		}

		if (m_addHip == true)
		{
			m_hipRotation+=0.5;
			Invalidate();
		}
		else
		{
			m_hipRotation-=0.5;
			Invalidate();
		}

	}
}

void CChildView::CleanGL()
{

}




void CChildView::OnAnimationArmsrotate()
{
	m_armsUp = true;
	m_armsDown = false;
	RenderGL();
}


void CChildView::OnAnimationArmsdown()
{
	m_armsDown = true;
	m_armsUp = false;
	RenderGL();
}


void CChildView::OnAnimationHeadrotate()
{
	if (m_headRotate == false)
	{
		m_headRotate = true;
		RenderGL();
	}
	else
	{
		m_headRotate = false;
	}
}


void CChildView::OnAnimationAllinoneaimation()
{
	OnAnimationHeadrotate();
	if (m_allInOne == false)
	{
		OnAnimationArmsrotate();
		m_allInOne = true;
	}
	else
	{	
		OnAnimationArmsdown();
		m_allInOne = false;
	}
}


void CChildView::OnAnimationHipjoint()
{
	if (m_hipRotate == false)
	{
		m_hipRotate = true;
		RenderGL();
	}
	else
	{
		m_hipRotate = false;
	}
}
