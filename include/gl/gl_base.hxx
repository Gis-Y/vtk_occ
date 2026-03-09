/*HEAD gl_base.hxx COMX_COMPONENT_MODULE */
/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
	================================================================================ 
	File description:
	
	-- Please append file description informations here --
	  
	================================================================================
	Date            Name            Description of Change
	
  
	$HISTORY$
	================================================================================
*/

#ifndef _GL_BASE_H
#define _GL_BASE_H

#ifdef USE_QT_OPENGL_CONFIG
    #include <QOpenGLWidget>
#else
    #ifdef _WIN32
        #define APIENTRY __stdcall
        #define WINGDIAPI __declspec(dllimport)
        #define CALLBACK __stdcall
        #include <gl/gl.h>
        #include <gl/glu.h>
    #endif 
#endif //USE_QT_OPENGL_CONFIG

#include <gl/gl_mouse.hxx>

template<typename T>
class TGLPoint2D
{
public:
  TGLPoint2D(const T vx, const T vy) : x(vx), y(vy){}
public:
  T x, y;
public:
  T X(){return x;}
  T Y(){return y;}
  
  void X(const T vx){x = vx;}
  void Y(const T vy){y = vy;}
};

// Define opengl light.
class TGLLampHouse
{
public:
  virtual ~TGLLampHouse(){}
public:
  virtual void Activate() = 0;
  virtual void Closure() = 0;
  virtual void SetIdentity(GLenum identity) = 0;
};

class TGLSunLampHouse : public TGLLampHouse
{
private:
  GLfloat _ambient[4];
  GLfloat _diffuse[4];
  GLfloat _specular[4];
protected:
  GLfloat _position[4];
  GLenum	_identity;
public:
  inline TGLSunLampHouse(const GLfloat &x = 0.,
			 const GLfloat &y = 0.,
			 const GLfloat &z = 1.,
			 GLenum identity = GL_LIGHT0);
  inline virtual ~TGLSunLampHouse(){}
  
public:
  inline void Activate();
  inline void Closure();
  
public:
  inline void SetAmbient(const GLfloat &red,
			 const GLfloat &green,
			 const GLfloat &blue,
			 const GLfloat &alpha);
  inline void SetDiffuse(const GLfloat &red,
			 const GLfloat &green,
			 const GLfloat &blue,
			 const GLfloat &alpha);
  inline void SetSpecular(const GLfloat &red,
			  const GLfloat &green,
			  const GLfloat &blue,
			  const GLfloat &alpha);
  inline void SetPosition(const GLfloat &x,
			  const GLfloat &y,
			  const GLfloat &z);
  inline void SetIdentity(GLenum identity);
};

class TGLPointLampHouse : public TGLSunLampHouse
{
private:
  GLfloat _const_attenuation;
  GLfloat _linear_attenuation;
  GLfloat _quadratic_attenuation;
public:
  inline TGLPointLampHouse(const GLfloat &x = 0.,
			   const GLfloat &y = 0.,
			   const GLfloat &z = 1.,
			   GLenum identity = GL_LIGHT0);
  inline virtual ~TGLPointLampHouse(){}
public:
  inline void Activate();
  inline void Closure();
public:
  inline void SetConstAttenuation(const GLfloat &attenuation);
  inline void SetLinearAttenuation(const GLfloat &attenuation);
  inline void SetQuadraticAttenuation(const GLfloat &attenuation);
};

class TGLSpotLampHouse : public TGLPointLampHouse
{
private:
  GLfloat _direction[3];
  GLfloat _exponent;
  GLfloat _cutoff;
public:
  inline TGLSpotLampHouse(const GLfloat &x = 0.,
			  const GLfloat &y = 0.,
			  const GLfloat &z = 0.,
			  GLenum identity = GL_LIGHT0);
  inline virtual ~TGLSpotLampHouse(){}
public:
  inline void Activate();
  inline void Closure();
public:
  inline void SetDirection(const GLfloat x,const GLfloat &y,const GLfloat &z);
  inline void SetExponent(const GLfloat &exponent);
  inline void SetCutoff(const GLfloat &cutoff);
};

TGLSunLampHouse::TGLSunLampHouse(const GLfloat &x,
				 const GLfloat &y,
				 const GLfloat &z,
				 GLenum identity)
{
  _ambient[0] = 0.0;
  _ambient[1] = 0.0;
  _ambient[2] = 0.0;
  _ambient[3] = 1.0;
  
  _diffuse[0] = 1.0;
  _diffuse[1] = 1.0;
  _diffuse[2] = 1.0;
  _diffuse[3] = 1.0;
  
  _specular[0] = 1.0;
  _specular[1] = 1.0;
  _specular[2] = 1.0;
  _specular[3] = 1.0;
  
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
  _position[3] = 0.0;
  
  _identity = identity;
}

