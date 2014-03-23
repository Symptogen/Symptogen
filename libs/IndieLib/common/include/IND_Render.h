/*****************************************************************************************
 * File: IND_Render.h
 * Desc: Render class, abstraction layer for the rendering subsystem used
 *****************************************************************************************/

/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/


#ifndef _IND_RENDER_H_
#define _IND_RENDER_H_

// ----- Includes -----

#include "IndiePlatforms.h"
#include "Defines.h"
#include "IND_Timer.h"
#include "IND_Window.h"
#include "IND_AnimationManager.h"

// ----- Forward Declarations -----

#ifdef INDIERENDER_DIRECTX
class DirectXRender;
#endif

#ifdef INDIERENDER_GLES_IOS
class OpenGLES2Render;
#endif

#ifdef INDIERENDER_OPENGL
#ifdef PLATORM_LINUX
#endif
class OpenGLRender;
#endif

class IND_FontManager;
class IND_SurfaceManager;
class IND_Surface;
class IND_3dMeshManager;
class IND_AnimationManager;
class IND_FontManager;
class IND_Timer;
class IND_Font;
class IND_3dMesh;
class IND_Animation;
class IND_Camera2d;
class IND_Camera3d;

// ----- Defines -----

#define MAX_PIXELS 2048


// --------------------------------------------------------------------------------
//									  IND_Render
// --------------------------------------------------------------------------------

/**
@defgroup IND_Render IND_Render
@ingroup Main
Render class for setting up the render area, click in ::IND_Render to see all the methods of this class.
*/
/**@{*/

/**
The methods of this class are used for preparing the render area, viewport, etc.
It wraps the actual rendering code used (DirectX, OpenGL...)
*/
class LIB_EXP IND_Render {
public:

	// ----- Init/End -----

	IND_Render():
		_wrappedRenderer(NULL)
	{}
	~IND_Render()              {
		end();
	}

	// ----- Public methods -----

	/** @name Renderer and window initialization and resetting
	*
	*/
	/**@{*/
	IND_Window* initRenderAndWindow(IND_WindowProperties& props);
	bool    reset(IND_WindowProperties& props);
	bool    toggleFullScreen();
	/**@}*/
	
    /** @name Object lifecycle
    *
    */
    /**@{*/
	void    end();
	bool    isOK();
     /**@}*/

    /** @name Rendering frame lifecycle
     *
     */
    /**@{*/
	void beginScene();
	void endScene();
    /**@}*/
    
    /** @name Render settings
     *
     */
    /**@{*/
	inline void showFpsInWindowTitle();

    inline bool setAntialiasing(bool pSwitch);
    
    void setPointPixelScale (float pNewScale);
    
    /**@}*/
    
	// ----- Viewports and cameras -----

	/** @name Viewport clearing (both for 2d and 3d viewports)
	*
	*/
	/**@{*/
	void clearViewPort(unsigned char pR,
	                   unsigned char pG,
	                   unsigned char pB);
	/**@}*/

	/** @name Viewport and camera 2d
	*
	*/
	/**@{*/
	bool setViewPort2d(int pX,
	                   int pY,
	                   int pWidth,
	                   int pHeight);

	void setCamera2d(IND_Camera2d *pCamera2d);
	/**@}*/

	/** @name Viewport and camera 3d
	*
	*/
	/**@{*/
	inline bool setViewPort3d(int pX,
	                          int pY,
	                          int pWidth,
	                          int pHeight);

	inline void setCamera3d(IND_Camera3d *pCamera3d);
	/**@}*/

	// ----- Render Primitive 2d -----

	/** @name Bliting primitives (you can also use IND_Entity2d instead this methods)
	*
	*/
	/**@{*/
	void blitPixel(int pX,
	               int pY,
	               unsigned char pR,
	               unsigned char pG,
	               unsigned char pB,
	               unsigned char pA);

	void blitLine(int pX1,
	              int pY1,
	              int pX2,
	              int pY2,
	              unsigned char pR,
	              unsigned char pG,
	              unsigned char pB,
	              unsigned char pA);

	void blitRectangle(int pX1,
	                   int pY1,
	                   int pX2,
	                   int pY2,
	                   unsigned char pR,
	                   unsigned char pG,
	                   unsigned char pB,
	                   unsigned char pA);

	void blitFillRectangle(int pX1,
	                       int pY1,
	                       int pX2,
	                       int pY2,
	                       unsigned char pR,
	                       unsigned char pG,
	                       unsigned char pB,
	                       unsigned char pA);

