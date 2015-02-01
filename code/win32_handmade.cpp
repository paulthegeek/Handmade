#include <windows.h>

#define internal static
#define local_persist static
#define global_variable static

// TODO This is a global for now
global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

//DIB means Device Independent Bitmap
internal void Win32ResizeDIBSection(int Width, int Height)
{
  if(BitmapHandle)
  {
    DeleteObject(BitmapHandle);
  }
  if(!BitmapDeviceContext)
  {
    //TODO Should we recreate these under certain special circumtances
    BitmapDeviceContext = CreateCompatibleDC(0); 
  }

  //TODO Free our DIB section
  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  BitmapInfo.bmiHeader.biHeight = Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;


  BitmapHandle = CreateDIBSection(BitmapDeviceContext,
                                  &BitmapInfo,
                                  DIB_RGB_COLORS,
                                  &BitmapMemory,
                                  0, 0);

}

internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
  StretchDIBits(DeviceContext,
                X, Y, Width, Height,
                X, Y, Width, Height,
                BitmapMemory,
                &BitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(HWND Window,
                                    UINT Message,
                                    WPARAM WParam,
                                    LPARAM LParam)
{
  LRESULT Result = 0;

  switch(Message)
  {
    case WM_SIZE:
    {
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top;
      Win32ResizeDIBSection(Width, Height);
      OutputDebugStringA("WM_Size\n");
    } break;

    case WM_CLOSE:
    {
      //TODO Handle this with a message to the user?
      Running = false;  
      OutputDebugStringA("WM_CLOSE\n");
    }break;

    case WM_DESTROY:
    {
      //TODO Handle this as an error - recreate window?
      Running = false;
      OutputDebugStringA("WM_DESTORY\n");
    } break;

    case WM_ACTIVATEAPP:
    { 
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.top;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      int Width = Paint.rcPaint.right - Paint.rcPaint.left;
      Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
      EndPaint(Window, &Paint);
    } break;

    default:
    {
      // OutputDebugStringA("default\n");
      Result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
  }

  return(Result);
}

// WinMain is the entry point for the application
int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE hPrevInstance,
                     LPSTR CmdLine,
                     int CmdShow)
{
  WNDCLASS WindowClass = {};
  // TODO Check if HREDRAW/VREDRAW/OWNDC matter?
  WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc = Win32MainWindowCallback;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "HandmadeHeroWindowClass";
  // HICON     hIcon;

  if(RegisterClass(&WindowClass))
  {
    HWND WindowHandle = CreateWindowEx(0,
                                       WindowClass.lpszClassName,
                                       "Handmade Hero",
                                       WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       0,
                                       0,
                                       Instance,
                                       0);
    if(WindowHandle)
    {
      MSG Message;

      Running = true;
      while(Running)
      {
        BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
        if(MessageResult > 0)
        {
          TranslateMessage(&Message);
          DispatchMessage(&Message);
        }
        else
        {
          break;
        }
      }
    }
    else
    {
      //TODO Logging
    }
  }
  else
  {
    //TODO Logging
  }

  return (0);
}
