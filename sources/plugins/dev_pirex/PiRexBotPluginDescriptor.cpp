/*
    PiRex Bot plug-ins for Computer Vision Sandbox

    Copyright (C) 2017-2019, cvsandbox
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
#include "PiRexBotPlugin.hpp"

static void PluginInitializer( );

// Version of the plug-in
static xversion PluginVersion = { 1, 0, 0 };

// ID of the plug-in
static xguid PluginID = { 0xAF000003, 0x00000000, 0x80000002, 0x00000001 };

// Address property
static PropertyDescriptor jpegUrlProperty =
{ XVT_String, "Address", "address", "IP address or DNS name of PiRex Bot.", PropertyFlag_None };
// Port property
static PropertyDescriptor portProperty =
{ XVT_U2, "Port", "port", "Port number to connect to.", PropertyFlag_None };
// User name property
static PropertyDescriptor userNameProperty =
{ XVT_String, "User name", "userName", "User name used for authentication.", PropertyFlag_None };
// Password property
static PropertyDescriptor passwordProperty =
{ XVT_String, "Password", "password", "Password used for authentication.", PropertyFlag_None };

// >>>>> Device run-time properties

// Bot Version property
static PropertyDescriptor botVersionProperty =
{ XVT_String, "Bot Version", "botVersion", "Version of the PiRex Bot's firmware.", PropertyFlag_ReadOnly | PropertyFlag_DeviceRuntimeConfiguration };

// Left Motor property
static PropertyDescriptor leftMotorProperty =
{ XVT_I1, "Left Motor", "leftMotor", "Power to set for the robot's left motor.", PropertyFlag_DeviceRuntimeConfiguration };
// Right Motor property
static PropertyDescriptor rightMotorProperty =
{ XVT_I1, "Right Motor", "rightMotor", "Power to set for the robot's right motor.", PropertyFlag_DeviceRuntimeConfiguration };
// Obstacle Distance property
static PropertyDescriptor obstacleDistanceProperty =
{ XVT_R4, "Obstacle Distance", "obstacleDistance", "Distance to an obstacle in front of the robot (cm).", PropertyFlag_ReadOnly | PropertyFlag_DeviceRuntimeConfiguration };

// <<<<<

// Array of available properties
static PropertyDescriptor* pluginProperties[] =
{
    &jpegUrlProperty, &portProperty, &userNameProperty, &passwordProperty,
    &botVersionProperty,
    &leftMotorProperty, &rightMotorProperty, &obstacleDistanceProperty
};

// Let the class itself know description of its properties
const PropertyDescriptor** PiRexBotPlugin::propertiesDescription = (const PropertyDescriptor**) pluginProperties;

// Register the plug-in
REGISTER_CPP_PLUGIN_WITH_PROPS
(
    PluginID,
    PluginFamilyID_Default,

    PluginType_Device,
    PluginVersion,
    "PiRex Bot",
    "PiRexBot",
    "Plug-in to control PiRex robot.",

    /* Long description */
    ""
    ,
    nullptr,
    nullptr,
    PiRexBotPlugin,

    sizeof( pluginProperties ) / sizeof( PropertyDescriptor* ),
    pluginProperties,
    PluginInitializer,
    nullptr,
    nullptr
);

// Complete properties description by initializing those parts, which were not
// initialized during properties array declaration
static void PluginInitializer( )
{
    // Port
    portProperty.DefaultValue.type = XVT_U2;
    portProperty.DefaultValue.value.usVal = 8000;

    portProperty.MinValue.type = XVT_U2;
    portProperty.MinValue.value.usVal = 1;

    portProperty.MaxValue.type = XVT_U2;
    portProperty.MaxValue.value.usVal = 65535;

    // Left Motor
    leftMotorProperty.DefaultValue.type = XVT_I1;
    leftMotorProperty.DefaultValue.value.bVal = 0;

    leftMotorProperty.MinValue.type = XVT_I1;
    leftMotorProperty.MinValue.value.bVal = -100;

    leftMotorProperty.MaxValue.type = XVT_I1;
    leftMotorProperty.MaxValue.value.bVal = 100;

    // Right Motor
    rightMotorProperty.DefaultValue.type = XVT_I1;
    rightMotorProperty.DefaultValue.value.bVal = 0;

    rightMotorProperty.MinValue.type = XVT_I1;
    rightMotorProperty.MinValue.value.bVal = -100;

    rightMotorProperty.MaxValue.type = XVT_I1;
    rightMotorProperty.MaxValue.value.bVal = 100;
}
