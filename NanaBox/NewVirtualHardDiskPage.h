﻿#pragma once

#include "NewVirtualHardDiskPage.g.h"

#include <Windows.h>

namespace winrt
{
    using Windows::Foundation::IInspectable;
    using Windows::UI::Xaml::RoutedEventArgs;
}

namespace winrt::NanaBox::implementation
{
    struct NewVirtualHardDiskPage :
        NewVirtualHardDiskPageT<NewVirtualHardDiskPage>
    {
        NewVirtualHardDiskPage(
            _In_ HWND WindowHandle = nullptr);

        void InitializeComponent();

        void CreateButtonClick(
            winrt::IInspectable const& sender,
            winrt::RoutedEventArgs const& e);

        void CancelButtonClick(
            winrt::IInspectable const& sender,
            winrt::RoutedEventArgs const& e);

    private:

        HWND m_WindowHandle;
    };
}

namespace winrt::NanaBox::factory_implementation
{
    struct NewVirtualHardDiskPage : NewVirtualHardDiskPageT<
        NewVirtualHardDiskPage, implementation::NewVirtualHardDiskPage>
    {
    };
}