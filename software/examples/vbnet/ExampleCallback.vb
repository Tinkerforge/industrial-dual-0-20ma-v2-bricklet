Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Industrial Dual 0-20mA Bricklet 2.0

    ' Callback subroutine for current callback
    Sub CurrentCB(ByVal sender As BrickletIndustrialDual020mAV2, ByVal channel As Byte, _
                  ByVal current As Integer)
        Console.WriteLine("Channel: " + channel.ToString())
        Console.WriteLine("Current: " + (current/1000000.0).ToString() + " mA")
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim id020 As New BrickletIndustrialDual020mAV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register current callback to subroutine CurrentCB
        AddHandler id020.CurrentCallback, AddressOf CurrentCB

        ' Set period for current (channel 0) callback to 1s (1000ms) without a threshold
        id020.SetCurrentCallbackConfiguration(0, 1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
