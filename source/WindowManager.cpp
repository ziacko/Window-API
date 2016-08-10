/**********************************************************************************************//**
 * @file	WindowAPI\WindowManager.cpp
 *
 * @brief	Implements the window manager class.
 **************************************************************************************************/

#include "WindowManager.h"
#include <cstring>

/**********************************************************************************************//**
 * @fn	WindowManager::WindowManager()
 *
 * @brief	Default constructor.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

WindowManager::WindowManager()
{
	//GetInstance()->Initialized = GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::Initialize()
 *
 * @brief	Initializes this object.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean WindowManager::Initialize()
{
	GetInstance()->Initialized = GL_FALSE;
#if defined(_WIN32) || defined(_WIN64)
	return Windows_Initialize();
#else
	return Linux_Initialize();
#endif
}

GLboolean WindowManager::IsInitialized()
{
	return GetInstance()->Initialized;
}

/**********************************************************************************************//**
 * @fn	WindowManager::~WindowManager()
 *
 * @brief	Destructor.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

WindowManager::~WindowManager()
{
	if (!GetInstance()->Windows.empty())
	{
#if defined(_MSC_VER)
		for each(auto CurrentWindow in GetInstance()->Windows)
#else
		for (auto CurrentWindow : GetInstance()->Windows)
#endif
		{
			delete CurrentWindow;
		}
		GetInstance()->Windows.clear();
	}
}

/**********************************************************************************************//**
 * @fn	FWindow* WindowManager::GetWindowByName(const char* WindowName)
 *
 * @brief	Gets window by name.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	null if it fails, else the window by name.
 **************************************************************************************************/

