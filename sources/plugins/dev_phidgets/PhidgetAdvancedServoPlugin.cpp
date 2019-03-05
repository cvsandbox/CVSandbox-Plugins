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

#include "PhidgetAdvancedServoPlugin.hpp"
#include "phidgets\phidget21.h"

#include <string>

using namespace std;

namespace Private
{
    class PhidgetAdvancedServoPluginData
    {
    public:
        CPhidgetAdvancedServoHandle DeviceHandle;

        bool     IsConnected;
        bool     IsAttached;

        int      SerialNumber;
        int      Version;
        string   DeviceType;

        uint32_t MotorCount;
        float*   TargetPositions;

    public:
        PhidgetAdvancedServoPluginData( ) :
            DeviceHandle( 0 ), IsConnected( false ), IsAttached( false ),
            SerialNumber( -1 ), Version( 0 ), DeviceType( ),
            MotorCount( 0 ), TargetPositions( nullptr )
        {

        }

        ~PhidgetAdvancedServoPluginData( )
        {
            delete [] TargetPositions;
            Close( );
        }

        // Callback to run when device is attached
        static int CCONV AttachHandler( CPhidgetHandle, void* ptr )
        {
            PhidgetAdvancedServoPluginData* data = static_cast<PhidgetAdvancedServoPluginData*>( ptr );
            const char* devType;

            CPhidget_getDeviceType( reinterpret_cast<CPhidgetHandle>( data->DeviceHandle ), &devType );
            CPhidget_getSerialNumber( reinterpret_cast<CPhidgetHandle>( data->DeviceHandle ), &data->SerialNumber );
            CPhidget_getDeviceVersion( reinterpret_cast<CPhidgetHandle>( data->DeviceHandle ), &data->Version );

            int motors = 0;

            CPhidgetAdvancedServo_getMotorCount( data->DeviceHandle, &motors );

            data->MotorCount = ( motors < 0 ) ? 0 : static_cast<uint32_t>( motors );
            data->DeviceType = devType;
            data->IsAttached = true;

            data->TargetPositions = new (nothrow) float[motors];

            if ( data->TargetPositions )
            {
                for ( int i = 0; i < motors; i++ )
                {
                    data->TargetPositions[i] = 0;
                }
            }

            return 0;
        }

        // Callback to run when device is detached
        static int CCONV DetachHandler( CPhidgetHandle, void* ptr )
        {
            PhidgetAdvancedServoPluginData* data = static_cast<PhidgetAdvancedServoPluginData*>( ptr );

            data->IsAttached = false;

            return 0;
        }

        bool Open( )
        {
            // close anything previosly opened
            Close( );

            CPhidgetAdvancedServo_create( &DeviceHandle );

            // set callbacks
            CPhidget_set_OnAttach_Handler( reinterpret_cast<CPhidgetHandle>( DeviceHandle ), AttachHandler, this );
            CPhidget_set_OnDetach_Handler( reinterpret_cast<CPhidgetHandle>( DeviceHandle ), DetachHandler, this );

            CPhidget_open( reinterpret_cast<CPhidgetHandle>( DeviceHandle ), SerialNumber );

            // wait for an interface kit device to be attached
            if ( CPhidget_waitForAttachment( reinterpret_cast<CPhidgetHandle>( DeviceHandle ), 1000 ) == 0 )
            {
                IsConnected = true;
            }

            return IsConnected;
        }

        void Close( )
        {
            if ( DeviceHandle != 0 )
            {
                CPhidget_close( reinterpret_cast<CPhidgetHandle>( DeviceHandle ) );
                CPhidget_delete( reinterpret_cast<CPhidgetHandle>( DeviceHandle ) );
                DeviceHandle = 0;
            }

            IsConnected = false;
            IsAttached  = false;
        }

        typedef bool ( *SetMotorValueCallback )( uint32_t motor, double value, PhidgetAdvancedServoPluginData* data );
        typedef bool ( *GetMotorValueCallback )( uint32_t motor, double* value, PhidgetAdvancedServoPluginData* data );
        typedef bool ( *GetMotorBoolValueCallback )( uint32_t motor, int* value, PhidgetAdvancedServoPluginData* data );
        typedef bool ( *GetMotorRangeValueCallback )( uint32_t motor, double* min, double* max, PhidgetAdvancedServoPluginData* data );

