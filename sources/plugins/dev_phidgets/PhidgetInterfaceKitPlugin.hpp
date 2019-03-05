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

#ifndef CVS_PHIDGET_INTERFACE_KIT_PLUGIN_HPP
#define CVS_PHIDGET_INTERFACE_KIT_PLUGIN_HPP

#include <iplugintypescpp.hpp>

namespace Private
{
    class PhidgetInterfaceKitPluginData;
}

class PhidgetInterfaceKitPlugin : public IDevicePlugin
{
public:
    PhidgetInterfaceKitPlugin( );

    // IPluginBase interface
    virtual void Dispose( );

    virtual XErrorCode GetProperty( int32_t id, xvariant* value ) const;
    virtual XErrorCode SetProperty( int32_t id, const xvariant* value );

    virtual XErrorCode GetIndexedProperty( int32_t id, uint32_t index, xvariant* value ) const;
    virtual XErrorCode SetIndexedProperty( int32_t id, uint32_t index, const xvariant* value );

    // IDevicePlugin interface
    virtual XErrorCode Connect( );
    virtual void Disconnect( );
    virtual bool IsConnected( );

private:
    static const PropertyDescriptor**         propertiesDescription;
    ::Private::PhidgetInterfaceKitPluginData* mData;
};

#endif // CVS_PHIDGET_INTERFACE_KIT_PLUGIN_HPP
