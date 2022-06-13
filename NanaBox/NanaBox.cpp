﻿/*
 * PROJECT:   NanaBox
 * FILE:      NanaBox.cpp
 * PURPOSE:   Implementation for NanaBox
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include "RdpClient.h"
#include "HostCompute.h"

#include <cwchar>

//#include <atlbase.h>
//#include <atlhost.h>

#include <winrt/Windows.Data.Json.h>

namespace winrt
{
    using Windows::Data::Json::JsonObject;
}

namespace
{
    struct OleClientSite : public winrt::implements<
        OleClientSite,
        IOleClientSite,
        IOleInPlaceSite>
    {
    private:

        HWND m_WindowHandle;

    public:

        OleClientSite(_In_ HWND WindowHandle = nullptr)
            : m_WindowHandle(WindowHandle)
        {

        }

        HRESULT STDMETHODCALLTYPE SaveObject()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE GetMoniker(
            _In_ DWORD dwAssign,
            _In_ DWORD dwWhichMoniker,
            _Out_ IMoniker** ppmk)
        {
            UNREFERENCED_PARAMETER(dwAssign);
            UNREFERENCED_PARAMETER(dwWhichMoniker);
            *ppmk = nullptr;
            return E_NOTIMPL;
        }

        HRESULT STDMETHODCALLTYPE GetContainer(
            _Out_ IOleContainer** ppContainer)
        {
            *ppContainer = nullptr;
            return E_NOTIMPL;
        }

        HRESULT STDMETHODCALLTYPE ShowObject()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnShowWindow(
            _In_ BOOL fShow)
        {
            UNREFERENCED_PARAMETER(fShow);
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE RequestNewObjectLayout()
        {
            return E_NOTIMPL;
        }

    public:

        HRESULT STDMETHODCALLTYPE GetWindow(
            _Out_ HWND* phwnd)
        {
            *phwnd = this->m_WindowHandle;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(
            _In_ BOOL fEnterMode)
        {
            UNREFERENCED_PARAMETER(fEnterMode);
            return S_OK;
        }

    public:

        HRESULT STDMETHODCALLTYPE CanInPlaceActivate()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnInPlaceActivate()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnUIActivate()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE GetWindowContext(
            _Out_ IOleInPlaceFrame** ppFrame,
            _Out_ IOleInPlaceUIWindow** ppDoc,
            _Out_ LPRECT lprcPosRect,
            _Out_ LPRECT lprcClipRect,
            _Inout_ LPOLEINPLACEFRAMEINFO lpFrameInfo)
        {
            *ppFrame = nullptr;
            *ppDoc = nullptr;
            ::GetClientRect(this->m_WindowHandle, lprcPosRect);
            ::GetClientRect(this->m_WindowHandle, lprcClipRect);
            lprcPosRect->top += 100;
            lprcPosRect->bottom -= 50;
            lprcClipRect->top += 100;
            lprcClipRect->bottom -= 50;
            lpFrameInfo->haccel = nullptr;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE Scroll(
            _In_ SIZE scrollExtant)
        {
            UNREFERENCED_PARAMETER(scrollExtant);
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnUIDeactivate(
            _In_ BOOL fUndoable)
        {
            UNREFERENCED_PARAMETER(fUndoable);
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE DiscardUndoState()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE DeactivateAndUndo()
        {
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnPosRectChange(
            _In_ LPCRECT lprcPosRect)
        {
            UNREFERENCED_PARAMETER(lprcPosRect);
            return S_OK;
        }
    };

    winrt::com_ptr<NanaBox::RdpClient> g_RdpClient;

    winrt::com_ptr<IOleClientSite> g_OleClientSite;

    winrt::hstring g_VMID = L"48781dff-90cc-4650-89c3-fe12e6210b19";

    static LRESULT CALLBACK NanaBoxMainWindowCallback(
        _In_ HWND   hWnd,
        _In_ UINT   uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE:
        {
            // Note: Return -1 directly because WM_DESTROY message will be sent
            // when destroy the window automatically. We free the resource when
            // processing the WM_DESTROY message of this window.

            /*LPCREATESTRUCT CreateStruct =
                reinterpret_cast<LPCREATESTRUCT>(lParam);*/

            /*WNDCLASSEXW WindowClass;
            WindowClass.cbSize = sizeof(WNDCLASSEXW);
            WindowClass.style = 0;
            WindowClass.lpfnWndProc = ::DefWindowProcW;
            WindowClass.cbClsExtra = 0;
            WindowClass.cbWndExtra = 0;
            WindowClass.hInstance = ::GetModuleHandleW(nullptr);
            WindowClass.hIcon = nullptr;
            WindowClass.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
            WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(
                ::GetStockObject(BLACK_BRUSH));
            WindowClass.lpszMenuName = nullptr;
            WindowClass.lpszClassName = L"NanaBoxHostWindow";
            WindowClass.hIconSm = nullptr;
            if (!::RegisterClassExW(&WindowClass))
            {
                return -1;
            }

            RECT ParentClientRect;
            winrt::check_bool(::GetClientRect(hWnd, &ParentClientRect));

            HWND WindowHandle = ::CreateWindowExW(
                0,
                WindowClass.lpszClassName,
                L"NanaBoxHost",
                WS_CHILD | WS_VISIBLE,
                0,
                0,
                ParentClientRect.right - ParentClientRect.left,
                ParentClientRect.bottom - ParentClientRect.top,
                hWnd,
                nullptr,
                WindowClass.hInstance,
                nullptr);
            if (!WindowHandle)
            {
                return -1;
            }*/

            g_RdpClient = winrt::make_self<NanaBox::RdpClient>();

            RECT ClientRect;
            winrt::check_bool(::GetClientRect(hWnd, &ClientRect));
            ClientRect.top += 100;
            ClientRect.bottom -= 50;

            winrt::com_ptr<IOleObject> OleObject =
                g_RdpClient->RawControl().as<IOleObject>();

            winrt::com_ptr<IOleInPlaceActiveObject> OleInPlaceActiveObject =
                g_RdpClient->RawControl().as<IOleInPlaceActiveObject>();

            winrt::com_ptr<IOleInPlaceObject> OleInPlaceObject =
                g_RdpClient->RawControl().as<IOleInPlaceObject>();

            g_OleClientSite =
                winrt::make<OleClientSite>(hWnd);

            winrt::check_hresult(OleObject->SetClientSite(
                g_OleClientSite.get()));

            winrt::check_hresult(OleObject->DoVerb(
                OLEIVERB_INPLACEACTIVATE,
                nullptr,
                g_OleClientSite.get(),
                0,
                hWnd,
                &ClientRect));

            g_RdpClient->Server(L"localhost");
            g_RdpClient->RDPPort(2179);
            g_RdpClient->AuthenticationServiceClass(L"Microsoft Virtual Console Service");
            g_RdpClient->AuthenticationLevel(0);
            g_RdpClient->EnableCredSspSupport(true);
            g_RdpClient->NegotiateSecurityLayer(false);
            g_RdpClient->MinInputSendInterval(20);

            VARIANT Value;
            Value.vt = VT_BOOL;
            Value.boolVal = VARIANT_TRUE;
            g_RdpClient->Property(L"DisableCredentialsDelegation", Value);

            g_RdpClient->PCB(g_VMID.c_str()/* + winrt::hstring(L";" L"EnhancedMode=1")*/);

            g_RdpClient->Connect();

            /*g_RdpClient->OnLoginComplete([hWnd]()
            {
                ::Sleep(200);

                RECT ClientRect;
                winrt::check_bool(::GetClientRect(hWnd, &ClientRect));
                ClientRect.top += 100;
                ClientRect.bottom -= 50;

                ULONG RawWidth = ClientRect.right - ClientRect.left;
                ULONG RawHeight = ClientRect.bottom - ClientRect.top;

                UINT WindowDpi = ::GetDpiForWindow(hWnd);

                g_RdpClient->UpdateSessionDisplaySettings(
                    RawWidth,
                    RawHeight,
                    (ULONG)(RawWidth * 100 * 25.4),
                    (ULONG)(RawHeight * 100 * 25.4),
                    0,
                    (ULONG)(WindowDpi * 100.0 / 96.0),
                    100);
            });*/

            g_RdpClient->OnDisconnected([](LONG)
            {
                g_RdpClient->Connect();
            });

            return 0;
        }
        case WM_SETFOCUS:
        {
            break;
        }
        case WM_SIZE:
        {
            RECT ClientRect;
            winrt::check_bool(::GetClientRect(hWnd, &ClientRect));
            ClientRect.top += 100;
            ClientRect.bottom -= 50;

            winrt::com_ptr<IOleInPlaceObject> OleInPlaceObject =
                g_RdpClient->RawControl().as<IOleInPlaceObject>();
            winrt::check_hresult(OleInPlaceObject->SetObjectRects(
                &ClientRect, &ClientRect));

            if (g_RdpClient->Connected() == 1)
            {
                ULONG RawWidth = ClientRect.right - ClientRect.left;
                ULONG RawHeight = ClientRect.bottom - ClientRect.top;

                UINT WindowDpi = ::GetDpiForWindow(hWnd);

                g_RdpClient->UpdateSessionDisplaySettings(
                    RawWidth,
                    RawHeight,
                    (ULONG)(RawWidth * 100 * 25.4),
                    (ULONG)(RawHeight * 100 * 25.4),
                    0,
                    (ULONG)(WindowDpi * 100.0 / 96.0),
                    100);
            }

            break;
        }
        case WM_DPICHANGED:
        {
            LPRECT NewWindowRectangle = reinterpret_cast<LPRECT>(lParam);

            ::SetWindowPos(
                hWnd,
                nullptr,
                NewWindowRectangle->left,
                NewWindowRectangle->top,
                NewWindowRectangle->right - NewWindowRectangle->left,
                NewWindowRectangle->bottom - NewWindowRectangle->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
        }
        case WM_MENUCHAR:
        {
            // Reference: https://github.com/microsoft/terminal
            //            /blob/756fd444b1d443320cf2ed6887d4b65aa67a9a03
            //            /scratch/ScratchIslandApp
            //            /WindowExe/SampleIslandWindow.cpp#L155
            // Return this LRESULT here to prevent the app from making a bell
            // when alt+key is pressed. A menu is active and the user presses a
            // key that does not correspond to any mnemonic or accelerator key.

            return MAKELRESULT(0, MNC_CLOSE);
        }
        case WM_SETTINGCHANGE:
        {
            if (lParam && 0 == std::wcscmp(
                reinterpret_cast<LPWSTR>(lParam),
                L"ImmersiveColorSet"))
            {
                
            }

            break;
        }
        case WM_DESTROY:
        {
            winrt::com_ptr<IOleObject> OleObject =
                g_RdpClient->RawControl().as<IOleObject>();

            winrt::check_hresult(OleObject->Close(OLECLOSE_NOSAVE));
            winrt::check_hresult(OleObject->SetClientSite(nullptr));

            ::PostQuitMessage(0);

            break;
        }
        default:
            return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    static constexpr wchar_t c_VmConfiguration[] = LR"(
    {
        "SchemaVersion": {
            "Major": 2,
            "Minor": 1
        },
        "Owner": "Sample",
        "ShouldTerminateOnLastHandleClosed": true,
        "VirtualMachine": {
            "Chipset": {
                "Uefi": {
                    "ApplySecureBootTemplate": "Apply",
                    "SecureBootTemplateId": "1734c6e8-3154-4dda-ba5f-a874cc483422"
                }
            },
            "ComputeTopology": {
                "Memory": {
                    "Backing": "Virtual",
                    "SizeInMB": 2048
                },
                "Processor": {
                    "Count": 2,
                    "ExposeVirtualizationExtensions": true
                }
            },
            "Devices": {
                "VideoMonitor": {},
                "EnhancedModeVideo": {},
                "Keyboard": {},
                "Mouse": {},
                "Scsi": {
                    "Primary disk": {
                        "Attachments": {
                            "0": {
                                "Type": "Iso",
                                "Path": "D:\\Projects\\MouriNaruto\\AluImageWorkspace\\Assets\\WindowsImages\\22000.493\\en-us_windows_11_consumer_editions_updated_feb_2022_x64_dvd_aa300219.iso"
                            },
                            "1": {
                                "Type": "VirtualDisk",
                                "Path": "D:\\Hyper-V\\DemoVM\\Virtual Hard Disks\\DemoVM.vhdx"
                            }
                        }
                    }
                }
            },
            "GuestState": {
                "GuestStateFilePath": "D:\\Hyper-V\\DemoVM\\Virtual Machines\\48781DFF-90CC-4650-89C3-FE12E6210B19.vmgs",
                "RuntimeStateFilePath": "D:\\Hyper-V\\DemoVM\\Virtual Machines\\48781DFF-90CC-4650-89C3-FE12E6210B19.vmrs"
            },
            "SecuritySettings": {
                "EnableTpm": false
            }
        }
    })";

    /*winrt::check_hresult(::HcsGrantVmAccess(
        L"Sample",
        L"D:\\Hyper-V\\DemoVM\\Virtual Machines\\48781DFF-90CC-4650-89C3-FE12E6210B19.vmgs"));
    winrt::check_hresult(::HcsGrantVmAccess(
        L"Sample",
        L"D:\\Hyper-V\\DemoVM\\Virtual Machines\\48781DFF-90CC-4650-89C3-FE12E6210B19.vmrs"));*/

    NanaBox::ComputeSystem test(L"Sample", c_VmConfiguration);

    test.Start();

    auto fuck = test.GetProperties();

    winrt::JsonObject testobj = winrt::JsonObject::Parse(fuck);
    g_VMID = testobj.GetNamedString(L"RuntimeId");

    //::MessageBoxW(nullptr, fuck.c_str(), L"NanaBox", 0);

    WNDCLASSEXW WindowClass;
    WindowClass.cbSize = sizeof(WNDCLASSEXW);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = ::NanaBoxMainWindowCallback;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = hInstance;
    WindowClass.hIcon = nullptr;
    WindowClass.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
    WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(
        ::GetStockObject(BLACK_BRUSH));
    WindowClass.lpszMenuName = nullptr;
    WindowClass.lpszClassName = L"NanaBoxMainWindow";
    WindowClass.hIconSm = nullptr;
    if (!::RegisterClassExW(&WindowClass))
    {
        return -1;
    }

    HWND WindowHandle = ::CreateWindowExW(
        0,
        WindowClass.lpszClassName,
        L"NanaBox",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        nullptr,
        nullptr,
        hInstance,
        nullptr);
    if (!WindowHandle)
    {
        return -1;
    }

    ::ShowWindow(WindowHandle, nShowCmd);
    ::UpdateWindow(WindowHandle);

    MSG Message;
    while (::GetMessageW(&Message, nullptr, 0, 0))
    {
        ::TranslateMessage(&Message);
        ::DispatchMessageW(&Message);
    }

    return static_cast<int>(Message.wParam);
}