        // Calls the specified callback for each motor to set some value
        XErrorCode SetMotorValues( xvariant* variantArray, SetMotorValueCallback callback )
        {
            XErrorCode ret      = SuccessCode;
            xarray*    values   = variantArray->value.arrayVal;
            uint32_t   arrayLen = XArrayLength( values );
            uint32_t   toSet    = XMIN( arrayLen, MotorCount );

            for ( uint32_t i = 0; i < toSet; i++ )
            {
                xvariant arrayElement;
                XVariantInit( &arrayElement );

                if ( XArrayGet( values, i, &arrayElement ) == SuccessCode )
                {
                    if ( !callback( i, arrayElement.value.fVal, this ) )
                    {
                        ret = ErrorIOFailure;
                    }
                }

                XVariantClear( &arrayElement );
            }

            return ret;
        }

        // Calls the specified callback for a single motor to set some value
        XErrorCode SetMotorValue( xvariant* variantValue, uint32_t motor, SetMotorValueCallback callback )
        {
            XErrorCode ret = SuccessCode;

            if ( !callback( motor, variantValue->value.fVal, this ) )
            {
                ret = ErrorIOFailure;
            }

            return ret;
        }

        // Calls the specified callback for each motor to get some value
        XErrorCode GetMotorValues( xvariant* variantArray, GetMotorValueCallback callback )
        {
            XErrorCode ret   = SuccessCode;
            xarray*    array = nullptr;
            xvariant   v;

            ret = XArrayAllocate( &array, XVT_R4, MotorCount );

            if ( ret == SuccessCode )
            {
                v.type = XVT_R4;

                for ( uint32_t i = 0, n = MotorCount; i < n; i++ )
                {
                    double value = 0;

                    if ( callback( i, &value, this ) )
                    {
                        v.value.fVal = static_cast<float>( value );
                        XArraySet( array, i, &v );
                    }
                }

                variantArray->type = XVT_R4 | XVT_Array;
                variantArray->value.arrayVal = array;
            }

            return ret;
        }

        // Calls the specified callback for a single motor to get some value
        XErrorCode GetMotorValue( xvariant* variantValue, uint32_t motor, GetMotorValueCallback callback )
        {
            XErrorCode ret = SuccessCode;
            double     value;

            if ( !callback( motor, &value, this ) )
            {
                ret = ErrorIOFailure;
            }
            else
            {
                variantValue->type = XVT_R4;
                variantValue->value.fVal = static_cast<float>( value );
            }

            return ret;
        }

        // Calls the specified callback for each motor to get some value (boolean type)
        XErrorCode GetMotorBoolValues( xvariant* variantArray, GetMotorBoolValueCallback callback )
        {
            XErrorCode ret = SuccessCode;
            xarray*    array = nullptr;
            xvariant   v;

            ret = XArrayAllocate( &array, XVT_Bool, MotorCount );

            if ( ret == SuccessCode )
            {
                v.type = XVT_Bool;

                for ( uint32_t i = 0, n = MotorCount; i < n; i++ )
                {
                    int value = 0;

                    if ( callback( i, &value, this ) )
                    {
                        v.value.boolVal = ( value == PTRUE );
                        XArraySet( array, i, &v );
                    }
                }

                variantArray->type = XVT_Bool | XVT_Array;
                variantArray->value.arrayVal = array;
            }

            return ret;
        }

        // Calls the specified callback for a single motor to get some value (boolean type)
        XErrorCode GetMotorBoolValue( xvariant* variantValue, uint32_t motor, GetMotorBoolValueCallback callback )
        {
            XErrorCode ret = SuccessCode;
            int        value;

            if ( !callback( motor, &value, this ) )
            {
                ret = ErrorIOFailure;
            }
            else
            {
                variantValue->type = XVT_Bool;
                variantValue->value.boolVal = ( value == PTRUE );
            }

            return ret;
        }

        // Calls the specified callback for each motor to get some range value
        XErrorCode GetMotorRangeValues( xvariant* variantArray, GetMotorRangeValueCallback callback )
        {
            XErrorCode ret   = SuccessCode;
            xarray*    array = nullptr;
            xvariant   v;

            ret = XArrayAllocate( &array, XVT_RangeF, MotorCount );

            if ( ret == SuccessCode )
            {
                v.type = XVT_RangeF;

                for ( uint32_t i = 0, n = MotorCount; i < n; i++ )
                {
                    double min = 0, max = 0;

                    if ( callback( i, &min, &max, this ) )
                    {
                        v.value.frangeVal.min = static_cast<float>( min );
                        v.value.frangeVal.max = static_cast<float>( max );
                        XArraySet( array, i, &v );
                    }
                }

                variantArray->type = XVT_RangeF | XVT_Array;
                variantArray->value.arrayVal = array;
            }

            return ret;
        }