FWindow* WindowManager::GetWindowByName(const char* WindowName)
{
	if (DoesExist(WindowName))
	{
#if defined(_MSC_VER)
		for each(auto CurrentWindow in GetInstance()->Windows)
#else
		for (auto CurrentWindow : GetInstance()->Windows)
#endif
		{
			if (!strcmp(CurrentWindow->Name, WindowName))
			{
				return CurrentWindow;
			}
		}
		PrintErrorMessage(ERROR_WINDOWNOTFOUND);
		return nullptr;
	}
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	FWindow* WindowManager::GetWindowByIndex(GLuint WindowIndex)
 *
 * @brief	Gets window by index.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	null if it fails, else the window by index.
 **************************************************************************************************/

FWindow* WindowManager::GetWindowByIndex(GLuint WindowIndex)
{
	if (DoesExist(WindowIndex))
	{
#if defined(_MSC_VER)
		for each (auto CurrentWindow in GetInstance()->Windows)
#else
		for (auto CurrentWindow : GetInstance()->Windows)
#endif
		{
			if(CurrentWindow->ID == WindowIndex)
			{
				return CurrentWindow;
			}
		}

		PrintErrorMessage(ERROR_WINDOWNOTFOUND);
		return nullptr;
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	WindowManager* WindowManager::AddWindow(FWindow* NewWindow)
 *
 * @brief	Adds a window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	NewWindow	If non-null, the new window.
 *
 * @return	null if it fails, else a reference to the WindowManager.
 **************************************************************************************************/

WindowManager* WindowManager::AddWindow(FWindow* NewWindow)
{
	if (GetInstance()->IsInitialized())
	{
		if (NewWindow != nullptr)
		{
			GetInstance()->Windows.push_back(NewWindow);
			NewWindow->ID = GetInstance()->Windows.size() - 1;
			NewWindow->Initialize();
			return GetInstance();
		}
		PrintErrorMessage(ERROR_INVALIDWINDOW);
		return nullptr;
	}
	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	WindowManager* WindowManager::GetInstance()
 *
 * @brief	Gets the instance to the WindowManager.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the instance to the WindowManager.
 **************************************************************************************************/

WindowManager* WindowManager::GetInstance()
{
	if(!WindowManager::Instance)
	{
		WindowManager::Instance = new WindowManager();
		return WindowManager::Instance;
	}

	else 
	{		
		return WindowManager::Instance;
	}
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::DoesExist(const char* WindowName)
 *
 * @brief	Does the window exist.
 *
 * @author	Ziyad
 * @date	30/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	whether the window is in the window manager.
 **************************************************************************************************/

GLboolean WindowManager::DoesExist(const char* WindowName)
{
	if (GetInstance()->IsInitialized())
	{
		if (IsValidString(WindowName))
		{
#if defined(_MSC_VER)
			for each(auto iter in GetInstance()->Windows)
#else
			for (auto iter : GetInstance()->Windows)
#endif
			{
				if (iter->Name == WindowName)
				{
					return GL_TRUE;
				}
			}
		}
		PrintErrorMessage(ERROR_INVALIDWINDOWNAME);
		return GL_FALSE;
	}
	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::DoesExist(GLuint WindowIndex)
 *
 * @brief	Does the window exist.
 *
 * @author	Ziyad
 * @date	30/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	whether the window index given is lower then the current size of the windows container.
 **************************************************************************************************/

GLboolean WindowManager::DoesExist(GLuint WindowIndex)
{
	if (GetInstance()->IsInitialized())
	{
		if (WindowIndex <= (GetInstance()->Windows.size() - 1))
		{
			return FOUNDATION_OKAY;
		}

		PrintErrorMessage(ERROR_INVALIDWINDOWINDEX);
		return FOUNDATION_ERROR;
	}
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLuint WindowManager::GetNumWindows()
 *
 * @brief	Gets the number of windows.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	The number of windows in the manager.
 **************************************************************************************************/

GLuint WindowManager::GetNumWindows()
{
	if(GetInstance()->IsInitialized())
	{
		return GetInstance()->Windows.size();
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::ShutDown()
 *
 * @brief	Shuts down this object and frees any resources it is using.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

void WindowManager::ShutDown()
{	
#if defined(_MSC_VER)
	for each(auto CurrentWindow in GetInstance()->Windows)
	{
		delete CurrentWindow;
	}
#endif

#if defined(CURRENT_OS_LINUX)
	for (auto CurrentWindow : GetInstance()->Windows)
	{
		delete CurrentWindow;
	}
	XCloseDisplay(GetInstance()->m_Display);
#endif

	GetInstance()->Windows.clear();

	delete Instance;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetMousePositionInScreen(GLuint& X, GLuint& Y)
 *
 * @brief	Gets mouse position in screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	X	The X coordinate of the mouse relative to screen position.
 * @param [in,out]	Y	The Y coordinate of the mouse relative to screen position.
 **************************************************************************************************/

GLboolean WindowManager::GetMousePositionInScreen(GLuint& X, GLuint& Y)
{
	if (GetInstance()->IsInitialized())
	{
		X = GetInstance()->ScreenMousePosition[0];
		Y = GetInstance()->ScreenMousePosition[1];
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;

}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetMousePositionInScreen()
 *
 * @brief	Gets mouse position in screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the mouse position in screen.
 **************************************************************************************************/

GLuint* WindowManager::GetMousePositionInScreen()
{
	if (GetInstance()->IsInitialized())
	{
		return GetInstance()->ScreenMousePosition;
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetMousePositionInScreen(GLuint X, GLuint Y)
 *
 * @brief	Sets mouse position in screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	X	The new X position of the mouse cursor relative to screen coordinates.
 * @param	Y	The new Y position of the mouse cursor relative to screen coordinates.
 **************************************************************************************************/

GLboolean WindowManager::SetMousePositionInScreen(GLuint X, GLuint Y)
{
	GetInstance()->ScreenMousePosition[0] = X;
	GetInstance()->ScreenMousePosition[1] = Y;
#if defined(_WIN32) || defined(_WIN64)
	return Windows_SetMousePositionInScreen(X, Y);
#else
	return Linux_SetMousePositionInScreen(X, Y);
#endif
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetScreenResolution()
 *
 * @brief	Gets screen resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @return	null if it fails, else the screen resolution. ScreenResolution[0] will always the
 *  width of the screen and ScreenResolution[1] will always return the height of the screen.
 **************************************************************************************************/

GLuint* WindowManager::GetScreenResolution()
{
	if (GetInstance()->IsInitialized())
	{
#if defined(_WIN32) || defined(_WIN64)
		RECT l_Screen;
		HWND m_Desktop = GetDesktopWindow();
		GetWindowRect(m_Desktop, &l_Screen);

		GetInstance()->ScreenResolution[0] = l_Screen.right;
		GetInstance()->ScreenResolution[1] = l_Screen.bottom;
		return GetInstance()->ScreenResolution;
#endif

#if defined(__linux__)
		GetInstance()->ScreenResolution[0] = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
		GetInstance()->ScreenResolution[1] = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));

		return GetInstance()->ScreenResolution;
#endif
	}
	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return nullptr;

}

/**********************************************************************************************//**
 * @fn	void WindowManager::PollForEvents()
 *
 * @brief	Poll for events for all windows in the manager.
 *
 * @author	Ziyad
 * @date	29/11/2014
 **************************************************************************************************/

GLboolean WindowManager::PollForEvents()
{
	if (GetInstance()->IsInitialized())
	{
#if defined(_WIN32) || defined(_WIN64)
		return GetInstance()->Windows_PollForEvents();
#else
		return GetInstance()->Linux_PollForEvents();
#endif	
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

GLboolean WindowManager::WaitForEvents()
{
	if (GetInstance()->IsInitialized())
	{
#if defined(_WIN32) || defined(_WIN64)
		return GetInstance()->Windows_WaitForEvents();
#else
		return GetInstance()->Linux_WaitForEvents();
#endif
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetScreenResolution(GLuint& Width, GLuint& Height)
 *
 * @brief	Gets screen resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param [in,out]	Width 	The width.
 * @param [in,out]	Height	The height.
 **************************************************************************************************/

GLboolean WindowManager::GetScreenResolution(GLuint& Width, GLuint& Height)
{
	if (GetInstance()->IsInitialized())
	{
#if defined(_WIN32) || defined(_WIN64)

		RECT l_Screen;
		HWND m_Desktop = GetDesktopWindow();
		GetWindowRect(m_Desktop, &l_Screen);
		Width = l_Screen.right;
		Height = l_Screen.bottom;
#endif

#if defined(__linux__)

		Width = WidthOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));
		Height = HeightOfScreen(XDefaultScreenOfDisplay(GetInstance()->m_Display));


		GetInstance()->ScreenResolution[0] = Width;
		GetInstance()->ScreenResolution[1] = Height;
#endif

		return FOUNDATION_OKAY;
	}
	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetWindowResolution(const char* WindowName, GLuint& Width, GLuint& Height)
 *
 * @brief	Gets window resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	  	Name of the window.
 * @param [in,out]	Width 	The width.
 * @param [in,out]	Height	The height.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowResolution(const char* WindowName, GLuint& Width, GLuint& Height)
{
	if (GetInstance()->IsInitialized())
	{
		if (DoesExist(WindowName))
		{
			if (GetWindowByName(WindowName)->GetResolution(Width, Height))
			{
				return FOUNDATION_OKAY;
			}
			return FOUNDATION_ERROR;
		}
		return FOUNDATION_ERROR;
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetWindowResolution(GLuint WindowIndex, GLuint& Width, GLuint& Height)
 *
 * @brief	Gets window resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex   	Zero-based index of the window.
 * @param [in,out]	Width 	The width.
 * @param [in,out]	Height	The height.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowResolution(GLuint WindowIndex, GLuint& Width, GLuint& Height)
{
	if (DoesExist(WindowIndex))
	{
		GetWindowByIndex(WindowIndex)->GetResolution(Width, Height);
		return FOUNDATION_OKAY;
	}

	PrintErrorMessage(ERROR_NOTINITIALIZED);
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetWindowResolution(const char* WindowName)
 *
 * @brief	Gets window resolution as an array.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	null if it fails, else the window resolution. Resolution[0] will always return the
 *  width of the window and Resolution[1] will always return the height of the window.
 **************************************************************************************************/

GLuint* WindowManager::GetWindowResolution(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetResolution();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetWindowResolution(GLuint WindowIndex)
 *
 * @brief	Gets window resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	null if it fails, else the window resolution. Resolution[0] will always return the
 *  width of the window and Resolution[1] will always return the height of the window.
 **************************************************************************************************/

GLuint* WindowManager::GetWindowResolution(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetResolution();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowResolution(const char* WindowName, GLuint Width, GLuint Height)
 *
 * @brief	Sets window resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	Width	  	The width.
 * @param	Height	  	The height.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowResolution(const char* WindowName, GLuint Width, GLuint Height)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetResolution(Width, Height);
	}

	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowResolution(GLuint WindowIndex, GLuint Width, GLuint Height)
 *
 * @brief	Sets window resolution.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	Width	   	The width.
 * @param	Height	   	The height.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowResolution(GLuint WindowIndex, GLuint Width, GLuint Height)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetResolution(Width, Height);
	}

	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetWindowPosition(const char* WindowName, GLuint& X, GLuint& Y)
 *
 * @brief	Gets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param [in,out]	X 	The X coordinate of the window relative to screen coordinates.
 * @param [in,out]	Y 	The Y coordinate of the window relative to screen coordinates.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowPosition(const char* WindowName, GLuint& X, GLuint& Y)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetPosition(X, Y);
	}

	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetWindowPosition(GLuint WindowIndex, GLuint& X, GLuint& Y)
 *
 * @brief	Gets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param [in,out]	X  	The X coordinate of the window relative to screen coordinates.
 * @param [in,out]	Y  	The Y coordinate of the window relative to screen coordinates.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowPosition(GLuint WindowIndex, GLuint& X, GLuint& Y)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetPosition(X, Y);
	}

	return GL_FALSE;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetWindowPosition(const char* WindowName)
 *
 * @brief	Gets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	null if it fails, else the window position relative to screen coordinates.
 *  Position[0] will always return the X coordinate of the window and Position[1] will always
 *  return the Y coordinate of the window.
 **************************************************************************************************/

GLuint* WindowManager::GetWindowPosition(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetPosition();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetWindowPosition(GLuint WindowIndex)
 *
 * @brief	Gets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	null if it fails, else the window position relative to screen coordinates.
 *  Position[0] will always return the X coordinate of the window and Position[1] will always
 *  return the Y coordinate of the window.
 **************************************************************************************************/

GLuint* WindowManager::GetWindowPosition(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		return GetWindowByIndex(WindowIndex)->GetPosition();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowPosition(const char* WindowName, GLuint X, GLuint Y)
 *
 * @brief	Sets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	X		  	The new X coordinate of the window relative to screen coordinates.
 * @param	Y		  	The new Y coordinate of the window relative to screen coordinates.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowPosition(const char* WindowName, GLuint X, GLuint Y)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetPosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowPosition(GLuint WindowIndex, GLuint X, GLuint Y)
 *
 * @brief	Sets window position relative to screen coordinates.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	X		   	The new X coordinate of the window relative to screen coordinates.
 * @param	Y		   	The new y coordinate of the window relative to screen coordinates.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowPosition(GLuint WindowIndex, GLuint X, GLuint Y)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		return GetWindowByIndex(WindowIndex)->SetPosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetMousePositionInWindow(const char* WindowName, GLuint& X, GLuint& Y)
 *
 * @brief	Gets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param [in,out]	X 	The X coordinate of the mouse position relative to window coordinates.
 * @param [in,out]	Y 	The Y coordinate of the mouse position relative to window coordinates.
 **************************************************************************************************/

GLboolean WindowManager::GetMousePositionInWindow(const char* WindowName, GLuint& X, GLuint& Y)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetMousePosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::GetMousePositionInWindow(GLuint WindowIndex, GLuint& X, GLuint& Y)
 *
 * @brief	Gets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param [in,out]	X  	The X coordinate of the mouse position relative to window coordinates.
 * @param [in,out]	Y  	The Y coordinate of the mouse position relative to window coordinates.
 **************************************************************************************************/

GLboolean WindowManager::GetMousePositionInWindow(GLuint WindowIndex, GLuint& X, GLuint& Y)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetMousePosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetMousePositionInWindow(const char* WindowName)
 *
 * @brief	Gets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	null if it fails, else the mouse position in window. MousePosition[0] will always
 *  return the X coordinate of the mouse relative to screen coordinates and WindowPosition[1]
 *  will always return the Y coordinate of the mouse relative to screen coordinates.
 **************************************************************************************************/

GLuint* WindowManager::GetMousePositionInWindow(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetMousePosition();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	GLuint* WindowManager::GetMousePositionInWindow(GLuint WindowIndex)
 *
 * @brief	Gets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	null if it fails, else the mouse position in window. MousePosition[0] will always
 *  return the X coordinate of the mouse relative to screen coordinates and WindowPosition[1]
 *  will always return the Y coordinate of the mouse relative to screen coordinates.
 **************************************************************************************************/

GLuint* WindowManager::GetMousePositionInWindow(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetMousePosition();
	}
	PrintErrorMessage(ERROR_INVALIDWINDOWINDEX);
	return nullptr;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetMousePositionInWindow(const char* WindowName, GLuint X, GLuint Y)
 *
 * @brief	Sets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	X		  	The new X coordinate of the mouse position relative to window coordinates.
 * @param	Y		  	The new Y coordinate of the mouse position relative to window coordinates.
 **************************************************************************************************/

GLboolean WindowManager::SetMousePositionInWindow(const char* WindowName, GLuint X, GLuint Y)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetMousePosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetMousePositionInWindow(GLuint WindowIndex, GLuint X, GLuint Y)
 *
 * @brief	Sets mouse position in window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	X		   	The new X coordinate of the mouse position relative to window coordinates.
 * @param	Y		   	The new Y coordinate of the mouse position relative to window coordinates.
 **************************************************************************************************/

GLboolean WindowManager::SetMousePositionInWindow(GLuint WindowIndex, GLuint X, GLuint Y)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetMousePosition(X, Y);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::WindowGetKey(const char* WindowName, GLuint Key)
 *
 * @brief	get the state of the key relative to the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	Key		  	The key.
 *
 * @return	The state of the key.
 **************************************************************************************************/

GLboolean WindowManager::WindowGetKey(const char* WindowName, GLuint Key)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetKeyState(Key);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::WindowGetKey(GLuint WindowIndex, GLuint Key)
 *
 * @brief	get the state of the key relative to the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	Key		   	The key.
 *
 * @return	The state of the key.
 **************************************************************************************************/

GLboolean WindowManager::WindowGetKey(GLuint WindowIndex, GLuint Key)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetKeyState(Key);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowShouldClose(const char* WindowName)
 *
 * @brief	Gets whether the window should close.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	Whether the window should close.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowShouldClose(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetShouldClose();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowShouldClose(GLuint WindowIndex)
 *
 * @brief	Gets whether the window should close.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	Whether the window should close.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowShouldClose(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetShouldClose();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::WindowSwapBuffers(const char* WindowName)
 *
 * @brief	Swap DrawBuffers for that window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 **************************************************************************************************/

GLboolean WindowManager::WindowSwapBuffers(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SwapDrawBuffers();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::WindowSwapBuffers(GLuint WindowIndex)
 *
 * @brief	Swap DrawBuffers for that window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 **************************************************************************************************/

GLboolean WindowManager::WindowSwapBuffers(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SwapDrawBuffers();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsFullScreen(const char* WindowName)
 *
 * @brief	Gets whether the window is full screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	Whether the window is full screen.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsFullScreen(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsFullScreen();
	}
	
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsFullScreen(GLuint WindowIndex)
 *
 * @brief	Gets whether the window is full screen.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	Whether the window is full screen.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsFullScreen(GLuint WindowIndex)
{
	if(WindowIndex <= GetInstance()->Windows.size() -1)
	{
		return GetWindowByIndex(WindowIndex)->GetIsFullScreen();
	}
	
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetFullScreen(const char* WindowName, GLboolean ShouldBeFullscreen)
 *
 * @brief	toggle the fullscreen mode for the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName		  	Name of the window.
 * @param	ShouldBeFullscreen	whether the window should be in fullscreen mode.
 **************************************************************************************************/

GLboolean WindowManager::SetFullScreen(const char* WindowName, GLboolean ShouldBeFullscreen)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->FullScreen(ShouldBeFullscreen);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetFullScreen(GLuint WindowIndex, GLboolean ShouldBeFullscreen)
 *
 * @brief	toggle the fullscreen mode for the window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex		  	Zero-based index of the window.
 * @param	ShouldBeFullscreen	whether the window should be in fullscreen mode.
 **************************************************************************************************/

GLboolean WindowManager::SetFullScreen(GLuint WindowIndex, GLboolean ShouldBeFullscreen)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->FullScreen(ShouldBeFullscreen);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsMinimized(const char* WindowName)
 *
 * @brief	Gets window is minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	The window is minimized.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsMinimized(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsMinimized();
	}
	
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsMinimized(GLuint WindowIndex)
 *
 * @brief	Gets window is minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	The window is minimized.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsMinimized(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetIsMinimized();
	}
	
	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::MinimizeWindow(const char* WindowName, GLboolean ShouldBeMinimized)
 *
 * @brief	set the window to be minimized depending on NewState.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName		 	Name of the window.
 * @param	ShouldBeMinimized	whether the window should be minimized.
 **************************************************************************************************/

GLboolean WindowManager::MinimizeWindow(const char* WindowName, GLboolean ShouldBeMinimized)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->FullScreen(ShouldBeMinimized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::MinimizeWindow(GLuint WindowIndex, GLboolean ShouldBeMinimized)
 *
 * @brief	Minimize window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex		 	Zero-based index of the window.
 * @param	ShouldBeMinimized	Whether the window should be minimized.
 **************************************************************************************************/

GLboolean WindowManager::MinimizeWindow(GLuint WindowIndex, GLboolean ShouldBeMinimized)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->FullScreen(ShouldBeMinimized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsMaximized(const char* WindowName)
 *
 * @brief	Gets window is maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	The window is maximized.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsMaximized(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetIsMaximized();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsMaximized(GLuint WindowIndex)
 *
 * @brief	Gets window is maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	The window is maximized.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsMaximized(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetIsMaximized();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::MaximizeWindow(const char* WindowName, GLboolean ShouldBeMaximized)
 *
 * @brief	Maximize window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName		 	Name of the window.
 * @param	ShouldBeMaximized	Whether the window should be maximized.
 **************************************************************************************************/

GLboolean WindowManager::MaximizeWindow(const char* WindowName, GLboolean ShouldBeMaximized)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->FullScreen(ShouldBeMaximized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::MaximizeWindow(GLuint WindowIndex, GLboolean ShouldBeMaximized)
 *
 * @brief	Maximize window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex		 	Zero-based index of the window.
 * @param	ShouldBeMaximized	Whether the window should be maximized.
 **************************************************************************************************/

GLboolean WindowManager::MaximizeWindow(GLuint WindowIndex, GLboolean ShouldBeMaximized)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->FullScreen(ShouldBeMaximized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	const char* WindowManager::GetWindowName(GLuint WindowIndex)
 *
 * @brief	Gets window name.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	null if it fails, else the window name.
 **************************************************************************************************/

const char* WindowManager::GetWindowName(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetWindowName();
	}

	return nullptr;
}

/**********************************************************************************************//**
 * @fn	GLuint WindowManager::GetWindowIndex(const char* WindowName)
 *
 * @brief	Gets window index.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	The window index.
 **************************************************************************************************/

GLuint WindowManager::GetWindowIndex(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->ID;
	}

	return 0;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowTitleBar(const char* WindowName, const char* NewTitle)
 *
 * @brief	Sets window title bar.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	NewTitle  	The new title bar text.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowTitleBar(const char* WindowName, const char* NewTitle)
{
	if(DoesExist(WindowName) && IsValidString(NewTitle))
	{
		return GetWindowByName(WindowName)->SetTitleBar(NewTitle);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowTitleBar(GLuint WindowIndex, const char* NewTitle)
 *
 * @brief	Sets window title bar.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	NewTitle   	The new title bar text.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowTitleBar(GLuint WindowIndex, const char* NewTitle)
{
	if(DoesExist(WindowIndex) && IsValidString(NewTitle))
	{
		return GetWindowByIndex(WindowIndex)->SetTitleBar(NewTitle);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsInFocus(const char* WindowName)
 *
 * @brief	Gets window is in focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 *
 * @return	The window is in focus.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsInFocus(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->GetInFocus();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	GLboolean WindowManager::GetWindowIsInFocus(GLuint WindowIndex)
 *
 * @brief	Gets window is in focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 *
 * @return	The window is in focus.
 **************************************************************************************************/

GLboolean WindowManager::GetWindowIsInFocus(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->GetInFocus();
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::FocusWindow(const char* WindowName, GLboolean ShouldBeFocused)
 *
 * @brief	Focus window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	   	Name of the window.
 * @param	ShouldBeFocused	Whether the window should be in event focus.
 **************************************************************************************************/

GLboolean WindowManager::FocusWindow(const char* WindowName, GLboolean ShouldBeFocused)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->Focus(ShouldBeFocused);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::FocusWindow(GLuint WindowIndex, GLboolean ShouldBeFocused)
 *
 * @brief	Focus window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	   	Zero-based index of the window.
 * @param	ShouldBeFocused	Whether the window should be in event focus.
 **************************************************************************************************/

GLboolean WindowManager::FocusWindow(GLuint WindowIndex, GLboolean ShouldBeFocused)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->Focus(ShouldBeFocused);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::RestoreWindow(const char* WindowName)
 *
 * @brief	Restore window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 **************************************************************************************************/

GLboolean WindowManager::RestoreWindow(const char* WindowName)
{
	if(DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->Restore();
	}
	return FOUNDATION_ERROR;
	//implement window focusing
}

/**********************************************************************************************//**
 * @fn	void WindowManager::RestoreWindow(GLuint WindowIndex)
 *
 * @brief	Restore window.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 **************************************************************************************************/

GLboolean WindowManager::RestoreWindow(GLuint WindowIndex)
{
	if(DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->Restore();
	}

	return FOUNDATION_ERROR;
	//implement window focusing
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowSwapInterval(const char* WindowName, GLint a_SyncSetting)
 *
 * @brief	Sets window swap interval.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName   	Name of the window.
 * @param	a_SyncSetting	The synchronize setting.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowSwapInterval(const char* WindowName, GLint a_SyncSetting)
{
	if (DoesExist(WindowName))
	{
		 return GetWindowByName(WindowName)->SetSwapInterval(a_SyncSetting);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowSwapInterval(GLuint WindowIndex, GLint a_SyncSetting)
 *
 * @brief	Sets window swap interval.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex  	Zero-based index of the window.
 * @param	a_SyncSetting	The synchronize setting.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowSwapInterval(GLuint WindowIndex, GLint a_SyncSetting)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetSwapInterval(a_SyncSetting);
	}

	return FOUNDATION_ERROR;
}

GLboolean WindowManager::SetWindowStyle(const char* WindowName, GLuint WindowStyle)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetStyle(WindowStyle);
	}

	return FOUNDATION_ERROR;
}

GLboolean WindowManager::SetWindowStyle(GLuint WindowIndex, GLuint WindowStyle)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetStyle(WindowStyle);
	}

	return FOUNDATION_ERROR;
}

GLboolean WindowManager::EnableWindowDecorator(const char* WindowName, GLbitfield Decorators)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->EnableDecorator(Decorators);
	}

	return FOUNDATION_ERROR;

}

GLboolean WindowManager::EnableWindowDecorator(GLuint WindowIndex, GLbitfield Decorators)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->EnableDecorator(Decorators);
	}

	return FOUNDATION_ERROR;
}

GLboolean WindowManager::DisableWindowDecorator(const char* WindowName, GLbitfield Decorators)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->DisableDecorator(Decorators);
	}

	return FOUNDATION_ERROR;
}

GLboolean WindowManager::DisableWindowDecorator(GLuint WindowIndex, GLbitfield Decorators)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->DisableDecorator(Decorators);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnKeyEvent(const char* WindowName, OnKeyEvent OnKey)
 *
 * @brief	Sets window on key event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	OnKey	  	The on key event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnKeyEvent(const char* WindowName, OnKeyEvent OnKey)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnKeyEvent(OnKey);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnKeyEvent(GLuint WindowIndex, OnKeyEvent OnKey)
 *
 * @brief	Sets window on key event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnKey	   	The on key event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnKeyEvent(GLuint WindowIndex, OnKeyEvent OnKey)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnKeyEvent(OnKey);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseButtonEvent(const char* WindowName, OnMouseButtonEvent OnMouseButton)
 *
 * @brief	Sets window on mouse button event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName   	Name of the window.
 * @param	OnMouseButton	The on mouse button event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseButtonEvent(const char* WindowName, OnMouseButtonEvent OnMouseButton)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMouseButtonEvent(OnMouseButton);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseButtonEvent(GLuint WindowIndex, OnMouseButtonEvent OnMouseButton)
 *
 * @brief	Sets window on mouse button event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex  	Zero-based index of the window.
 * @param	OnMouseButton	The on mouse button event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseButtonEvent(GLuint WindowIndex, OnMouseButtonEvent OnMouseButton)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMouseButtonEvent(OnMouseButton);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseWheelEvent(const char* WindowName, OnMouseWheelEvent OnMouseWheel)
 *
 * @brief	Sets window on mouse wheel event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName  	Name of the window.
 * @param	OnMouseWheel	The on mouse wheel event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseWheelEvent(const char* WindowName, OnMouseWheelEvent OnMouseWheel)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMouseWheelEvent(OnMouseWheel);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseWheelEvent(GLuint WindowIndex, OnMouseWheelEvent OnMouseWheel)
 *
 * @brief	Sets window on mouse wheel event.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex 	Zero-based index of the window.
 * @param	OnMouseWheel	The on mouse wheel event callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseWheelEvent(GLuint WindowIndex, OnMouseWheelEvent OnMouseWheel)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMouseWheelEvent(OnMouseWheel);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnDestroyed(const char* WindowName, OnDestroyedEvent OnDestroyed)
 *
 * @brief	Sets window on destroyed.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName 	Name of the window.
 * @param	OnDestroyed	The on destroyed callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnDestroyed(const char* WindowName, OnDestroyedEvent OnDestroyed)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnDestroyed(OnDestroyed);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnDestroyed(GLuint WindowIndex, OnDestroyedEvent OnDestroyed)
 *
 * @brief	Sets window on destroyed.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnDestroyed	The on destroyed callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnDestroyed(GLuint WindowIndex, OnDestroyedEvent OnDestroyed)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnDestroyed(OnDestroyed);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMaximized(const char* WindowName, OnMaximizedEvent OnMaximized)
 *
 * @brief	Sets window on maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName 	Name of the window.
 * @param	OnMaximized	The on maximized callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMaximized(const char* WindowName, OnMaximizedEvent OnMaximized)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMaximized(OnMaximized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMaximized(GLuint WindowIndex, OnMaximizedEvent OnMaximized)
 *
 * @brief	Sets window on maximized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnMaximized	The on maximized callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMaximized(GLuint WindowIndex, OnMaximizedEvent OnMaximized)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMaximized(OnMaximized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMinimized(const char* WindowName, OnMinimizedEvent OnMinimized)
 *
 * @brief	Sets window on minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName 	Name of the window.
 * @param	OnMinimized	The on minimized callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMinimized(const char* WindowName, OnMinimizedEvent OnMinimized)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMinimized(OnMinimized);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMinimized(GLuint WindowIndex, OnMinimizedEvent OnMinimized)
 *
 * @brief	Sets window on minimized.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnMinimized	The on minimized callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMinimized(GLuint WindowIndex, OnMinimizedEvent OnMinimized)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMinimized(OnMinimized);
	}

	return FOUNDATION_ERROR;
}

/*void WindowManager::SetWindowOnRestored(const char* WindowName, OnRestoredEvent OnRestored)
{
	if (DoesExist(WindowName))
	{
		GetWindowByName(WindowName)->SetOnRestored(OnRestored);
	}
}

void WindowManager::SetWindowOnRestored(GLuint WindowIndex, OnRestoredEvent OnRestored)
{
	if (DoesExist(WindowIndex))
	{
		GetWindowByIndex(WindowIndex)->SetOnRestored(OnRestored);
	}
}*/

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnFocus(const char* WindowName, OnFocusEvent OnFocus)
 *
 * @brief	Sets window on focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	OnFocus   	The on focus callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnFocus(const char* WindowName, OnFocusEvent OnFocus)
{
	if(IsValidString(WindowName))
	{
		GetWindowByName(WindowName)->FocusEvent = OnFocus;
		return FOUNDATION_OKAY;
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnFocus(GLuint WindowIndex, OnFocusEvent OnFocus)
 *
 * @brief	Sets window on focus.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnFocus	   	The on focus callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnFocus(GLuint WindowIndex, OnFocusEvent OnFocus)
{
	if(DoesExist(WindowIndex))
	{
		GetWindowByIndex(WindowIndex)->FocusEvent = OnFocus;
		return FOUNDATION_OKAY;
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMoved(const char* WindowName, OnMovedEvent OnMoved)
 *
 * @brief	Sets window on moved.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	OnMoved   	The on moved callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMoved(const char* WindowName, OnMovedEvent OnMoved)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMoved(OnMoved);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMoved(GLuint WindowIndex, OnMovedEvent OnMoved)
 *
 * @brief	Sets window on moved.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnMoved	   	The on moved callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMoved(GLuint WindowIndex, OnMovedEvent OnMoved)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMoved(OnMoved);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnResize(const char* WindowName, OnResizeEvent OnResize)
 *
 * @brief	Sets window on resize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName	Name of the window.
 * @param	OnResize  	The on resize callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnResize(const char* WindowName, OnResizeEvent OnResize)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnResize(OnResize);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnResize(GLuint WindowIndex, OnResizeEvent OnResize)
 *
 * @brief	Sets window on resize.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnResize   	The on resize callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnResize(GLuint WindowIndex, OnResizeEvent OnResize)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnResize(OnResize);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseMove(const char* WindowName, OnMouseMoveEvent OnMouseMove)
 *
 * @brief	Sets window on mouse move.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowName 	Name of the window.
 * @param	OnMouseMove	The on mouse move callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseMove(const char* WindowName, OnMouseMoveEvent OnMouseMove)
{
	if (DoesExist(WindowName))
	{
		return GetWindowByName(WindowName)->SetOnMouseMove(OnMouseMove);
	}

	return FOUNDATION_ERROR;
}

/**********************************************************************************************//**
 * @fn	void WindowManager::SetWindowOnMouseMove(GLuint WindowIndex, OnMouseMoveEvent OnMouseMove)
 *
 * @brief	Sets window on mouse move.
 *
 * @author	Ziyad
 * @date	29/11/2014
 *
 * @param	WindowIndex	Zero-based index of the window.
 * @param	OnMouseMove	The on mouse move callback.
 **************************************************************************************************/

GLboolean WindowManager::SetWindowOnMouseMove(GLuint WindowIndex, OnMouseMoveEvent OnMouseMove)
{
	if (DoesExist(WindowIndex))
	{
		return GetWindowByIndex(WindowIndex)->SetOnMouseMove(OnMouseMove);
	}

	return FOUNDATION_ERROR;
}

WindowManager* WindowManager::Instance = 0;
