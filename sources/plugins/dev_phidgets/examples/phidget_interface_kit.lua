--[[
    Computer Vision Sandbox, Lua scripting sample
    http://www.cvsandbox.com/
    Copyright Andrew Kirillov, 2019

    Sample script which demonstrates how to talk
    to Phidget Interface Kit board.
]]--

kit = Host.CreatePluginInstance( 'PhidgetInterfaceKit' )

-- Connected to Phidget Interface Kit board plugged into the system
if not kit:Connect( ) then
    error( "Failed connecting to interface kit board" )
end

-- Check number of available digital/analog I/O
digitalInputCount  = kit:GetProperty( 'digitalInputCount' )
digitalOutputCount = kit:GetProperty( 'digitalOutputCount' )
analogInputCount   = kit:GetProperty( 'analogInputCount' )

-- Switch OFF all digital inputs (assuming 8 inputs available)
kit:SetProperty( 'digitalOutputs', { false, false, false, false,
                                     false, false, false, false } )

function Main( )
    -- Switch ON 1st and 2nd digital outputs
    kit:SetProperty( 'digitalOutputs', { true, true } )
    -- Also switch ON the 7th output
    kit:SetProperty( 'digitalOutputs', 7, true )

    -- Read digital/analog inputs
    analogInputs  = kit:GetProperty( 'analogInputs' )
    digitalInputs = kit:GetProperty( 'digitalInputs' )

    for i = 1, #analogInputs do
        print( 'Analog input', i, 'is', analogInputs[i] )
    end

    for i = 1, #digitalInputs do
        print( 'Digital input', i, 'is', digitalInputs[i] )
    end
end
