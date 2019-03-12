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

#include "PiRexBotPlugin.hpp"
#include <string.h>
#include <XHttpCommandsThread.hpp>
#include <XVariant.hpp>
#include <XMutex.hpp>
#include <XManualResetEvent.hpp>

using namespace std;
using namespace CVSandbox;
using namespace CVSandbox::Video;
using namespace CVSandbox::Video::MJpeg;
using namespace CVSandbox::Threading;

// Version request timeout (ms). When connectiong to PiRex Bot, the first request to send is always
// version query, so we could check we are talking to the right device. So the below is the timeout
// for getting version response.
#define VERSION_REQUEST_TIMEOUT  (5000)

// Distance request timeout (ms)
#define DISTANCE_REQUEST_TIMEOUT (200)

// URL to use to query PiRex Bot version
static const char* URL_VERSION       = "/version";

// URL to use to set motors' power
static const char* URL_MOTORS_CONFIG = "/motors/config";

// URL to use to query distance to obstacle
static const char* URL_DISTANCE      = "/distance";

namespace Private
{
    class PiRexBotPluginData : public IHttpCommandResultListener
    {
    public:
        XMutex                          Sync;
        shared_ptr<XHttpCommandsThread> CommandsThread;
        string                          Address;
        string                          UserName;
        string                          Password;
        uint16_t                        Port;

        bool                            IsConnected;
        string                          BotVersion;
        int8_t                          LeftMotorPower;
        int8_t                          RightMotorPower;
        float                           ObstacleDistance;

        uint32_t                        VersionRequestId;
        uint32_t                        VersionRequestCounter;
        XManualResetEvent               VersionEvent;

        uint32_t                        DistanceRequestId;
        XManualResetEvent               DistanceEvent;

    public:
        PiRexBotPluginData( ) :
            Sync( ), CommandsThread( XHttpCommandsThread::Create( ) ),
            Address( ), UserName( ), Password( ), Port( 8000 ),
            IsConnected( false ), BotVersion( ),
            LeftMotorPower( 0 ), RightMotorPower( 0 ), ObstacleDistance( 0 ),
            VersionRequestId( 0 ), VersionRequestCounter( 0 ), VersionEvent( ),
            DistanceRequestId( 0 ), DistanceEvent( )
        {
        }

        ~PiRexBotPluginData( )
        {
            Disconnect( );
        }

        XErrorCode Connect( );
        void Disconnect( );
        void UpdateMotors( );

    public:     // IHttpCommandResultListener interface

        // Successful completion of HTTP command
        virtual void OnCompletion( uint32_t requestId, const uint8_t* data, uint32_t dataLength );

        // HTTP command error notification
        virtual void OnError( uint32_t requestId, const string& errorMessage );
    };
}

PiRexBotPlugin::PiRexBotPlugin( ) :
    mData( new ::Private::PiRexBotPluginData( ) )
{
}

PiRexBotPlugin::~PiRexBotPlugin( )
{
    delete mData;
}

void PiRexBotPlugin::Dispose( )
{
    delete this;
}

// Open Phidget Interface Kit
XErrorCode PiRexBotPlugin::Connect( )
{
    return mData->Connect( );
}

// Close from Phidget Interface Kit
void PiRexBotPlugin::Disconnect( )
{
    mData->Disconnect( );
}

// Check if device is open
bool PiRexBotPlugin::IsConnected( )
{
    return mData->IsConnected;
}

// Get specified property value of the plug-in
XErrorCode PiRexBotPlugin::GetProperty( int32_t id, xvariant* value ) const
{
    XScopedLock lock( &mData->Sync );
    XErrorCode  ret = SuccessCode;

    if ( ( !mData->IsConnected ) &&
         ( id >= 4 ) )
    {
        ret = ErrorNotConnected;
    }

    if ( ret == SuccessCode )
    {
        switch ( id )
        {
        case 0:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->Address.c_str( ) );
            break;

        case 1:
            value->type = XVT_U2;
            value->value.usVal = mData->Port;
            break;

        case 2:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->UserName.c_str( ) );
            break;

        case 3:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->Password.c_str( ) );
            break;

        case 4:
            value->type = XVT_String;
            value->value.strVal = XStringAlloc( mData->BotVersion.c_str( ) );
            break;

        case 5:
            value->type = XVT_I1;
            value->value.bVal = mData->LeftMotorPower;
            break;

        case 6:
            value->type = XVT_I1;
            value->value.bVal = mData->RightMotorPower;
            break;

        case 7:
            mData->DistanceEvent.Reset( );

            mData->DistanceRequestId = mData->CommandsThread->EnqueueGetRequest( URL_DISTANCE, true );

            if ( mData->DistanceEvent.Wait( DISTANCE_REQUEST_TIMEOUT ) )
            {
                value->value.fVal = mData->ObstacleDistance;
            }
            else
            {
                value->value.fVal = -1;
            }

            value->type = XVT_R4;

            break;

        default:
            ret = ErrorInvalidProperty;
        }
    }

    return ret;
}

