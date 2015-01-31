#include <windows.h>

LRESULT CALLBACK MainWindowCallback(HWND Window,
                                    UINT Message,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
  LRESULT Result = 0;

  switch(Message)
  {
    case WM_SIZE:
    {
      OutputDebugStringA("WM_Size\n");
    } break;

    case WM_DESTROY:
    {
      OutputDebugStringA("WM_DESTORY\n");
    } break;  

    case WM_CLOSE:
    {
      OutputDebugStringA("WM_CLOSE\n");
    }break;

    case WM_ACTIVATEAPP:
    { 
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      HDC DeviceContent = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.top;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      int Width = Paint.rcPaint.right - Paint.rcPaint.left;
      static DWORD Operation = WHITENESS;
      PatBlt(DeviceContent, X, Y, Width, Height, Operation);
      if(Operation == WHITENESS)
      {
        Operation = BLACKNESS;
      }
      else
      {
        Operation = WHITENESS;
      }
      EndPaint(Window, &Paint);
    } break;

    default:
    {
      // OutputDebugStringA("default\n");
      Result = DefWindowProc(Window, Message, wParam, lParam);
    } break;
  }

  return(Result);
}


int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE hPrevInstance,
                     LPSTR CmdLine,
                     int CmdShow)
{
  WNDCLASS WindowClass = {};
  // Check if HREDRAW/VREDRAW/OWNDC matter?
  WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc = MainWindowCallback;
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
      for(;;)
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
