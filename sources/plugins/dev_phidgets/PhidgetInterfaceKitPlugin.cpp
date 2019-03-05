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

#include "PhidgetInterfaceKitPlugin.hpp"
#include "phidgets\phidget21.h"

#include <string>

using namespace std;

namespace Private
{
    class PhidgetInterfaceKitPluginData
    {
    public:
        CPhidgetInterfaceKitHandle KitHandle;

        bool     IsConnected;
        bool     IsAttached;

        int      SerialNumber;
        int      Version;
        string   DeviceType;
        bool     RatiometricState;

        uint32_t DigitalInputCount;
        uint32_t DigitalOutputCount;
        uint32_t AnalogInputCount;

    public:
        PhidgetInterfaceKitPluginData( ) :
            KitHandle( 0 ), IsConnected( false ), IsAttached( false ),
            SerialNumber( -1 ), Version( 0 ), DeviceType( ), RatiometricState( true ),
            DigitalInputCount( 0 ), DigitalOutputCount( 0 ), AnalogInputCount( 0 )
        {

        }

        ~PhidgetInterfaceKitPluginData( )
        {
            Close( );
        }

        // Callback to run when device is attached
        static int CCONV AttachHandler( CPhidgetHandle, void* ptr )
        {
            PhidgetInterfaceKitPluginData* data = static_cast<PhidgetInterfaceKitPluginData*>( ptr );
            const char* devType;

            CPhidgetInterfaceKit_setRatiometric( data->KitHandle, ( data->RatiometricState ) ? PTRUE : PFALSE );

            CPhidget_getDeviceType( reinterpret_cast<CPhidgetHandle>( data->KitHandle ), &devType );
            CPhidget_getSerialNumber( reinterpret_cast<CPhidgetHandle>( data->KitHandle ), &data->SerialNumber );
            CPhidget_getDeviceVersion( reinterpret_cast<CPhidgetHandle>( data->KitHandle ), &data->Version );

            int inputs = 0, outputs = 0, sensors = 0;

            CPhidgetInterfaceKit_getInputCount ( data->KitHandle, &inputs  );
            CPhidgetInterfaceKit_getOutputCount( data->KitHandle, &outputs );
            CPhidgetInterfaceKit_getSensorCount( data->KitHandle, &sensors );

            data->DigitalInputCount  = ( inputs  < 0 ) ? 0 : static_cast<uint32_t>( inputs );
            data->DigitalOutputCount = ( outputs < 0 ) ? 0 : static_cast<uint32_t>( outputs );
            data->AnalogInputCount   = ( sensors < 0 ) ? 0 : static_cast<uint32_t>( sensors );

            data->DeviceType = devType;
            data->IsAttached = true;

            return 0;
        }

        // Callback to run when device is detached
        static int CCONV DetachHandler( CPhidgetHandle, void* ptr )
        {
            PhidgetInterfaceKitPluginData* data = static_cast<PhidgetInterfaceKitPluginData*>( ptr );

            data->IsAttached = false;

            return 0;
        }

        /*
        // Callback to run if an input changes
        static int CCONV InputChangeHandler( CPhidgetInterfaceKitHandle, void *ptr, int index, int state )
        {
            return 0;
        }

        // Callback to run if an output changes
        static int CCONV OutputChangeHandler( CPhidgetInterfaceKitHandle, void *ptr, int index, int state )
        {
            return 0;
        }

        // Callback to run if the sensor value changes by more than the OnSensorChange trigger
        static int CCONV SensorChangeHandler( CPhidgetInterfaceKitHandle, void *ptr, int index, int value )
        {
            return 0;
        }
        */

        bool Open( )
        {
            // close anything previously opened
            Close( );

            CPhidgetInterfaceKit_create( &KitHandle );

            // set callbacks
            CPhidget_set_OnAttach_Handler( reinterpret_cast<CPhidgetHandle>( KitHandle ), AttachHandler, this );
            CPhidget_set_OnDetach_Handler( reinterpret_cast<CPhidgetHandle>( KitHandle ), DetachHandler, this );
            //CPhidgetInterfaceKit_set_OnInputChange_Handler( KitHandle, InputChangeHandler, this );
            //CPhidgetInterfaceKit_set_OnSensorChange_Handler( KitHandle, SensorChangeHandler, this );
            //CPhidgetInterfaceKit_set_OnOutputChange_Handler( KitHandle, OutputChangeHandler, this );

            CPhidget_open( reinterpret_cast<CPhidgetHandle>( KitHandle ), SerialNumber );

            // wait for an interface kit device to be attached
            if ( CPhidget_waitForAttachment( reinterpret_cast<CPhidgetHandle>( KitHandle ), 1000 ) == 0 )
            {
                IsConnected = true;
            }

            return IsConnected;
        }

        void Close( )
        {
            if ( KitHandle != 0 )
            {
                CPhidget_close( reinterpret_cast<CPhidgetHandle>( KitHandle ) );
                CPhidget_delete( reinterpret_cast<CPhidgetHandle>( KitHandle ) );
                KitHandle = 0;
            }

            IsConnected = false;
            IsAttached  = false;
        }

