// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "EquationTextBox.h"
#include "CalcViewModel/GraphingCalculator/EquationViewModel.h"

using namespace std;
using namespace Platform;
using namespace CalculatorApp;
using namespace CalculatorApp::Common;
using namespace CalculatorApp::Controls;
using namespace CalculatorApp::ViewModel;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Text;

DEPENDENCY_PROPERTY_INITIALIZATION(EquationTextBox, EquationColor);
DEPENDENCY_PROPERTY_INITIALIZATION(EquationTextBox, KeyGraphFeaturesContent);
DEPENDENCY_PROPERTY_INITIALIZATION(EquationTextBox, ColorChooserFlyout);

void EquationTextBox::OnApplyTemplate()
{
    m_equationButton = dynamic_cast<Button^>(GetTemplateChild("EquationButton"));
    m_richEditBox = dynamic_cast<RichEditBox^>(GetTemplateChild("EquationTextBox"));
    m_deleteButton = dynamic_cast<Button^>(GetTemplateChild("DeleteButton"));
    m_removeButton = dynamic_cast<Button^>(GetTemplateChild("RemoveButton"));
    m_functionButton = dynamic_cast<Button^>(GetTemplateChild("FunctionButton"));
    m_colorChooserButton = dynamic_cast<ToggleButton^>(GetTemplateChild("ColorChooserButton"));

    if (m_richEditBox != nullptr)
    {
        m_richEditBox->GotFocus += ref new RoutedEventHandler(this, &EquationTextBox::OnRichEditBoxGotFocus);
        m_richEditBox->LostFocus += ref new RoutedEventHandler(this, &EquationTextBox::OnRichEditBoxLostFocus);
        m_richEditBox->TextChanged += ref new RoutedEventHandler(this, &EquationTextBox::OnRichEditBoxTextChanged);
    }

    if (m_equationButton != nullptr)
    {
        m_equationButton->Click += ref new RoutedEventHandler(this, &EquationTextBox::OnEquationButtonClicked);
    }

    if (m_deleteButton != nullptr)
    {
        m_deleteButton->Click += ref new RoutedEventHandler(this, &EquationTextBox::OnDeleteButtonClicked);
    }

    if (m_removeButton != nullptr)
    {
        m_removeButton->Click += ref new RoutedEventHandler(this, &EquationTextBox::OnRemoveButtonClicked);
    }

    if (m_colorChooserButton != nullptr)
    {
        m_colorChooserButton->Click += ref new RoutedEventHandler(this, &EquationTextBox::OnColorChooserButtonClicked);
    }

    if (m_functionButton != nullptr)
    {
        m_functionButton->Click += ref new RoutedEventHandler(this, &EquationTextBox::OnFunctionButtonClicked);
    }

    if (ColorChooserFlyout != nullptr)
    {
        ColorChooserFlyout->Opened += ref new EventHandler<Object^>(this, &EquationTextBox::OnColorFlyoutOpened);
        ColorChooserFlyout->Closed += ref new EventHandler<Object^>(this, &EquationTextBox::OnColorFlyoutClosed);
    }
}

void EquationTextBox::OnPointerEntered(PointerRoutedEventArgs^ e)
{
    m_isPointerOver = true;
    UpdateCommonVisualState();
}

void EquationTextBox::OnPointerExited(PointerRoutedEventArgs^ e)
{
    m_isPointerOver = false;
    UpdateCommonVisualState();
}

void EquationTextBox::OnPointerCanceled(PointerRoutedEventArgs^ e)
{
    m_isPointerOver = false;
    UpdateCommonVisualState();
}

void EquationTextBox::OnPointerCaptureLost(PointerRoutedEventArgs^ e)
{
    m_isPointerOver = false;
    UpdateCommonVisualState();
}

void EquationTextBox::OnColorFlyoutOpened(Object^ sender, Object^ e)
{
    m_isColorChooserFlyoutOpen = true;
    UpdateCommonVisualState();
}

void EquationTextBox::OnColorFlyoutClosed(Object^ sender, Object^ e)
{
    m_colorChooserButton->IsChecked = false;
    m_isColorChooserFlyoutOpen = false;
    UpdateCommonVisualState();
}

void EquationTextBox::OnRichEditBoxTextChanged(Object^ sender, RoutedEventArgs^ e)
{
    UpdateDeleteButtonVisualState();
}

void EquationTextBox::OnRichEditBoxGotFocus(Object^ sender, RoutedEventArgs^ e)
{
    m_isFocused = true;
    UpdateCommonVisualState();
    UpdateDeleteButtonVisualState();
}

void EquationTextBox::OnRichEditBoxLostFocus(Object^ sender, RoutedEventArgs^ e)
{
    m_isFocused = false;
    UpdateCommonVisualState();
    UpdateDeleteButtonVisualState();
}

void EquationTextBox::OnDeleteButtonClicked(Object^ sender, RoutedEventArgs^ e)
{
    if (m_richEditBox != nullptr)
    {
        m_richEditBox->TextDocument->SetText(::TextSetOptions::None, L"");
    }
}

void EquationTextBox::OnEquationButtonClicked(Object^ sender, RoutedEventArgs^ e)
{

}

void EquationTextBox::OnRemoveButtonClicked(Object^ sender, RoutedEventArgs^ e)
{
    RemoveButtonClicked(this, ref new RoutedEventArgs());
}

void EquationTextBox::OnColorChooserButtonClicked(Object^ sender, RoutedEventArgs^ e)
{
    if (ColorChooserFlyout != nullptr && m_richEditBox != nullptr)
    {
        ColorChooserFlyout->ShowAt(m_richEditBox);
    }
}

void EquationTextBox::OnFunctionButtonClicked(Object^ sender, RoutedEventArgs^ e)
{
    auto equationViewModel = static_cast<EquationViewModel^>(DataContext);
    equationViewModel->KeyGraphFeaturesVisibility = (equationViewModel->KeyGraphFeaturesVisibility == ::Visibility::Collapsed) ? ::Visibility::Visible : ::Visibility::Collapsed;
    UpdateCommonVisualState();
}

void EquationTextBox::UpdateDeleteButtonVisualState()
{
    String^ state;

    if (ShouldDeleteButtonBeVisible())
    {
        state = "ButtonVisible";
    }
    else
    {
        state = "ButtonCollapsed";
    }

    VisualStateManager::GoToState(this, state, true);
}

void EquationTextBox::UpdateCommonVisualState()
{
    String^ state = "Normal";

    if (m_isFocused)
    {
        state = "Focused";
    }
    else if (m_isPointerOver || m_isColorChooserFlyoutOpen)
    {
        state = "PointerOver";
    }

    VisualStateManager::GoToState(this, state, true);
}


Platform::String^ EquationTextBox::GetEquationText()
{
    String^ text;

    if (m_richEditBox != nullptr)
    {
        m_richEditBox->TextDocument->GetText(::TextGetOptions::NoHidden, &text);
    }

    return text;
}

void EquationTextBox::SetEquationText(Platform::String^ equationText)
{
    if (m_richEditBox != nullptr)
    {
        m_richEditBox->TextDocument->SetText(::TextSetOptions::None, equationText);
    }
}


bool EquationTextBox::ShouldDeleteButtonBeVisible()
{
    return (!GetEquationText()->IsEmpty() && m_isFocused);

}