        // Calls the specified callback for a single motor to get some range value
        XErrorCode GetMotorRangeValue( xvariant* variantValue, uint32_t motor, GetMotorRangeValueCallback callback )
        {
            XErrorCode ret = SuccessCode;
            double     min = 0, max = 0;

            if ( !callback( motor, &min, &max, this ) )
            {
                ret = ErrorIOFailure;
            }
            else
            {
                variantValue->type = XVT_RangeF;
                variantValue->value.frangeVal.min = static_cast<float>( min );
                variantValue->value.frangeVal.max = static_cast<float>( max );
            }

            return ret;
        }
    };
}

PhidgetAdvancedServoPlugin::PhidgetAdvancedServoPlugin( ) :
    mData( new ::Private::PhidgetAdvancedServoPluginData )
{
}

void PhidgetAdvancedServoPlugin::Dispose( )
{
    delete mData;
    delete this;
}

// Open Phidget Interface Kit
XErrorCode PhidgetAdvancedServoPlugin::Connect( )
{
    return ( mData->Open( ) ) ? SuccessCode : ErrorConnectionFailed;
}

// Close from Phidget Interface Kit
void PhidgetAdvancedServoPlugin::Disconnect( )
{
    mData->Close( );
}

// Check if device is open
bool PhidgetAdvancedServoPlugin::IsConnected( )
{
    return mData->IsConnected;
}

