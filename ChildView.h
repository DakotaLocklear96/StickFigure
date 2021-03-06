// ChildView.h : interface of the CChildView class
//


#pragma once
#include "ShaderWnd/ShaderWnd.h"
#include "Mesh.h"
#include "Sphere.h"
#include "Torus.h"
#include "graphics\GrTexture.h"
#include "graphics\GrCubeTexture.h"

// CChildView window

class CChildView : public CShaderWnd
{
	// Construction
public:
	CChildView();

	// Attributes
public:
	CGrTexture m_catTex;
	CGrTexture m_sphereTex;
	CGrTexture m_torusTex;
	CGrTexture m_letterTex;
	CGrTexture m_cylinderTex;
	CGrTexture m_coneTex;
	CGrCubeTexture m_cubeTex;
	CMesh m_cat;
	CSphere m_sphere;
	CSphere m_metallicSphere;
	CMesh m_skybox;
	CMesh m_letter;
	CMesh m_legCylinder;
	CTorus m_torus;
	CMesh m_cylinder;
	CMesh m_bodyCylinder;
	CMesh m_cone;

	// Operations
public:
	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void RenderGL();
	virtual void InitGL();
	virtual void CleanGL();
	UINT_PTR    m_timer;

	// Implementation
public:
	virtual ~CChildView();

private:
	glm::lowp_float m_armRotation = 90.;
	glm::lowp_float m_headRotation = 90.;
	glm::lowp_float m_hipRotation = 0.;
	bool m_armsUp = false;
	bool m_armsDown = false;
	bool m_headRotate = false;
	bool m_hipRotate = false;
	bool m_add = true;
	bool m_addHip = true;
	bool m_allInOne = false;


	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAnimationArmsrotate();
	afx_msg void OnAnimationArmsdown();
	afx_msg void OnAnimationHeadrotate();
	afx_msg void OnAnimationAllinoneaimation();
	afx_msg void OnAnimationHipjoint();
};