        XErrorCode SetRatiometricState( bool state )
        {
            XErrorCode ret = ErrorIOFailure;

            if ( IsAttached )
            {
                CPhidgetInterfaceKit_setRatiometric( KitHandle, ( state ) ? PTRUE : PFALSE );
                RatiometricState = state;
                ret = SuccessCode;
            }

            return ret;
        }
    };
}

PhidgetInterfaceKitPlugin::PhidgetInterfaceKitPlugin( ) :
    mData( new ::Private::PhidgetInterfaceKitPluginData )
{
}

void PhidgetInterfaceKitPlugin::Dispose( )
{
    delete mData;
    delete this;
}

// Open Phidget Interface Kit
XErrorCode PhidgetInterfaceKitPlugin::Connect( )
{
    return ( mData->Open( ) ) ? SuccessCode : ErrorConnectionFailed;
}

// Close from Phidget Interface Kit
void PhidgetInterfaceKitPlugin::Disconnect( )
{
    mData->Close( );
}

// Check if device is open
bool PhidgetInterfaceKitPlugin::IsConnected( )
{
    return mData->IsConnected;
}

// Get specified property value of the plug-in
XErrorCode PhidgetInterfaceKitPlugin::GetProperty( int32_t id, xvariant* value ) const
{
    XErrorCode ret = SuccessCode;

    if ( ( id >= 2 ) && ( id <= 6 ) && ( !mData->IsConnected ) )
    {
        ret = ErrorNotConnected;
    }
    else if ( ( id >= 7 ) && ( id <= 9 ) && ( !mData->IsAttached ) )
    {
        ret = ErrorIOFailure;
    }

    if ( ret == SuccessCode )
    {
        switch ( id )
        {
        case 0:
            value->type = XVT_I4;
            value->value.iVal = mData->SerialNumber;
            break;

        case 1:
            value->type = XVT_Bool;
            value->value.boolVal = mData->RatiometricState;
            break;

        case 2:
            value->type = XVT_I4;
            value->value.iVal = mData->Version;
            break;

        case 3:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->DeviceType.c_str( ) );
            break;

        case 4:
            value->type = XVT_U4;
            value->value.iVal = mData->DigitalInputCount;
            break;

        case 5:
            value->type = XVT_U4;
            value->value.iVal = mData->DigitalOutputCount;
            break;

        case 6:
            value->type = XVT_U4;
            value->value.iVal = mData->AnalogInputCount;
            break;

        case 7:
            {
                xarray*  array = nullptr;
                xvariant v;

                ret = XArrayAllocate( &array, XVT_Bool, mData->DigitalInputCount );

                if ( ret == SuccessCode )
                {
                    v.type = XVT_Bool;

                    for ( uint32_t i = 0, n = mData->DigitalInputCount; i < n; i++ )
                    {
                        int state = PFALSE;

                        if ( CPhidgetInterfaceKit_getInputState( mData->KitHandle, i, &state ) == EPHIDGET_OK )
                        {
                            v.value.boolVal = ( state == PTRUE );
                            XArraySet( array, i, &v );
                        }
                    }

                    value->type = XVT_Bool | XVT_Array;
                    value->value.arrayVal = array;
                }
            }
            break;

        case 8:
            {
                xarray*  array = nullptr;
                xvariant v;

                ret = XArrayAllocate( &array, XVT_Bool, mData->DigitalOutputCount );

                if ( ret == SuccessCode )
                {
                    v.type = XVT_Bool;

                    for ( uint32_t i = 0, n = mData->DigitalOutputCount; i < n; i++ )
                    {
                        int state = PFALSE;

                        if ( CPhidgetInterfaceKit_getOutputState( mData->KitHandle, i, &state ) == EPHIDGET_OK )
                        {
                            v.value.boolVal = ( state == PTRUE );
                            XArraySet( array, i, &v );
                        }
                    }

                    value->type = XVT_Bool | XVT_Array;
                    value->value.arrayVal = array;
                }
            }
            break;

        case 9:
            {
                xarray*  array = nullptr;
                xvariant v;

                ret = XArrayAllocate( &array, XVT_U2, mData->AnalogInputCount );

                if ( ret == SuccessCode )
                {
                    v.type = XVT_U2;

                    for ( uint32_t i = 0, n = mData->AnalogInputCount; i < n; i++ )
                    {
                        int sensorValue = 0;

                        if ( CPhidgetInterfaceKit_getSensorValue( mData->KitHandle, i, &sensorValue ) == EPHIDGET_OK )
                        {
                            v.value.usVal = static_cast<uint16_t>( sensorValue );
                            XArraySet( array, i, &v );
                        }
                    }

                    value->type = XVT_U2 | XVT_Array;
                    value->value.arrayVal = array;
                }
            }
            break;

        default:
            ret = ErrorInvalidProperty;
            break;
        }
    }

    return ret;
}