// Helper function to get different values of motors
bool GetEngagedCallback( uint32_t motor, int* value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_getEngaged( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool GetStoppedCallback( uint32_t motor, int* value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_getStopped( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool GetActualPositionCallback( uint32_t motor, double* value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_getPosition( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool GetTargetPositionCallback( uint32_t motor, double* value, Private::PhidgetAdvancedServoPluginData* data )
{
    *value = data->TargetPositions[motor];
    return true;
}
bool GetVelocityLimitCallback( uint32_t motor, double* value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_getVelocityLimit( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool GetAccelerationCallback( uint32_t motor, double* value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_getAcceleration( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool GetPositionRangeCallback( uint32_t motor, double* min, double* max, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( ( CPhidgetAdvancedServo_getPositionMin( data->DeviceHandle, motor, min ) == EPHIDGET_OK ) &&
             ( CPhidgetAdvancedServo_getPositionMax( data->DeviceHandle, motor, max ) == EPHIDGET_OK ) );
}
bool GetVelocityRangeCallback( uint32_t motor, double* min, double* max, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( ( CPhidgetAdvancedServo_getVelocityMin( data->DeviceHandle, motor, min ) == EPHIDGET_OK ) &&
             ( CPhidgetAdvancedServo_getVelocityMax( data->DeviceHandle, motor, max ) == EPHIDGET_OK ) );
}
bool GetAccelerationRangeCallback( uint32_t motor, double* min, double* max, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( ( CPhidgetAdvancedServo_getAccelerationMin( data->DeviceHandle, motor, min ) == EPHIDGET_OK ) &&
             ( CPhidgetAdvancedServo_getAccelerationMax( data->DeviceHandle, motor, max ) == EPHIDGET_OK ) );
}

// Get specified property value of the plug-in
XErrorCode PhidgetAdvancedServoPlugin::GetProperty( int32_t id, xvariant* value ) const
{
    XErrorCode ret = SuccessCode;

    if ( ( id >= 1 ) && ( id <= 3) && ( !mData->IsConnected ) )
    {
        ret = ErrorNotConnected;
    }
    else if ( ( id >= 4 ) && ( id <= 12 ) && ( !mData->IsAttached ) )
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
            value->type = XVT_I4;
            value->value.iVal = mData->Version;
            break;

        case 2:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->DeviceType.c_str( ) );
            break;

        case 3:
            value->type = XVT_U4;
            value->value.iVal = mData->MotorCount;
            break;

        case 4:
            mData->GetMotorBoolValues( value, GetEngagedCallback );
            break;

        case 5:
            mData->GetMotorBoolValues( value, GetStoppedCallback );
            break;

        case 6:
            mData->GetMotorValues( value, GetActualPositionCallback );
            break;

        case 7:
            mData->GetMotorValues( value, GetTargetPositionCallback );
            break;

        case 8:
            mData->GetMotorValues( value, GetVelocityLimitCallback );
            break;

        case 9:
            mData->GetMotorValues( value, GetAccelerationCallback );
            break;

        case 10:
            mData->GetMotorRangeValues( value, GetPositionRangeCallback );
            break;

        case 11:
            mData->GetMotorRangeValues( value, GetVelocityRangeCallback );
            break;

        case 12:
            mData->GetMotorRangeValues( value, GetAccelerationRangeCallback );
            break;
        }
    }

    return ret;
}

// Helper functions to set values of motors
bool SetPositionCallback( uint32_t motor, double value, Private::PhidgetAdvancedServoPluginData* data )
{
    double pos = XINRANGE( value, 0.0, 220.0 );

    data->TargetPositions[motor] = static_cast<float>( pos );

    return ( CPhidgetAdvancedServo_setPosition( data->DeviceHandle, motor, pos ) == EPHIDGET_OK );
}
bool SetVelocityLimitCallback( uint32_t motor, double value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_setVelocityLimit( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}
bool SetAccelerationCallback( uint32_t motor, double value, Private::PhidgetAdvancedServoPluginData* data )
{
    return ( CPhidgetAdvancedServo_setAcceleration( data->DeviceHandle, motor, value ) == EPHIDGET_OK );
}

// Set specified property value of the plug-in
XErrorCode PhidgetAdvancedServoPlugin::SetProperty( int32_t id, const xvariant* value )
{
    XErrorCode ret = SuccessCode;

    xvariant convertedValue;
    XVariantInit( &convertedValue );

    // make sure property value has expected type
    ret = PropertyChangeTypeHelper( id, value, propertiesDescription, 13, &convertedValue );

    if ( ret == SuccessCode )
    {
        if ( id >= 13 )
        {
            ret = ErrorInvalidProperty;
        }
        else
        {
            switch ( id )
            {
            case 0:
                mData->SerialNumber = convertedValue.value.iVal;
                break;

            case 4:
                {
                    xarray*  arrayVar = convertedValue.value.arrayVal;
                    uint32_t arrayLen = XArrayLength( arrayVar );
                    uint32_t toSet    = XMIN( arrayLen, mData->MotorCount );

                    for ( uint32_t i = 0; i < toSet; i++ )
                    {
                        xvariant arrayElement;
                        XVariantInit( &arrayElement );

                        if ( XArrayGet( arrayVar, i, &arrayElement ) == SuccessCode )
                        {
                            if ( CPhidgetAdvancedServo_setEngaged( mData->DeviceHandle, i,
                                    ( arrayElement.value.boolVal ) ? PTRUE : PFALSE ) != EPHIDGET_OK )
                            {
                                ret = ErrorIOFailure;
                            }
                        }

                        XVariantClear( &arrayElement );
                    }
                }
                break;

            case 7:
                if ( mData->TargetPositions == nullptr )
                {
                    ret = ErrorOutOfMemory;
                }
                else
                {
                    mData->SetMotorValues( &convertedValue, SetPositionCallback );
                }
                break;

            case 8:
                mData->SetMotorValues( &convertedValue, SetVelocityLimitCallback );
                break;

            case 9:
                mData->SetMotorValues( &convertedValue, SetAccelerationCallback );
                break;

            case 10:
                {
                    xarray*  arrayVar = convertedValue.value.arrayVal;
                    uint32_t arrayLen = XArrayLength( arrayVar );
                    uint32_t toSet    = XMIN( arrayLen, mData->MotorCount );

                    for ( uint32_t i = 0; i < toSet; i++ )
                    {
                        xvariant arrayElement;
                        XVariantInit( &arrayElement );

                        if ( XArrayGet( arrayVar, i, &arrayElement ) == SuccessCode )
                        {
                            float min = XINRANGE( arrayElement.value.frangeVal.min, 0.0f, 220.0f );
                            float max = XINRANGE( arrayElement.value.frangeVal.max, 0.0f, 220.0f );

                            if ( ( CPhidgetAdvancedServo_setPositionMin( mData->DeviceHandle, i, min ) != EPHIDGET_OK ) ||
                                 ( CPhidgetAdvancedServo_setPositionMax( mData->DeviceHandle, i, max ) != EPHIDGET_OK ) )
                            {
                                ret = ErrorIOFailure;
                            }
                        }

                        XVariantClear( &arrayElement );
                    }
                }
                break;

            default:
                ret = ErrorReadOnlyProperty;
                break;
            }
        }
    }

    return ret;
}

// Get an element of the specified indexed property of the plug-in
XErrorCode PhidgetAdvancedServoPlugin::GetIndexedProperty( int32_t id, uint32_t index, xvariant* value ) const
{
    XErrorCode ret = SuccessCode;

    if ( id < 4 )
    {
        ret = ErrorNotIndexedProperty;
    }
    else if ( id > 12 )
    {
        ret = ErrorInvalidProperty;
    }
    else if ( !mData->IsAttached )
    {
        ret = ErrorIOFailure;
    }
    else if ( index >= mData->MotorCount )
    {
        ret = ErrorIndexOutOfBounds;
    }
    else
    {
        switch ( id )
        {
        case 4:
            mData->GetMotorBoolValue( value, index, GetStoppedCallback );
            break;

        case 5:
            mData->GetMotorBoolValue( value, index, GetStoppedCallback );
            break;

        case 6:
            mData->GetMotorValue( value, index, GetActualPositionCallback );
            break;

        case 7:
            mData->GetMotorValue( value, index, GetTargetPositionCallback );
            break;

        case 8:
            mData->GetMotorValue( value, index, GetVelocityLimitCallback );
            break;

        case 9:
            mData->GetMotorValue( value, index, GetAccelerationCallback );
            break;

        case 10:
            mData->GetMotorRangeValue( value, index, GetPositionRangeCallback );
            break;

        case 11:
            mData->GetMotorRangeValue( value, index, GetVelocityRangeCallback );
            break;

        case 12:
            mData->GetMotorRangeValue( value, index, GetAccelerationRangeCallback );
            break;
        }
    }

    return ret;
}

// Set an element of the specified indexed property of the plug-in
XErrorCode PhidgetAdvancedServoPlugin::SetIndexedProperty( int32_t id, uint32_t index, const xvariant* value )
{
    XErrorCode ret = SuccessCode;

    if ( id < 4 )
    {
        ret = ErrorNotIndexedProperty;
    }
    else if ( id > 12 )
    {
        ret = ErrorInvalidProperty;
    }
    else if ( ( index == 5 ) || ( index == 6 ) || ( index == 11 ) || ( index == 12 ) )
    {
        ret = ErrorReadOnlyProperty;
    }
    else if ( !mData->IsAttached )
    {
        ret = ErrorIOFailure;
    }
    else if ( index >= mData->MotorCount )
    {
        ret = ErrorIndexOutOfBounds;
    }
    else
    {
        xvariant convertedValue;
        XVariantInit( &convertedValue );

        // make sure property value has expected type
        ret = PropertyChangeTypeIndexedHelper( id, value, propertiesDescription, 13, &convertedValue );

        switch ( id )
        {
        case 4:
            if ( CPhidgetAdvancedServo_setEngaged( mData->DeviceHandle, index,
                    ( convertedValue.value.boolVal ) ? PTRUE : PFALSE ) != EPHIDGET_OK )
            {
                ret = ErrorIOFailure;
            }
            break;

        case 7:
            if ( mData->TargetPositions == nullptr )
            {
                ret = ErrorOutOfMemory;
            }
            else
            {
                mData->SetMotorValue( &convertedValue, index, SetPositionCallback );
            }
            break;

        case 8:
            mData->SetMotorValue( &convertedValue, index, SetVelocityLimitCallback );
            break;

        case 9:
            mData->SetMotorValue( &convertedValue, index, SetAccelerationCallback );
            break;

        case 10:
            {
                float min = XINRANGE( convertedValue.value.frangeVal.min, 0.0f, 220.0f );
                float max = XINRANGE( convertedValue.value.frangeVal.max, 0.0f, 220.0f );

                if ( ( CPhidgetAdvancedServo_setPositionMin( mData->DeviceHandle, index, min ) != EPHIDGET_OK ) ||
                     ( CPhidgetAdvancedServo_setPositionMax( mData->DeviceHandle, index, max ) != EPHIDGET_OK ) )
                {
                    ret = ErrorIOFailure;
                }
            }
            break;

        default:
            ret = ErrorReadOnlyProperty;
            break;
        }

        XVariantClear( &convertedValue );
    }
    return ret;
}
