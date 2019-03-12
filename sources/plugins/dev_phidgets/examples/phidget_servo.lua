--[[
    Computer Vision Sandbox, Lua scripting sample
    http://www.cvsandbox.com/
    Copyright Andrew Kirillov, 2019

    Sample script which demonstrates how to talk
    to Phidget Advances Servo board.
]]--

servos = Host.CreatePluginInstance( 'PhidgetAdvancedServo' )

-- Connected to Phidget Servo board connected to the system
if not servos:Connect( ) then
    error( "Failed connecting to servo board" )
end

-- Check number of supported servos
motorCount = servos:GetProperty( 'motorCount' )

-- Configure velocity limit, acceleration and position range
servos:SetProperty( 'velocityLimit', { 2, 2 } )
servos:SetProperty( 'acceleration', { 20, 20 } )
servos:SetProperty( 'positionRange', { { 105, 115 }, { 135, 145 } } )

-- Engage both servos
servos:SetProperty( 'engaged', { true, true } )

-- Set target position of servo 1 and 2
servos:SetProperty( 'targetPosition', { 110, 140 } )

function Main( )
    -- Check actual position of servos and they are still moving
    actualPosition = servos:GetProperty( 'actualPosition' )
    stopped        = servos:GetProperty( 'stopped' )

    -- Set new target positions
    servos:SetProperty( 'targetPosition', 1, 115 )
    servos:SetProperty( 'targetPosition', 2, 135 )
end