// Set specified property value of the plug-in
XErrorCode PhidgetInterfaceKitPlugin::SetProperty( int32_t id, const xvariant* value )
{
    XErrorCode ret = SuccessCode;

    xvariant convertedValue;
    XVariantInit( &convertedValue );

    // make sure property value has expected type
    ret = PropertyChangeTypeHelper( id, value, propertiesDescription, 10, &convertedValue );

    if ( ret == SuccessCode )
    {
        if ( id == 0 )
        {
            mData->SerialNumber = convertedValue.value.iVal;
        }
        else if ( id == 1 )
        {
            ret = mData->SetRatiometricState( convertedValue.value.boolVal );
        }
        else if ( id == 8 )
        {
            xarray*  arrayVar = value->value.arrayVal;
            uint32_t arrayLen = XArrayLength( arrayVar );
            uint32_t toSet    = XMIN( arrayLen, mData->DigitalInputCount );

            for ( uint32_t i = 0; i < toSet; i++ )
            {
                xvariant arrayElement;
                XVariantInit( &arrayElement );

                if ( XArrayGet( arrayVar, i, &arrayElement ) == SuccessCode )
                {
                    if ( CPhidgetInterfaceKit_setOutputState( mData->KitHandle, i,
                        ( arrayElement.value.boolVal ) ? PTRUE : PFALSE ) != EPHIDGET_OK )
                    {
                        ret = ErrorIOFailure;
                    }
                }

                XVariantClear( &arrayElement );
            }
        }
        else if ( id > 9 )
        {
            ret = ErrorInvalidProperty;
        }
        else
        {
            ret = ErrorReadOnlyProperty;
        }
    }

    XVariantClear( &convertedValue );

    return ret;
}

// Get an element of the specified indexed property of the plug-in
XErrorCode PhidgetInterfaceKitPlugin::GetIndexedProperty( int32_t id, uint32_t index, xvariant* value ) const
{
    XErrorCode ret = SuccessCode;

    if ( id < 7 )
    {
        ret = ErrorNotIndexedProperty;
    }
    else if ( id > 9 )
    {
        ret = ErrorInvalidProperty;
    }
    else if ( !mData->IsAttached )
    {
        ret = ErrorIOFailure;
    }
    else if ( ( ( id == 7 ) && ( index >= mData->DigitalInputCount  ) ) ||
              ( ( id == 8 ) && ( index >= mData->DigitalOutputCount ) ) ||
              ( ( id == 9 ) && ( index >= mData->AnalogInputCount   ) ) )
    {
        ret = ErrorIndexOutOfBounds;
    }
    else
    {
        switch ( id )
        {
        case 7:
            {
                int state = PFALSE;

                if ( CPhidgetInterfaceKit_getInputState( mData->KitHandle, index, &state ) != EPHIDGET_OK )
                {
                    ret = ErrorIOFailure;
                }
                else
                {
                    value->type = XVT_Bool;
                    value->value.boolVal = ( state == PTRUE );
                }
            }
            break;

        case 8:
            {
                int state = PFALSE;

                if ( CPhidgetInterfaceKit_getOutputState( mData->KitHandle, index, &state ) != EPHIDGET_OK )
                {
                    ret = ErrorIOFailure;
                }
                else
                {
                    value->type = XVT_Bool;
                    value->value.boolVal = ( state == PTRUE );
                }
            }
            break;

        case 9:
            {
                int sensorValue = 0;

                if ( CPhidgetInterfaceKit_getSensorValue( mData->KitHandle, index, &sensorValue ) != EPHIDGET_OK )
                {
                    ret = ErrorIOFailure;
                }
                else
                {
                    value->type = XVT_U2;
                    value->value.usVal = static_cast<uint16_t>( sensorValue );
                }
            }
            break;
        }
    }

    return ret;
}

// Set an element of the specified indexed property of the plug-in
XErrorCode PhidgetInterfaceKitPlugin::SetIndexedProperty( int32_t id, uint32_t index, const xvariant* value )
{
    XErrorCode ret = SuccessCode;

    if ( id < 7 )
    {
        ret = ErrorNotIndexedProperty;
    }
    else if ( id > 9 )
    {
        ret = ErrorInvalidProperty;
    }
    else if ( id != 8 )
    {
        ret = ErrorReadOnlyProperty;
    }
    else if ( !mData->IsAttached )
    {
        ret = ErrorIOFailure;
    }
    else if ( index >= mData->DigitalOutputCount )
    {
        ret = ErrorIndexOutOfBounds;
    }
    else
    {
        xvariant convertedValue;
        XVariantInit( &convertedValue );

        // make sure property value has expected type
        ret = PropertyChangeTypeIndexedHelper( id, value, propertiesDescription, 10, &convertedValue );

        if ( CPhidgetInterfaceKit_setOutputState( mData->KitHandle, index,
                            ( convertedValue.value.boolVal ) ? PTRUE : PFALSE ) != EPHIDGET_OK )
        {
            ret = ErrorIOFailure;
        }

        XVariantClear( &convertedValue );
    }

    return ret;
}