	void blitColoredTriangle(int pX1,
	                         int pY1,
	                         int pX2,
	                         int pY2,
	                         int pX3,
	                         int pY3,
	                         unsigned char pR1, unsigned char pG1, unsigned char pB1,
	                         unsigned char pR2, unsigned char pG2, unsigned char pB2,
	                         unsigned char pR3, unsigned char pG3, unsigned char pB3,
	                         unsigned char pA);

	void blitTriangleList(IND_Point *pTrianglePoints,
	                      int pNumPoints,
	                      unsigned char pR,
	                      unsigned char pG,
	                      unsigned char pB,
	                      unsigned char pA);

	bool blitPoly2d(IND_Point *pPixel,
	                int pNumLines,
	                unsigned char pR,
	                unsigned char pG,
	                unsigned char pB,
	                unsigned char pA);

	bool blitRegularPoly(int pX,
	                     int pY,
	                     int pRadius,
	                     int pN,
	                     float pAngle,
	                     unsigned char pR,
	                     unsigned char pG,
	                     unsigned char pB,
	                     unsigned char pA);

	/**@}*/


	// ----- Advanced transformations -----

	/** @name Advanced transformations methods (it's preferible to use IND_Entity2d and IND_Entity3d instead)
	*
	*/
	/**@{*/
	void setTransform2d(int pX,
	                    int  pY,
	                    float pAngleX,
	                    float pAngleY,
	                    float pAngleZ,
	                    float pScaleX,
	                    float pScaleY,
	                    int pAxisCalX,
	                    int pAxisCalY,
	                    bool pMirrorX,
	                    bool pMirrorY,
	                    int pWidth,
	                    int pHeight,
	                    IND_Matrix *pMatrix);

	void setTransform2d(IND_Matrix &pTransformMatrix);
	
	void setIdentityTransform2d ();

	inline void setTransform3d(float pX,
	                           float  pY,
	                           float  pZ,
	                           float pAngleX,
	                           float pAngleY,
	                           float pAngleZ,
	                           float pScaleX,
	                           float pScaleY,
	                           float pScaleZ,
	                           IND_Matrix *pMatrix);

	void setRainbow2d(IND_Type pType,
	                  bool pCull,
	                  bool pMirrorX,
	                  bool pMirrorY,
	                  IND_Filter pFilter,
	                  unsigned char pR,
	                  unsigned char pG,
	                  unsigned char pB,
	                  unsigned char pA,
	                  unsigned char pFadeR,
	                  unsigned char pFadeG,
	                  unsigned char pFadeB,
	                  unsigned char pFadeA,
	                  IND_BlendingType pSo,
	                  IND_BlendingType pDs);

	inline void setRainbow3d(bool pCull,
	                         bool pFlipNormals,
	                         IND_Filter pFilter,
	                         unsigned char pR,
	                         unsigned char pG,
	                         unsigned char pB,
	                         unsigned char pA,
	                         unsigned char pFadeR,
	                         unsigned char pFadeG,
	                         unsigned char pFadeB,
	                         unsigned char pFadeA,
	                         IND_BlendingType pSo,
	                         IND_BlendingType pDs);