void TGLSunLampHouse::Activate()
{
  glLightfv(_identity,GL_AMBIENT,_ambient);
  glLightfv(_identity,GL_DIFFUSE,_diffuse);
  glLightfv(_identity,GL_SPECULAR,_specular);
  glLightfv(_identity,GL_POSITION,_position);
  
  glLightf(_identity,GL_CONSTANT_ATTENUATION,1.0);
  glLightf(_identity,GL_LINEAR_ATTENUATION,0.0);
  glLightf(_identity,GL_QUADRATIC_ATTENUATION,0.0);
  
  GLfloat spot_direction[] = {0.0,0.0,-1.0};
  glLightfv(_identity,GL_SPOT_DIRECTION,spot_direction);
  glLightf(_identity,GL_SPOT_EXPONENT,0.0);
  glLightf(_identity,GL_SPOT_CUTOFF,180.0);
  
  glEnable(_identity);
}

void TGLSunLampHouse::Closure()
{
  glDisable(_identity);
}

void TGLSunLampHouse::SetAmbient(const GLfloat &red,
				 const GLfloat &green,
				 const GLfloat &blue,
				 const GLfloat &alpha)
{
  _ambient[0] = red;
  _ambient[1] = green;
  _ambient[2] = blue;
  _ambient[3] = alpha;
}
void TGLSunLampHouse::SetDiffuse(const GLfloat &red,
				 const GLfloat &green,
				 const GLfloat &blue,
				 const GLfloat &alpha)
{
  _diffuse[0] = red;
  _diffuse[1] = green;
  _diffuse[2] = blue;
  _diffuse[3] = alpha;
}
void TGLSunLampHouse::SetSpecular(const GLfloat &red,
				  const GLfloat &green,
				  const GLfloat &blue,
				  const GLfloat &alpha)
{
  _specular[0] = red;
  _specular[1] = green;
  _specular[2] = blue;
  _specular[3] = alpha;
}
void TGLSunLampHouse::SetPosition(const GLfloat &x,
				  const GLfloat &y,
				  const GLfloat &z)
{
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
}
void TGLSunLampHouse::SetIdentity(GLenum identity)
{
  _identity = identity;
}

//////////////////////////////////////////////////////////////////////////

TGLPointLampHouse::TGLPointLampHouse(const GLfloat &x,
				     const GLfloat &y,
				     const GLfloat &z,
				     GLenum identity)
  :TGLSunLampHouse(x,y,z,identity)
{
  _const_attenuation = 1.0;
  _linear_attenuation = 0.0;
  _quadratic_attenuation = 0.0;
  
  _position[3] = 1.0;
}

void TGLPointLampHouse::Activate()
{
  TGLSunLampHouse::Activate();
  
  glLightf(_identity,GL_CONSTANT_ATTENUATION,_const_attenuation);
  glLightf(_identity,GL_LINEAR_ATTENUATION,_linear_attenuation);
  glLightf(_identity,GL_QUADRATIC_ATTENUATION,_quadratic_attenuation);
}

void TGLPointLampHouse::Closure()
{
  TGLSunLampHouse::Closure();
}

void TGLPointLampHouse::SetConstAttenuation(const GLfloat &attenuation)
{
  _const_attenuation = attenuation;
}

void TGLPointLampHouse::SetLinearAttenuation(const GLfloat &attenuation)
{
  _linear_attenuation = attenuation;
}

void TGLPointLampHouse::SetQuadraticAttenuation(const GLfloat &attenuation)
{
  _quadratic_attenuation = attenuation;
}


//////////////////////////////////////////////////////////////////////////

TGLSpotLampHouse::TGLSpotLampHouse(const GLfloat &x,
				   const GLfloat &y,
				   const GLfloat &z,
				   GLenum identity)
  :TGLPointLampHouse(x,y,z,identity)
{
  _direction[0] = 0.0;
  _direction[1] = 0.0;
  _direction[2] = -1.0;
  
  _exponent = 0.0;
  _cutoff = 22.5;
}

void TGLSpotLampHouse::Activate()
{
  TGLPointLampHouse::Activate();

  glLightfv(_identity,GL_SPOT_DIRECTION,_direction);
  glLightf(_identity,GL_SPOT_EXPONENT,_exponent);
  glLightf(_identity,GL_SPOT_CUTOFF,_cutoff);
}

void TGLSpotLampHouse::Closure()
{
  TGLPointLampHouse::Closure();
}

void TGLSpotLampHouse::SetDirection(const GLfloat x,const GLfloat &y,const GLfloat &z)
{
  _direction[0] = x;
  _direction[1] = y;
  _direction[2] = z;
}
void TGLSpotLampHouse::SetExponent(const GLfloat &exponent)
{
  _exponent = exponent;
}
void TGLSpotLampHouse::SetCutoff(const GLfloat &cutoff)
{
  _cutoff = cutoff;
}

#endif
