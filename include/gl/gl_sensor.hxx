/*HEAD gl_sensor.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_SENSOR_H_20073511538
#define _KMAS_COMX_GL_SENSOR_H_20073511538

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

  enum TSensorType{
    PAN_SENSOR,
    ROTATE_SENSOR,
    SCALE_SENSOR,
    RECT_SCALE_SENSOR,
    RESET_SENSOR,
    YOX_SENSOR,
    XOY_SENSOR,
    ZOY_SENSOR,
    YOZ_SENSOR,
    XOZ_SENSOR,
    ZOX_SENSOR
  };

  // {F331213E-133F-4270-88A6-73DACE9B6ABE}
  const MUID IID_IGlSensorToolkit = 
    { 0xf331213e, 0x133f, 0x4270, { 0x88, 0xa6, 0x73, 0xda, 0xce, 0x9b, 0x6a, 0xbe } };
  
  comx_interface IGlSensorToolkit: public IRoot{
    virtual TStatus STDCALL Activate(const TSensorType &type) = 0;
  };

  // {CAF76FF7-5F61-4902-8062-86058FB69882}
  const MUID IID_IGlSensorContent = 
    { 0xcaf76ff7, 0x5f61, 0x4902, { 0x80, 0x62, 0x86, 0x5, 0x8f, 0xb6, 0x98, 0x82 } };

  comx_interface IGlSensorContent : public IRoot{
    virtual TStatus STDCALL GetProjectionBody(double &left,double &right,double &bottom,double &top,double &near,double &far) = 0;
    virtual TStatus STDCALL MultiScaleFactor(const double &scale) = 0;
    virtual TStatus STDCALL GetScaleFactor(double &scale) = 0;
    virtual TStatus STDCALL GetWindow(int &width,int &height) = 0;
    virtual TStatus STDCALL SendComxMessage(const char *msg) = 0;
    virtual TStatus STDCALL GetZBuffer(float *&buf) = 0;
  };

  // {D315DB80-4996-4b6c-A53A-8EB1D2DCEF66}
  const MUID IID_IGlSensorLCS = 
    { 0xd315db80, 0x4996, 0x4b6c, { 0xa5, 0x3a, 0x8e, 0xb1, 0xd2, 0xdc, 0xef, 0x66 } };

  comx_interface IGlSensorLCS : public IRoot
  {
    virtual TStatus STDCALL GetSensorLCS(double matrix[3][3]) = 0;
  };

  // {4F61FE42-81A1-4944-999F-8D1363F80D74}
  const MUID IID_IGlSensorToolkit2 = 
    { 0x4f61fe42, 0x81a1, 0x4944, { 0x99, 0x9f, 0x8d, 0x13, 0x63, 0xf8, 0xd, 0x74 } };

  comx_interface IGlSensorToolkit2 : public IGlSensorToolkit
  {
    virtual TStatus STDCALL EnableLCSDisplay(bool flag = true) = 0;
  };

  // {4E085CB1-DD20-4733-B431-FF772BAD9B69}
  const MUID IID_IGlSensorToolkit3 = 
  { 0x4e085cb1, 0xdd20, 0x4733, { 0xb4, 0x31, 0xff, 0x77, 0x2b, 0xad, 0x9b, 0x69 } };
  
  comx_interface IGlSensorToolkit3 : public IGlSensorToolkit2
  {
	  virtual TStatus STDCALL GetCurrentSensorType(TSensorType &type) = 0;
  };


  // {CE130AC0-8A41-42EB-AD4E-EE68D2A8AA4A}
  const MUID IID_IGlSensorToolkit4 = 
  { 0xce130ac0, 0x8a41, 0x42eb, { 0xad, 0x4e, 0xee, 0x68, 0xd2, 0xa8, 0xaa, 0x4a } };

  comx_interface IGlSensorToolkit4 : public IGlSensorToolkit3
  {
	  virtual TStatus STDCALL SetOpenGLMatrix(double *matrix) = 0;
	  virtual TStatus STDCALL GetOpenGLMatrix(double *&matrix) = 0;
          virtual TStatus STDCALL SetRecordNumber(const int cnt) = 0;
  };

  // {2220DE99-F1D7-45ab-B6FE-E98460E39A8C}
  const MUID CLSID_IGlSensor =
    { 0x2220de99, 0xf1d7, 0x45ab, { 0xb6, 0xfe, 0xe9, 0x84, 0x60, 0xe3, 0x9a, 0x8c } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
