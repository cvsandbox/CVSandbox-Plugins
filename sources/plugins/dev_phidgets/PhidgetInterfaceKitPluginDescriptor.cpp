/*
    Phidgets device plug-ins for Computer Vision Sandbox

    Copyright (C) 2016-2019, cvsandbox
    http://www.cvsandbox.com/contacts.html

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <iplugincpp.hpp>
#include "PhidgetInterfaceKitPlugin.hpp"
#include "image_hardware_16x16.h"

static void PluginInitializer( );

// Version of the plug-in
static xversion PluginVersion = { 1, 0, 0 };

// ID of the plug-in
static xguid PluginID = { 0xAF000003, 0x00000000, 0x80000001, 0x00000001 };

// Serial Number property
static PropertyDescriptor serialNumberProperty =
{ XVT_I4, "Serial Number", "serialNumber", "Serial number of the device to open.", PropertyFlag_None };

// Ratiometric State property
static PropertyDescriptor ratiometricStateProperty =
{ XVT_Bool, "Ratiometric State", "ratiometricState", "  .", PropertyFlag_None };

// Version property
static PropertyDescriptor versionProperty =
{ XVT_I4, "Version", "version", "Version of the opened device.", PropertyFlag_ReadOnly };

// Type property
static PropertyDescriptor typeProperty =
{ XVT_String, "Device Type", "deviceType", "Type of the opened device.", PropertyFlag_ReadOnly };

// Digital Input Count property
static PropertyDescriptor digitalInputCountProperty =
{ XVT_U4, "Digital Input Count", "digitalInputCount", "Number of digital inputs supported by the board.", PropertyFlag_ReadOnly };

// Digital Output Count property
static PropertyDescriptor digitalOutputCountProperty =
{ XVT_U4, "Digital Output Count", "digitalOutputCount", "Number of digital outputs supported by the board.", PropertyFlag_ReadOnly };

// Analog Input Count property
static PropertyDescriptor analogInputCountProperty =
{ XVT_U4, "Analog Input Count", "analogInputCount", "Number of analog inputs supported by the board.", PropertyFlag_ReadOnly };

// Digital Inputs property
static PropertyDescriptor digitalInputsProperty =
{ XVT_Bool | XVT_Array, "Digital Inputs", "digitalInputs", "State of digital inputs.", PropertyFlag_ReadOnly | PropertyFlag_Dynamic };

// Digital Outputs property
static PropertyDescriptor digitalOutputsProperty =
{ XVT_Bool | XVT_Array, "Digital Outputs", "digitalOutputs", "State of digital outputs.", PropertyFlag_Dynamic };

// Analog Inputs property
static PropertyDescriptor analogInputsProperty =
{ XVT_U2 | XVT_Array, "Analog Inputs", "analogInputs", "State of analog inputs.", PropertyFlag_ReadOnly | PropertyFlag_Dynamic };

// Array of available properties
static PropertyDescriptor* pluginProperties[] =
{
    &serialNumberProperty, &ratiometricStateProperty, &versionProperty, &typeProperty,
    &digitalInputCountProperty, &digitalOutputCountProperty, &analogInputCountProperty,
    &digitalInputsProperty, &digitalOutputsProperty, &analogInputsProperty
};

// Let the class itself know description of its properties
const PropertyDescriptor** PhidgetInterfaceKitPlugin::propertiesDescription = (const PropertyDescriptor**) pluginProperties;

// Register the plug-in
REGISTER_CPP_PLUGIN_WITH_PROPS
(
    PluginID,
    PluginFamilyID_Default,

    PluginType_Device,
    PluginVersion,
    "Phidget Interface Kit",
    "PhidgetInterfaceKit",
    "Plug-in to access Phidget Interface Kit board.",

    "The plug-in allows controlling <a href='http://www.phidgets.com/products.php?product_id=1018'>Phidget Interface Kit</a> board."
    "It allows reading its digital and analog inputs, and set its digital outputs."
    ,

    &image_hardware_16x16,
    nullptr,
    PhidgetInterfaceKitPlugin,

    XARRAY_SIZE( pluginProperties ),
    pluginProperties,
    PluginInitializer,
    nullptr, // no clean-up
    nullptr  // no dynamic properties update
);

// Complete properties description by initializing those parts, which were not 
// initialized during properties array declaration
static void PluginInitializer( )
{
    ratiometricStateProperty.DefaultValue.type = XVT_Bool;
    ratiometricStateProperty.DefaultValue.value.boolVal = true;
}
