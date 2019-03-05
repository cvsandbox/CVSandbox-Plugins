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
#include "PhidgetAdvancedServoPlugin.hpp"
#include "image_hardware_16x16.h"

static void PluginInitializer( );

// Version of the plug-in
static xversion PluginVersion = { 1, 0, 0 };

// ID of the plug-in
static xguid PluginID = { 0xAF000003, 0x00000000, 0x80000001, 0x00000002 };

// Serial Number property
static PropertyDescriptor serialNumberProperty =
{ XVT_I4, "Serial Number", "serialNumber", "Serial number of the device to open.", PropertyFlag_None };

// Version property
static PropertyDescriptor versionProperty =
{ XVT_I4, "Version", "version", "Version of the opened device.", PropertyFlag_ReadOnly };

// Type property
static PropertyDescriptor typeProperty =
{ XVT_String, "Device Type", "deviceType", "Type of the opened device.", PropertyFlag_ReadOnly };

// Motor property
static PropertyDescriptor motorCountProperty =
{ XVT_U4, "Motor Count", "motorCount", "Number of motors supported by the board.", PropertyFlag_ReadOnly };

// Engaged property
static PropertyDescriptor engagedProperty =
{ XVT_Bool | XVT_Array, "Engaged", "engaged", "State of digital inputs.", PropertyFlag_None };

// Stopped property
static PropertyDescriptor stoppedProperty =
{ XVT_Bool | XVT_Array, "Stopped", "stopped", "Tells if motor has finished moving to its target position.", PropertyFlag_ReadOnly | PropertyFlag_Dynamic };

// Actual Position property
static PropertyDescriptor actualPositionProperty =
{ XVT_R4 | XVT_Array, "Actual Position", "actualPosition", "Actual position of motors.", PropertyFlag_ReadOnly | PropertyFlag_Dynamic };

// Target Position property
static PropertyDescriptor targetPositionProperty =
{ XVT_R4 | XVT_Array, "Target Position", "targetPosition", "Target position of motors.", PropertyFlag_None };

// Velocity Limit property
static PropertyDescriptor velocityLimitProperty =
{ XVT_R4 | XVT_Array, "Velocity Limit", "velocityLimit", "Velocity limits for motors.", PropertyFlag_None };

// Acceleration property
static PropertyDescriptor accelerationProperty =
{ XVT_R4 | XVT_Array, "Acceleration", "acceleration", "Acceleration values for motors.", PropertyFlag_None };

// Position Range property
static PropertyDescriptor positionRangeProperty =
{ XVT_RangeF | XVT_Array, "Position Range", "positionRange", "Specifies range in which motors can move.", PropertyFlag_None };

// Velocity Range property
static PropertyDescriptor velocityRangeProperty =
{ XVT_RangeF | XVT_Array, "Velocity Range", "velocityRange", "Specifies the range in which velocity limit can be set.", PropertyFlag_ReadOnly };

// Acceleration Range property
static PropertyDescriptor accelerationRangeProperty =
{ XVT_RangeF | XVT_Array, "Acceleration Range", "accelerationRange", "Specifies the range in which acceleration can be set.", PropertyFlag_ReadOnly };


// Array of available properties
static PropertyDescriptor* pluginProperties[] =
{
    &serialNumberProperty, &versionProperty, &typeProperty, &motorCountProperty, &engagedProperty, &stoppedProperty,
    &actualPositionProperty, &targetPositionProperty, &velocityLimitProperty, &accelerationProperty,
    &positionRangeProperty, &velocityRangeProperty, &accelerationRangeProperty
};

// Let the class itself know description of its properties
const PropertyDescriptor** PhidgetAdvancedServoPlugin::propertiesDescription = (const PropertyDescriptor**) pluginProperties;

// Register the plug-in
REGISTER_CPP_PLUGIN_WITH_PROPS
(
    PluginID,
    PluginFamilyID_Default,

    PluginType_Device,
    PluginVersion,
    "Phidget Advanced Servo",
    "PhidgetAdvancedServo",
    "Plug-in to access Phidget Advanced Servo board.",

    "The plug-in allows controlling <a href='http://www.phidgets.com/products.php?product_id=1061'>Phidget Advanced Servo</a> board."
    "For every attached servo motor, it allows setting its target position, velocity limit, acceleration and movement range."
    ,

    &image_hardware_16x16,
    nullptr,
    PhidgetAdvancedServoPlugin,

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
}