	inline void perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane);

	inline void perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane);
	/**@}*/


	// ----- Render Object 2d -----

	/** @name Advanced 2d bliting methods (it's preferible to use IND_Entity2d together with IND_Entity2dManager instead)
	*
	*/
	/**@{*/
	void blitSurface(IND_Surface *pSu);

	void blitGrid(IND_Surface *pSu, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);

	void blitRegionSurface(IND_Surface *pSu,
	                              int pX,
	                              int pY,
	                              int pWidth,
	                              int pHeight);

	bool blitWrapSurface(IND_Surface *pSu,
	                     int pWidth,
	                     int pHeight,
	                     float pUOffset,
	                     float pVOffset);

	int blitAnimation(IND_Animation *pAn,
	                  unsigned int pSequence,
	                  int pX,
			  int pY,
	                  int pWidth,
			  int pHeight,
	                  bool pToggleWrap,
	                  float pUOffset,
	                  float pVOffset);

	// ------ Render Text 2d -----

	void blitText(IND_Font *pFo,
	              char *pText,
	              int pX,
	              int pY,
	              int pOffset,
	              int pLineSpacing,
	              float pScaleX,  //MIGUEL: New param
	              float pScaleY,  //MIGUEL: New param
	              unsigned char pR,
	              unsigned char pG,
	              unsigned char pB,
	              unsigned char pA,
	              unsigned char pFadeR,
	              unsigned char pFadeG,
	              unsigned char pFadeB,
	              unsigned char pFadeA,
	              IND_Filter pLinearFilter,
	              IND_BlendingType pSo,
	              IND_BlendingType pDs,
	              IND_Align pAlign);

	/**@}*/


	// ----- Render Object 3d -----

	/** @name Advanced 3d bliting methods (it's preferible to use IND_Entity3d together with  IND_Entity3dManager instead)
	*
	*/
	/**@{*/
	inline void blit3dMesh(IND_3dMesh *p3dMesh);
	inline void set3dMeshSequence(IND_3dMesh *p3dMesh, unsigned int pIndex);
	/**@}*/

	// ----- Attributes -----

	/** @name Gets
	*
	*/
	/**@{*/
	//! This function returns the x position of the actual viewport
	int getViewPortX();
	//! This function returns the y position of the actual viewport
	int getViewPortY();
	//! This function returns the width position of the actual viewport
	int getViewPortWidth();
	//! This function returns the width position of the actual viewport
	int getViewPortHeight();
	//! This function returns the actual version of Direct3d that is using IndieLib.
	char *getVersion();
	//! This function returns the name of the graphic card vendor.
	char *getVendor();
	//! This function returns the name of the graphic card.
	char *getRenderer();
	//! This function returns the maximum texture size allowed by the graphic card.
	int  getMaxTextureSize();

	//! This function returns the actual fps (frames per second) in a string of chars.
	//! @param[in,out] pBuffer buffer capable to hold string representation of integer FPS value
	void getFpsString(char *pBuffer);

	//! This function returns the actual fps (frames per second) in an int
	int getFpsInt()      {
		return _lastFps;
	}
	//! This function returns a pointer to the IND_Window object where the render has been created
	IND_Window *getWindow();
	//! This function returns in miliseconds the time that took the previous frame.
	/**
	It is very useful to indicate transformations along the timeline.

	For example, making Position_X += Speed * Render.getFrameTime() / 1000.0f we would get
	one x position that would go moving along the time with a given speed.
	*/
	float getFrameTime()      {
		return _last;
	}
	/**@}*/

	//! This function returns the number of renderered objects in one frame
	//! @param[in,out] pBuffer buffer capable to hold string representation of integer. Recommended size is 15
	void getNumrenderedObjectsString(char* pBuffer);

	//! This function returns the number of discarded objects in one frame
	//! @param[in,out] pBuffer buffer capable to hold string representation of integer. Recommended size is 15
	void getNumDiscardedObjectsString(char* pBuffer);

	//! This function returns the number of renderered objects in one frame (integer)
	//! @return The number of objects
	int getNumrenderedObjectsInt();

	//! This function returns the number of discarded objects in one frame (integer)
	//! @return The number of objects
	int getNumDiscardedObjectsInt();

	//! Resets the counters for rendered objects
	void resetNumrenderedObject();

	//! Resets the counters for discarded objects
	void resetNumDiscardedObjects();

private:
    /** @cond DOCUMENT_PRIVATEAPI */

	// ----- Objects -----

#ifdef INDIERENDER_DIRECTX
	DirectXRender *_wrappedRenderer;
#endif

#ifdef INDIERENDER_GLES_IOS
	OpenGLES2Render *_wrappedRenderer;
#endif

#ifdef INDIERENDER_OPENGL
	OpenGLRender *_wrappedRenderer;
#endif
	
    // ----- Vars -----
	
    // Timer
	IND_Timer _timer;
	float _lastTime;
	float _last;


	// Fps
	int _fpsCounter;
	float _currentTimeFps;
	float _lastTimeFps;
	int _lastFps;

	// ----- Private methods -----

	IND_Window* createRender(IND_WindowProperties& windowProperties);
	void resetTimer();

	// ----- Private Interface (for friend classes) -----

	void reCalculateFrustrumPlanes();
	void blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pWorldMatrix);
	void blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix);

	// ----- Friends -----

	friend class IND_Entity2dManager;
	friend class IND_Input;
	friend class DirectXTextureBuilder;
    
    /** @endcond */
};
/**@}*/

#endif // _IND_RENDER_H_