// Set specified property value of the plug-in
XErrorCode PiRexBotPlugin::SetProperty( int32_t id, const xvariant* value )
{
    XScopedLock lock( &mData->Sync );
    XErrorCode  ret = SuccessCode;

    xvariant convertedValue;
    XVariantInit( &convertedValue );

    // make sure property value has expected type
    ret = PropertyChangeTypeHelper( id, value, propertiesDescription, 8, &convertedValue );

    if ( ( ret == SuccessCode ) &&
         ( ( id == 4 ) || ( id == 7 ) ) )
    {
        ret = ErrorReadOnlyProperty;
    }

    if ( ret == SuccessCode )
    {
        switch ( id )
        {
        case 0:
            mData->Address = convertedValue.value.strVal;
            break;

        case 1:
            mData->Port = convertedValue.value.usVal;
            break;

        case 2:
            mData->UserName = convertedValue.value.strVal;
            break;

        case 3:
            mData->Password = convertedValue.value.strVal;
            break;

        case 5:
            mData->LeftMotorPower = XINRANGE( convertedValue.value.bVal, -100, 100 );
            mData->UpdateMotors( );
            break;

        case 6:
            mData->RightMotorPower = XINRANGE( convertedValue.value.bVal, -100, 100 );
            mData->UpdateMotors( );
            break;
        }
    }

    XVariantClear( &convertedValue );

    return ret;
}

namespace Private
{

// Connect to PiRex robot
XErrorCode PiRexBotPluginData::Connect( )
{
    XScopedLock lock( &Sync );
    XErrorCode  ret = SuccessCode;

    if ( Address.empty( ) )
    {
        ret = ErrorInvalidConfiguration;
    }
    else
    {
        char* baseUrl = new char[Address.length( ) + 32];

        sprintf( baseUrl, "http://%s:%u", Address.c_str( ), Port );

        // configure commands thread
        CommandsThread->SetBaseAddress( baseUrl );
        CommandsThread->SetAuthenticationCredentials( UserName, Password );
        CommandsThread->SetListener( static_cast<IHttpCommandResultListener*>( this ) );

        // start commands' thread
        ret = CommandsThread->Start( );

        if ( ret == SuccessCode )
        {
            // enqueue version request, so we could check if we are really talking to PiRex Bot
            VersionEvent.Reset( );
            BotVersion.clear( );

            VersionRequestCounter = 0;
            VersionRequestId      = CommandsThread->EnqueueGetRequest( URL_VERSION );

            if ( ( VersionEvent.Wait( VERSION_REQUEST_TIMEOUT ) ) && ( !BotVersion.empty( ) ) )
            {
                IsConnected = true;
            }
            else
            {
                ret = ErrorConnectionFailed;
                Disconnect( );
            }
        }

        delete [] baseUrl;
    }

    return ret;
}

// Disconnect from the robot
void PiRexBotPluginData::Disconnect( )
{
    XScopedLock lock( &Sync );

    if ( CommandsThread->IsRunning( ) )
    {
        CommandsThread->SignalToStop( );
        CommandsThread->WaitForStop( );
    }
}

// Send new motors' state to the robot
void PiRexBotPluginData::UpdateMotors( )
{
    char buffer[64];

    sprintf( buffer, "{\"leftPower\":%d, \"rightPower\":%d}", LeftMotorPower, RightMotorPower );
    CommandsThread->EnqueuePostRequest( URL_MOTORS_CONFIG, buffer, true );
}

// Successful completion of HTTP command
void PiRexBotPluginData::OnCompletion( uint32_t requestId, const uint8_t* data, uint32_t dataLength )
{
    char buffer[256] = { 0 };
    strncpy( buffer, (const char*) data, 255 );

    XUNREFERENCED_PARAMETER( dataLength )

    if ( requestId == VersionRequestId )
    {
        if ( ( strstr( (char*) data, "\"status\":\"OK\"" ) != nullptr ) &&
             ( strstr( (char*) data, "\"product\":\"pirexbot\"" ) != nullptr ) )
        {
            char* versionStart = strstr( (char*) data, "\"version\":\"" );
            char* versionEnd   = nullptr;

            if ( versionStart != nullptr )
            {
                versionStart += 11;
                versionEnd    = strchr( versionStart, '"' );

                if ( versionEnd != nullptr )
                {
                    BotVersion = string( versionStart, static_cast<size_t>( versionEnd - versionStart ) );
                }
            }
        }

        VersionRequestId = 0;
        VersionEvent.Signal( );
    }
    else if ( requestId == DistanceRequestId )
    {
        DistanceRequestId = 0;
        ObstacleDistance = -1;

        if ( strstr( (char*) data, "\"status\":\"OK\"" ) != nullptr )
        {
            char* distanceStart = strstr( (char*) data, "\"medianDistance\":\"" );
            char* distanceEnd   = nullptr;

            if ( distanceStart != nullptr )
            {
                distanceStart += 18;
                distanceEnd    = strchr( distanceStart, '"' );

                if ( distanceEnd != nullptr )
                {
                    distanceEnd = '\0';

                    if ( sscanf( distanceStart, "%f", &ObstacleDistance ) != 1 )
                    {
                        ObstacleDistance = -1;
                    }
                }
            }
        }

        DistanceEvent.Signal( );
    }
}

// HTTP command error notification
void PiRexBotPluginData::OnError( uint32_t requestId, const string& errorMessage )
{
    if ( requestId == VersionRequestId )
    {
        // if we get "HTTP 404" for version request, then it is definitely not what we expect
        if ( errorMessage.find( "HTTP 404" ) == 0 )
        {
            VersionRequestId = 0;
            VersionEvent.Signal( );
        }
        else
        {
            // if failed checking version, try few more times
            if ( VersionRequestCounter++ < 5 )
            {
                VersionRequestId = CommandsThread->EnqueueGetRequest( URL_VERSION );
            }
            else
            {
                VersionRequestId = 0;
                VersionEvent.Signal( );
            }
        }
    }
    else if ( requestId == DistanceRequestId )
    {
        DistanceRequestId = 0;
        ObstacleDistance  = -1;
        DistanceEvent.Signal( );
    }
}

} // namespace Private
